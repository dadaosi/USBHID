#ifndef COMMAND_MANAGER_HEADER

#define COMMAND_MANAGER_HEADER

#include "..\usbdriver\hidusb.h"


#define MSG_COUNT	( 7 * 6 + 5 )		// [column_code		row_code] * [	6

										//									1	]



class ComManager
{
private:

	HidUsb *m_pHUsb;

	typedef void(*COMMAND_MSG)(void*);

	static COMMAND_MSG cmdList[MSG_COUNT];

public:

	ComManager(void);

	~ComManager(void);

	BOOL CmdInit(void);

	BOOL ReceiveCmdAndSendRespond(void);

	BOOL EndCmdTransfer(void);

	BOOL GetKey(unsigned char *keyValue);

	BOOL OutputDevice(unsigned char inputValue);

private:

	class COMMAND_CALLBACK
	{
	public:

		static void dummy(void*);

	};
};


#endif

