// HIDUSBView.cpp : CHIDUSBView 类的实现
//

#include "stdafx.h"
#include "HIDUSB.h"

#include "HIDUSBDoc.h"
#include "HIDUSBView.h"
#include ".\hidusbview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "commandmgr\commgr.h"


// CHIDUSBView

IMPLEMENT_DYNCREATE(CHIDUSBView, CView)

BEGIN_MESSAGE_MAP(CHIDUSBView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_RUN, OnRun)
	ON_COMMAND(ID_STOP, OnStop)
	ON_COMMAND(ID_KEY_VALUE_TEST, OnKeyValueTest)
	ON_COMMAND(ID_DEVICE_OUTPUT_TEST, OnDeviceOutputTest)
END_MESSAGE_MAP()

// CHIDUSBView 构造/析构

CHIDUSBView::CHIDUSBView() : m_pThread(NULL), m_pComMgr(NULL), m_bHasRun(false)
{
	// TODO: 在此处添加构造代码

}

CHIDUSBView::~CHIDUSBView()
{
	if(m_pComMgr != NULL)
	{
		delete m_pComMgr;

		m_pComMgr = NULL;
	}
}




BOOL CHIDUSBView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或
	// 样式

	return CView::PreCreateWindow(cs);
}


// CHIDUSBView 绘制

void CHIDUSBView::OnDraw(CDC* /*pDC*/)
{
	CHIDUSBDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CHIDUSBView 打印

BOOL CHIDUSBView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CHIDUSBView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印前添加额外的初始化
}

void CHIDUSBView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 打印后添加清除过程
}


// CHIDUSBView 诊断

#ifdef _DEBUG
void CHIDUSBView::AssertValid() const
{
	CView::AssertValid();
}

void CHIDUSBView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHIDUSBDoc* CHIDUSBView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHIDUSBDoc)));
	return (CHIDUSBDoc*)m_pDocument;
}
#endif //_DEBUG



#define TEST_BEGIN_CHECK()			if(m_pComMgr != NULL){		\
		::MessageBox(NULL, "The program has already been running!", "Attention", MB_OK);	\
		m_bHasRun = false;				\
		return;		\
	}				\
	m_pComMgr = new ComManager();		\
	if(m_pComMgr == NULL)	{			\
		::MessageBox(NULL, "Error occurred during initialization: lack of memory!", "Fatal Error", MB_OK);	\
		m_bHasRun = false;				\
		return;		\
	}				\
	if(!m_pComMgr->CmdInit())	{		\
		delete m_pComMgr;				\
		m_pComMgr = NULL;				\
		m_bHasRun = false;				\
		return;							\
	}


// CHIDUSBView 消息处理程序

void CHIDUSBView::OnRun()
{
	// TODO: 在此添加命令处理程序代码
	if(m_bHasRun)
	{
		::MessageBox(NULL, "The programming has been running!", "Warning", MB_OK);

		return;
	}

	m_bHasRun = true;

	TEST_BEGIN_CHECK();

	::MessageBox(NULL, "Program starts!", "Attention", MB_OK);

	m_pThread = AfxBeginThread(&USBCommunicate, this);
}



UINT CHIDUSBView::USBCommunicate(LPVOID lpArg)
{
	CHIDUSBView *pView = (CHIDUSBView*)lpArg;

	ComManager *pComMgr = pView->m_pComMgr;

	while(TRUE)
	{
		if(!pComMgr->ReceiveCmdAndSendRespond())
		{
			::MessageBox(NULL, "The program exits abnormally", "Warning", MB_OK);

			AfxEndThread(pView->m_pThread->m_nThreadID, TRUE);
		}
		else
			Sleep(10);
	}

	return TRUE;
}



void CHIDUSBView::OnStop()
{
	// TODO: 在此添加命令处理程序代码
	if(m_pThread)
	{
		::TerminateThread(m_pThread->m_hThread, m_pThread->m_nThreadID);

		m_pThread->Delete();

		m_pThread = NULL;
	}

	if(m_pComMgr)
	{
		if(!m_pComMgr->EndCmdTransfer())
		{
			::MessageBox(NULL, "The USB communication cannot be terminated now! Please try again later!", "Warning", MB_OK);

			return;
		}

		delete m_pComMgr;

		m_pComMgr = NULL;
	}

	m_bHasRun = false;

	::MessageBox(NULL, "This program stops!", "Attendtion", MB_OK);
}


#include "KeyReceptDlg.h"


void CHIDUSBView::OnKeyValueTest()
{
	// TODO: 在此添加命令处理程序代码
	if(m_bHasRun)
	{
		::MessageBox(NULL, "The programming has been running!", "Warning", MB_OK);

		return;
	}

	m_bHasRun = true;

	CKeyReceptDlg dlg;

	dlg.DoModal();

	m_bHasRun = false;
}



void CHIDUSBView::OnDeviceOutputTest()
{
	// TODO: 在此添加命令处理程序代码
	if(m_bHasRun)
	{
		::MessageBox(NULL, "The programming has been running!", "Warning", MB_OK);

		return;
	}

	m_bHasRun = true;

	// Add dlg.DoModal() here

	m_bHasRun = false;
}
