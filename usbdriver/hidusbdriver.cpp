#include "stdafx.h"

#include "hidusb.h"


#include <string.h>
#include <stdlib.h>


#ifndef DEVICE_VID

#error The macro "DEVICE_VID" should be defined in "usbdriver\hidusb.h" file.

#endif


#ifndef DEVICE_PID

#error The macro "DEVICE_PID" should be defined in "usbdriver\hidusb.h" file.

#endif


HidUsb::HidUsb(void) : m_hRead(NULL), m_hWrite(NULL), m_hwDeviceInfo(NULL),
						eCommType(USB_COMM_TYPEDEF),
						m_pReadOverlapped(NULL), m_pWriteOverlapped(NULL)
						, deviceVID(DEVICE_VID), devicePID(DEVICE_PID)
{
	m_nBytesToRead = m_nBytesToWrite = 0UL;

	HidD_GetHidGuid(&m_hidGUID);
}

HidUsb::HidUsb(const char* vid, const char* pid) : m_hRead(NULL), m_hWrite(NULL), m_hwDeviceInfo(NULL),
						eCommType(USB_COMM_TYPEDEF),
						m_pReadOverlapped(NULL), m_pWriteOverlapped(NULL)
						, deviceVID(vid), devicePID(pid)
{
	m_nBytesToRead = m_nBytesToWrite = 0UL;

	HidD_GetHidGuid(&m_hidGUID);
}



HidUsb::~HidUsb(void)
{
	if(m_pReadOverlapped)
	{
		delete m_pReadOverlapped;

		m_pReadOverlapped = NULL;
	}

	if(m_pWriteOverlapped)
	{
		delete m_pWriteOverlapped;

		m_pWriteOverlapped = NULL;
	}
}


void HidUsb::ErrorReport(CString info)
{
	::MessageBox(NULL, info, "warning", MB_OK);

	DWORD dwErr = GetLastError();
}


BOOL HidUsb::ParsePath(LPCSTR pathname)
{
	LPCSTR p = pathname;

	LPCSTR q = deviceVID;

	// Parse VID
	while(*p)
	{
		if(*p++ != 'v')
			continue;

		if(*p++ != 'i')
			continue;

		if(*p++ != 'd')
			continue;

		if(*p++ != '_')
			continue;

		for(int i=0; i<4; i++)
		{
			if(*p++ != *q++)
				return FALSE;
		}

		break;
	}

	if(*p == '\0')
		return FALSE;

	// Parse PID
	while(*p)
	{
		if(*p++ != 'p')
			continue;

		if(*p++ != 'i')
			continue;

		if(*p++ != 'd')
			continue;

		if(*p++ != '_')
			continue;

		q = devicePID;

		for(int i=0; i<4; i++)
		{
			if(*p++ != *q++)
				return FALSE;
		}

		break;
	}

	if(*p == '\0')
		return FALSE;

	return TRUE;
}


BOOL HidUsb::EnumerateDevices(PSP_DEVINFO_DATA DeviceInfoData, PSP_DEVICE_INTERFACE_DATA DeviceInterfaceData,
								PSP_INTERFACE_DEVICE_DETAIL_DATA DeviceDetailInfoData)
{
	DWORD dwNameLength = 0;

	for(DWORD i=0; ; i++)
	{
		if(DeviceInfoData != NULL)
		{
			// Use this enumeration to get a specified device interface information
			if(!SetupDiEnumDeviceInfo(m_hwDeviceInfo, i, DeviceInfoData))
			{
				dwNameLength = GetLastError();		// just for debugging

				return FALSE;
			}
		}
		
		// Enumerate each device interface
		if(!SetupDiEnumInterfaceDevice(m_hwDeviceInfo, DeviceInfoData, &m_hidGUID, i, DeviceInterfaceData ))
		{
			dwNameLength = GetLastError();			// just for debugging

			return FALSE;
		}

		else
		{
			// find out how many bytes to malloc for the DeviceName.
			SetupDiGetInterfaceDeviceDetail( m_hwDeviceInfo, DeviceInterfaceData, NULL, 0, &dwNameLength, NULL );

			// Get the device name
			if( !SetupDiGetInterfaceDeviceDetail( m_hwDeviceInfo, DeviceInterfaceData, DeviceDetailInfoData, 
													dwNameLength, NULL, NULL ) )
			{
				ErrorReport("Device information reallocation failed!");

				return FALSE;
			}

			if(ParsePath((LPSTR)DeviceDetailInfoData->DevicePath))
				return TRUE;
		}
	}

	return FALSE;
}


