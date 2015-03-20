
// CntrItem.cpp : CCocoCodeCntrItem 类的实现
//

#include "stdafx.h"
#include "CocoCode.h"

#include "CocoCodeDoc.h"
#include "CocoCodeView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCocoCodeCntrItem 的实现

IMPLEMENT_SERIAL(CCocoCodeCntrItem, CRichEditCntrItem, 0)

CCocoCodeCntrItem::CCocoCodeCntrItem(REOBJECT* preo, CCocoCodeDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: 在此添加一次性构造代码
}

CCocoCodeCntrItem::~CCocoCodeCntrItem()
{
	// TODO: 在此处添加清理代码
}


// CCocoCodeCntrItem 诊断

#ifdef _DEBUG
void CCocoCodeCntrItem::AssertValid() const
{
	CRichEditCntrItem::AssertValid();
}

void CCocoCodeCntrItem::Dump(CDumpContext& dc) const
{
	CRichEditCntrItem::Dump(dc);
}
#endif

