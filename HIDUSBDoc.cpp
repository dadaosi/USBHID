// HIDUSBDoc.cpp :  CHIDUSBDoc ���ʵ��
//

#include "stdafx.h"
#include "HIDUSB.h"

#include "HIDUSBDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHIDUSBDoc

IMPLEMENT_DYNCREATE(CHIDUSBDoc, CDocument)

BEGIN_MESSAGE_MAP(CHIDUSBDoc, CDocument)
END_MESSAGE_MAP()


// CHIDUSBDoc ����/����

CHIDUSBDoc::CHIDUSBDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CHIDUSBDoc::~CHIDUSBDoc()
{
}

BOOL CHIDUSBDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CHIDUSBDoc ���л�

void CHIDUSBDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CHIDUSBDoc ���

#ifdef _DEBUG
void CHIDUSBDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHIDUSBDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CHIDUSBDoc ����
