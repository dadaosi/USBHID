// HIDUSBDoc.h :  CHIDUSBDoc ��Ľӿ�
//


#pragma once

class CHIDUSBDoc : public CDocument
{
protected: // �������л�����
	CHIDUSBDoc();
	DECLARE_DYNCREATE(CHIDUSBDoc)

// ����
public:

// ����
public:

// ��д
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CHIDUSBDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


