// HIDUSB.h : HIDUSB Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h�� 
#endif

#include "resource.h"       // ������


// CHIDUSBApp:
// �йش����ʵ�֣������ HIDUSB.cpp
//

class CHIDUSBApp : public CWinApp
{
public:
	CHIDUSBApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHIDUSBApp theApp;
