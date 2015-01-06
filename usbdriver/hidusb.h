#ifndef HID_USB_DRIVER_HEADER

#define HID_USB_DRIVER_HEADER


extern "C"
{
	//#include "..\ddkinc\hidclass.h"

	#include "..\ddkinc\hidsdi.h"

	#include "..\ddkinc\setupapi.h"
}


enum USB_COMM_TYPE
{
	USB_COMM_SYNC,		// synchronous communication

	USB_COMM_ASYNC		// asynchronous communication
};


// Users can get PID nad VID via DDK\WMD\USB\usbview.exe program

#define DEVICE_VID			"1241"

// NB: Productor ID and Vender ID shuold be lower case!
#define DEVICE_PID			"e001"

#define USB_COMM_TYPEDEF	USB_COMM_SYNC

#define MAX_IO_WAIT		3000		// timeout for USB transfers in msecs


class HidUsb
{
private:

	GUID m_hidGUID;				// HID device GUID

	HANDLE	m_hRead, m_hWrite;	// read & write handle to USB IO object.

	HDEVINFO m_hwDeviceInfo;	// handle to the hardware device information

	LPOVERLAPPED m_pReadOverlapped, m_pWriteOverlapped;		// used in asynchronous I/O

	const enum USB_COMM_TYPE eCommType;	// use synchronous or asynchronous communication

	DWORD m_nBytesToRead, m_nBytesToWrite;	// the number of bytes to read or write

	const char *deviceVID, *devicePID;

public:

	HidUsb(void);

	HidUsb(const char* vid, const char* pid);

	~HidUsb(void);

	BOOL OpenDevice(void);

	BOOL ReadPipe(LPVOID lpBuffer);

	BOOL WritePipe(LPVOID lpBuffer);

	BOOL CloseDevice(void);

	inline DWORD GetBytesToRead(void)const;

	inline DWORD GetBytesToWrite(void)const;

private:

	BOOL EnumerateDevices(PSP_DEVINFO_DATA  DeviceInfoData, PSP_DEVICE_INTERFACE_DATA  DeviceInterfaceData,
							PSP_INTERFACE_DEVICE_DETAIL_DATA DeviceDetailInfoData);

	BOOL ParsePath(LPCSTR pathname);

	static void ErrorReport(CString info);	// This method can be configured according to the specified condition
};


inline DWORD HidUsb::GetBytesToRead(void) const
{
	return m_nBytesToRead;
}

inline DWORD HidUsb::GetBytesToWrite(void) const
{
	return m_nBytesToWrite;
}


#endif

