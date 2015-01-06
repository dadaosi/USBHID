#include "stdafx.h"

#include "commgr.h"



ComManager::ComManager(void) : m_pHUsb(NULL)
{

}


ComManager::~ComManager(void)
{
	if(m_pHUsb != NULL)
	{
		delete m_pHUsb;

		m_pHUsb = NULL;
	}
}


BOOL ComManager::CmdInit(void)
{
	if(m_pHUsb == NULL)
	{
		m_pHUsb = new HidUsb();

		if(m_pHUsb == NULL)
		{
			::MessageBox(NULL, "Lack of memory!", "Error", MB_OK);

			return FALSE;
		}

		if(!m_pHUsb->OpenDevice())
		{
			::MessageBox(NULL, "Failed to open the device!", "warning", MB_OK);

			return FALSE;
		}
	}

	return TRUE;
}


BOOL ComManager::EndCmdTransfer(void)
{
	return m_pHUsb->CloseDevice();
}


BOOL ComManager::ReceiveCmdAndSendRespond(void)
{
	unsigned char keyValue[10] = { 0x0, 0x0, 0x0 };

	while(!m_pHUsb->ReadPipe(keyValue))
		Sleep(15);

	if(keyValue[0] != 0)
	{
		__asm nop;

		keyValue[0] = 0;
	}

	while(!m_pHUsb->WritePipe(keyValue))
		Sleep(15);

//	(*cmdList[keyValue])(this);

	return TRUE;
}


BOOL ComManager::GetKey(unsigned char* keyValue)
{
	return m_pHUsb->ReadPipe(keyValue);
}


BOOL ComManager::OutputDevice(unsigned char inputValue)
{
	(*cmdList[inputValue])(m_pHUsb);

	return TRUE;
}


ComManager::COMMAND_MSG ComManager::cmdList[MSG_COUNT] = 
{
	&ComManager::COMMAND_CALLBACK::dummy
};

