
// CntrItem.h : CCocoCodeCntrItem 类的接口
//

#pragma once

class CCocoCodeDoc;
class CCocoCodeView;

class CCocoCodeCntrItem : public CRichEditCntrItem
{
	DECLARE_SERIAL(CCocoCodeCntrItem)

// 构造函数
public:
	CCocoCodeCntrItem(REOBJECT* preo = NULL, CCocoCodeDoc* pContainer = NULL);
		// 注意: 允许 pContainer 为 NULL 以启用 IMPLEMENT_SERIALIZE
		//  IMPLEMENT_SERIALIZE 要求类具有带零
		//  参数的构造函数。OLE 项通常是用
		//  非 NULL 文档指针构造的

// 特性
public:
	CCocoCodeDoc* GetDocument()
		{ return reinterpret_cast<CCocoCodeDoc*>(CRichEditCntrItem::GetDocument()); }
	CCocoCodeView* GetActiveView()
		{ return reinterpret_cast<CCocoCodeView*>(CRichEditCntrItem::GetActiveView()); }

// 实现
public:
	~CCocoCodeCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

