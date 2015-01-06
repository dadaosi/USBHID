// HIDUSBView.h : CHIDUSBView 类的接口
//


#pragma once

class ComManager;

class CHIDUSBView : public CView
{
protected: // 仅从序列化创建
	CHIDUSBView();
	DECLARE_DYNCREATE(CHIDUSBView)

// 属性
public:
	CHIDUSBDoc* GetDocument() const;

private:

	CWinThread *m_pThread;

	ComManager *m_pComMgr;

	bool m_bHasRun;

// 操作
private:

	static UINT USBCommunicate(LPVOID lpArg);

public:

// 重写
	public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CHIDUSBView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRun();
	afx_msg void OnStop();
	afx_msg void OnKeyValueTest();
	afx_msg void OnDeviceOutputTest();
};



#ifndef _DEBUG  // HIDUSBView.cpp 的调试版本
inline CHIDUSBDoc* CHIDUSBView::GetDocument() const
   { return reinterpret_cast<CHIDUSBDoc*>(m_pDocument); }
#endif

