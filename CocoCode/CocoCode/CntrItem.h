
// CntrItem.h : CCocoCodeCntrItem ��Ľӿ�
//

#pragma once

class CCocoCodeDoc;
class CCocoCodeView;

class CCocoCodeCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CCocoCodeCntrItem)

// ���캯��
public:
	CCocoCodeCntrItem(REOBJECT* preo = NULL, CCocoCodeDoc* pContainer = NULL);
		// ע��: ���� pContainer Ϊ NULL ������ IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE Ҫ������д���
		//  �����Ĺ��캯����OLE ��ͨ������
		//  �� NULL �ĵ�ָ�빹���

// ����
public:
	CCocoCodeDoc* GetDocument()
		{ return reinterpret_cast<CCocoCodeDoc*>(CRichEditCntrItem::GetDocument()); }
	CCocoCodeView* GetActiveView()
		{ return reinterpret_cast<CCocoCodeView*>(CRichEditCntrItem::GetActiveView()); }

// ʵ��
public:
	~CCocoCodeCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

