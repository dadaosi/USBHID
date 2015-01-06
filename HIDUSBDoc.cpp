// HIDUSBDoc.cpp :  CHIDUSBDoc 类的实现
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


// CHIDUSBDoc 构造/析构

CHIDUSBDoc::CHIDUSBDoc()
{
	// TODO: 在此添加一次性构造代码

}

CHIDUSBDoc::~CHIDUSBDoc()
{
}

BOOL CHIDUSBDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CHIDUSBDoc 序列化

void CHIDUSBDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CHIDUSBDoc 诊断

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


// CHIDUSBDoc 命令
