// HIDUSBView.h : CHIDUSBView ��Ľӿ�
//


#pragma once

class ComManager;

class CHIDUSBView : public CView
{
protected: // �������л�����
	CHIDUSBView();
	DECLARE_DYNCREATE(CHIDUSBView)

// ����
public:
	CHIDUSBDoc* GetDocument() const;

private:

	CWinThread *m_pThread;

	ComManager *m_pComMgr;

	bool m_bHasRun;

// ����
private:

	static UINT USBCommunicate(LPVOID lpArg);

public:

// ��д
	public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CHIDUSBView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRun();
	afx_msg void OnStop();
	afx_msg void OnKeyValueTest();
	afx_msg void OnDeviceOutputTest();
};



#ifndef _DEBUG  // HIDUSBView.cpp �ĵ��԰汾
inline CHIDUSBDoc* CHIDUSBView::GetDocument() const
   { return reinterpret_cast<CHIDUSBDoc*>(m_pDocument); }
#endif

