#pragma once
#include "afxwin.h"

#include "./usbdriver/hidusb.h"

// CKeyReceptDlg 对话框

class CKeyReceptDlg : public CDialog
{
	DECLARE_DYNAMIC(CKeyReceptDlg)

public:
	CKeyReceptDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CKeyReceptDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	HidUsb* hidusb;
	CListBox m_list;
	bool isString;

	void WStringToString(char *dest, const CString &src);
	void ByteToAscii(char *dest, char c);
	bool ParseWString(unsigned char *dest, const CString &s);
	inline char HexToChar(unsigned char c);
	inline unsigned char CharToHex(char c);

	afx_msg void OnBnClickedButtonBeginReceive();
	afx_msg void OnBnClickedButtonStopReceive();
	afx_msg void OnBnClickedButtonClearList();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	afx_msg void OnBnClickedButtonReceive();

	afx_msg void OnBnClickedButtonSend();
	CString m_contentEdit;

	afx_msg void OnBnClickedRadioString();

	afx_msg void OnBnClickedRadioAscii();
	CStatic m_hintStatic;
	CString m_vidEdit;
	CString m_pidEdit;
	virtual BOOL OnInitDialog();
};

inline char CKeyReceptDlg::HexToChar(unsigned char c)
{
	c &= 0x0f;
	if(c < 10)
		return (char)(c + '0');
	else
		return (char)(c - 10 + 'A');
}

inline unsigned char CKeyReceptDlg::CharToHex(char c)
{
	register unsigned char uc = (unsigned char)c;

	if(uc >= '0' && uc <= '9')
		uc -= '0';

	else if(uc >= 'a' && uc <= 'f')
		uc = (uc - 'a') + 10;
	else if(uc >= 'A' && uc <= 'Z')
		uc = (uc - 'A') + 10;
	else
		uc = 0xff;
	return uc;
}