BOOL HidUsb::OpenDevice(void)
{
	NTSTATUS stats = 0;

	DWORD errcode = 0xffUL;

	m_hwDeviceInfo = SetupDiGetClassDevs(&m_hidGUID, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

	if(m_hwDeviceInfo == INVALID_HANDLE_VALUE)
	{
		ErrorReport("Failed to open device handle!");

		return FALSE;
	}

	SP_INTERFACE_DEVICE_DATA ifData;

	SP_DEVINFO_DATA infoData;

	ifData.cbSize = sizeof(ifData);

	infoData.cbSize = sizeof(infoData);

	PSP_INTERFACE_DEVICE_DETAIL_DATA pDeviceInfoData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)operator new(MAX_PATH);

	if(!pDeviceInfoData)
	{
		ErrorReport("Device information allacation failed!");

		SetupDiDestroyDeviceInfoList( m_hwDeviceInfo );

		delete pDeviceInfoData;

		return FALSE;
	}

	pDeviceInfoData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);

	// Enumerate devices.

	// Here, we allow all the HID devices attached to the PC to work. So we don't specify a particular device via "infoData"
	if( !EnumerateDevices(NULL/*&infoData*/, &ifData, pDeviceInfoData) )
	{
		ErrorReport("Cannot find a device during enumeration!");

		SetupDiDestroyDeviceInfoList( m_hwDeviceInfo );

		delete pDeviceInfoData;

		return FALSE;
	}

	// Create OVERLAPPED

	m_pReadOverlapped = new OVERLAPPED;

	if(m_pReadOverlapped == NULL)
		return FALSE;

	m_pWriteOverlapped = new OVERLAPPED;

	if(m_pWriteOverlapped == NULL)
		return FALSE;

	memset(m_pReadOverlapped, 0, sizeof(OVERLAPPED));

	memset(m_pWriteOverlapped, 0, sizeof(OVERLAPPED));

	DWORD dwAttr;		// flags and attributes for file

	if(eCommType == USB_COMM_ASYNC)
	{
		m_pReadOverlapped->hEvent = CreateEvent(	NULL,    // no security attribute
													TRUE,    // manual-reset event
													TRUE,    // initial state = signaled
													NULL
													);   // unnamed event object

		m_pWriteOverlapped->hEvent = CreateEvent(	NULL,
													TRUE,
													TRUE,
													NULL
													);


		dwAttr = FILE_FLAG_OVERLAPPED;
	}
	else
		dwAttr = FILE_ATTRIBUTE_NORMAL;

	char szReadName[MAX_PATH], szWriteName[MAX_PATH];

	strncpy( szReadName, pDeviceInfoData->DevicePath, sizeof(szReadName) );
	strncpy( szWriteName, pDeviceInfoData->DevicePath, sizeof(szWriteName) );

	strcat (szReadName, "\\" );
	strcat (szWriteName, "\\" );

	strcat(szReadName, "PIPE_00");
	strcat(szWriteName, "PIPE_01");

	delete pDeviceInfoData;

	m_hRead = CreateFile(	szReadName,
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL,
							OPEN_EXISTING,
							dwAttr,
							NULL
							);

	m_hWrite = CreateFile(	szWriteName,
							GENERIC_READ | GENERIC_WRITE,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							NULL,
							OPEN_EXISTING,
							dwAttr,
							NULL
							);

	if(m_hRead == INVALID_HANDLE_VALUE || m_hWrite == INVALID_HANDLE_VALUE)
	{
		ErrorReport("Port open failed!");

		return FALSE;
	}

	PHIDP_PREPARSED_DATA preparsed_data;
	HIDP_CAPS hidPCaps;

	// Prepare preparsed data for read
	if(!HidD_GetPreparsedData(m_hRead, &preparsed_data))
	{
		ErrorReport("Failed to get pre-parsed data for read!");

		return FALSE;
	}

	if(HidP_GetCaps(preparsed_data, &hidPCaps) != HIDP_STATUS_SUCCESS)
	{
		ErrorReport("Failed to get capabilities for read!");

		return FALSE;
	}

	m_nBytesToRead = hidPCaps.InputReportByteLength;

	if(!HidD_FreePreparsedData(preparsed_data))
	{
		ErrorReport("Failed to release pre-parsed data resource!");

		return FALSE;
	}


	// Prepare preparsed data for write
	if(!HidD_GetPreparsedData(m_hWrite, &preparsed_data))
	{
		ErrorReport("Failed to get pre-parsed data for write!");

		return FALSE;
	}

	if(HidP_GetCaps(preparsed_data, &hidPCaps) != HIDP_STATUS_SUCCESS)
	{
		ErrorReport("Failed to get capabilities for write!");

		return FALSE;
	}

	m_nBytesToWrite = hidPCaps.OutputReportByteLength;

	// Create output report

	HIDP_VALUE_CAPS valueCaps;			// special value capabilities including ReportID

    if(!HidP_GetValueCaps(HidP_Output, &valueCaps, &hidPCaps.NumberOutputValueCaps, preparsed_data))
	{
		ErrorReport("Failed to get special value capabilities!");

		return FALSE;
	}

	if(!HidD_FreePreparsedData(preparsed_data))
	{
		ErrorReport("Failed to release pre-parsed data resource!");

		return FALSE;
	}

	return TRUE;
}



