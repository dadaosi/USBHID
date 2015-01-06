// KeyReceptDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HIDUSB.h"
#include "KeyReceptDlg.h"
#include ".\keyreceptdlg.h"


// CKeyReceptDlg �Ի���

IMPLEMENT_DYNAMIC(CKeyReceptDlg, CDialog)
CKeyReceptDlg::CKeyReceptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyReceptDlg::IDD, pParent)
	, m_contentEdit(_T(""))
	, m_vidEdit(_T(""))
	, m_pidEdit(_T(""))
{
	hidusb = NULL;
	isString = true;
}

CKeyReceptDlg::~CKeyReceptDlg()
{
	if(hidusb != NULL)
	{
		hidusb->CloseDevice();
		delete hidusb;
		hidusb = NULL;
		MessageBox("�豸�ѹر�");
	}
}

void CKeyReceptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_KEY_RECEPT, m_list);
	DDX_Text(pDX, IDC_EDIT_CONTENT_TO_SEND, m_contentEdit);
	DDV_MaxChars(pDX, m_contentEdit, 99);
	DDX_Control(pDX, IDC_STATIC_ASCII_HINT, m_hintStatic);
	DDX_Text(pDX, IDC_EDIT_VID, m_vidEdit);
	DDX_Text(pDX, IDC_EDIT_PID, m_pidEdit);
}


BEGIN_MESSAGE_MAP(CKeyReceptDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_BEGIN_RECEIVE, OnBnClickedButtonBeginReceive)
	ON_BN_CLICKED(IDC_BUTTON_STOP_RECEIVE, OnBnClickedButtonStopReceive)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_LIST, OnBnClickedButtonClearList)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_RECEIVE, &CKeyReceptDlg::OnBnClickedButtonReceive)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CKeyReceptDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_RADIO_STRING, &CKeyReceptDlg::OnBnClickedRadioString)
	ON_BN_CLICKED(IDC_RADIO_ASCII, &CKeyReceptDlg::OnBnClickedRadioAscii)
END_MESSAGE_MAP()



void CKeyReceptDlg::WStringToString(char *dest, const CString &src)
{
	register int n = src.GetLength();

	for(register int i=0; i<n; i++)
		dest[i] = (unsigned char)src.GetAt(i);
	dest[n] = '\0';
}

void CKeyReceptDlg::ByteToAscii(char *dest, char c)
{
	register unsigned char a = c & 0xf0;
	dest[0] = HexToChar(a >> 4);
	a = c & 0xf;
	dest[1] = HexToChar(a);
}

bool CKeyReceptDlg::ParseWString(unsigned char *dest, const CString &s)
{
	register int n = s.GetLength();

	register int time = 0;

	unsigned char tmp[2];

	int j = 0;

	char t;

	for(int i=0; i<n; i++)
	{
		t = (char)s.GetAt(i);

		switch(t)
		{
		case ' ':
			if(time == 1)
				goto BAD_FORMAT;
			break;

		case ',':
			if(time == 2)
			{
				time = 0;
				dest[j++] = tmp[0] * 16 + tmp[1];
				break;
			}
			else
				goto BAD_FORMAT;

		default:
			
			tmp[time] = CharToHex(t);
			if(tmp[time] == 0xff)
				goto BAD_FORMAT;
			time++;
			break;
BAD_FORMAT:
			MessageBox("ASCII����ʽ������μ���ʾ��ʽ��");
			return false;
		}
	}

	if(time != 2)
		goto BAD_FORMAT;

	dest[j++] = tmp[0] * 16 + tmp[1];

	return true;
}




// CKeyReceptDlg ��Ϣ�������

void CKeyReceptDlg::OnBnClickedButtonBeginReceive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if(hidusb == NULL)
		return;
	register int len = hidusb->GetBytesToRead();

	char buf[128];

	if(!hidusb->ReadPipe(buf))
	{
		MessageBox("�޷���ȡ���ݣ�");
		return;
	}
	char display[768];
	char tmp[2];

	int j = 0;

	for(register int i=0; i<len; i++)
	{
		display[j++] = '0';
		display[j++] = 'x';
		ByteToAscii(tmp, buf[i]);
		display[j++] = tmp[0];
		display[j++] = tmp[1];
		display[j++] = ',';
		display[j++] = ' ';
	}
	display[j] = '\0';

	CString s = "�ѽ������ݣ� ";
	s += display;
	m_list.AddString(s);

	UpdateData(FALSE);
}

void CKeyReceptDlg::OnBnClickedButtonStopReceive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	if(hidusb == NULL)
		return;

	register int len = hidusb->GetBytesToWrite();

	char buf[768];

	int editLen = m_contentEdit.GetLength();

	if(editLen == 0)
		return;
	len = len < editLen? len : editLen;

	WStringToString(buf, m_contentEdit);

	bool result = false;

	unsigned char send[128];

	if(isString)
	{
		buf[len] = '\0';
		result = hidusb->WritePipe(buf) == TRUE;
	}
	else
	{
		if(ParseWString(send, m_contentEdit));
			result = hidusb->WritePipe(send) == TRUE;
	}

	CString s = "�ѷ������ݣ�";

	if(result)
	{
		s += buf;
		m_list.AddString(s);
	}
	else
		MessageBox("�����޷������ͣ�");

	UpdateData(FALSE);
}


void CKeyReceptDlg::OnBnClickedButtonClearList()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	register int n = m_list.GetCount();

	for(register int i=0; i<n; i++)
		m_list.DeleteString(0);

	UpdateData(FALSE);
}

void CKeyReceptDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	
	register int n = m_vidEdit.GetLength();
	if(n != 4)
	{
		MessageBox("�������豸VID������Ϊ4���ֽڣ�");
		return;
	}
	n = m_pidEdit.GetLength();
	if(n != 4)
	{
		MessageBox("�������豸PID������Ϊ4���ֽڣ�");
		return;
	}

	char bufVID[5];
	char bufPID[5];

	WStringToString(bufVID, m_vidEdit);
	WStringToString(bufPID, m_pidEdit);
	
	if(hidusb == NULL)
	{
		hidusb = new HidUsb(bufVID, bufPID);

		if(hidusb == NULL)
		{
			::MessageBox(NULL, "Lack of memory!", "Error", MB_OK);

			return;
		}

		if(!hidusb->OpenDevice())
		{
			::MessageBox(NULL, "Failed to open the device!", "warning", MB_OK);

			return;
		}

		MessageBox("�豸������");
	}

}

void CKeyReceptDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

void CKeyReceptDlg::OnBnClickedButtonReceive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

}

void CKeyReceptDlg::OnBnClickedButtonSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CKeyReceptDlg::OnBnClickedRadioString()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	isString = true;
	m_hintStatic.ShowWindow(SW_HIDE);
	UpdateData(FALSE);
}

void CKeyReceptDlg::OnBnClickedRadioAscii()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	isString = false;
	m_hintStatic.ShowWindow(SW_SHOW);
	UpdateData(FALSE);
}

BOOL CKeyReceptDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_hintStatic.ShowWindow(SW_HIDE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