BOOL HidUsb::CloseDevice(void)
{
	BOOL bRes = TRUE;

	if(m_hwDeviceInfo)
		SetupDiDestroyDeviceInfoList(m_hwDeviceInfo);

	if(m_hRead)
	{
		bRes &= CloseHandle(m_hRead);

		m_hRead = NULL;
	}

	if(m_hWrite)
	{
		bRes &= CloseHandle(m_hWrite);

		m_hWrite = NULL;
	}

	if(!bRes)
	{
		ErrorReport("Error occurred during closing handles");

		return FALSE;
	}
	else
		return TRUE;
}


BOOL HidUsb::WritePipe(LPVOID lpBuffer)
{
	BOOL bStatus = TRUE;

	DWORD nNumHasWritten = 0UL;

	DWORD errCode;

	// asynchronous I/O handler
	if(eCommType == USB_COMM_ASYNC)
	{
		// Write USB port
		WriteFile(m_hWrite, lpBuffer, m_nBytesToWrite, &nNumHasWritten, m_pWriteOverlapped);

		// wait for the event
		if(WaitForSingleObject(m_pWriteOverlapped->hEvent, MAX_IO_WAIT) != WAIT_OBJECT_0)
			bStatus = FALSE;

		// get the result of the operation
		if(!GetOverlappedResult(m_hWrite, m_pWriteOverlapped, &nNumHasWritten, FALSE))
			bStatus = FALSE;

		errCode = GetLastError();

		// If failed in operation, cancel all I/O for this handle in this thread
		if(!bStatus)
			CancelIo(m_hWrite);

		// make sure we have written all the data we wanted to
		if ((nNumHasWritten > 0) && (nNumHasWritten != m_nBytesToWrite))
			bStatus = FALSE;

		return bStatus;
	}
	else			// synchronous I/O handler
	{
		if(!WriteFile(m_hWrite,lpBuffer,m_nBytesToWrite,&nNumHasWritten,NULL))
		{
			errCode = GetLastError();

			return FALSE;
		}

		if(m_nBytesToWrite != nNumHasWritten)
			return FALSE;

		return TRUE;
	}

	return TRUE;
}


BOOL HidUsb::ReadPipe(LPVOID lpBuffer)
{
	BOOL bStatus = TRUE;

	DWORD nNumHasRead = 0UL;

	DWORD errCode;

	// asynchronous I/O handler
	if(eCommType == USB_COMM_ASYNC)
	{
		// Read USB port
		ReadFile(m_hRead, lpBuffer, m_nBytesToRead, &nNumHasRead, m_pReadOverlapped);

		// wait for the event
		if(WaitForSingleObject(m_pReadOverlapped->hEvent, MAX_IO_WAIT) != WAIT_OBJECT_0)
			bStatus = FALSE;

		// Get the result of the operation
		if (!GetOverlappedResult(m_hRead, m_pReadOverlapped, &nNumHasRead, FALSE))
			bStatus = FALSE;

		// If failed in operation, cancel all I/O for this handle in this thread
		if(!bStatus)
			CancelIo(m_hRead);

		// make sure we have read all the data we wanted to
		if ((nNumHasRead > 0) && (nNumHasRead != m_nBytesToRead))
			bStatus = FALSE;

		return bStatus;
	}
	else		// synchronous I/O handler
	{
		if(!ReadFile(m_hRead, lpBuffer, m_nBytesToRead, &nNumHasRead, NULL))
		{
			errCode = GetLastError();

			return FALSE;
		}

		if(m_nBytesToRead != nNumHasRead)
			return FALSE;

		return TRUE;
	}
}

