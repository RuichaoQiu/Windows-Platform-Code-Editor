
// CntrItem.cpp : CCocoCodeCntrItem ���ʵ��
//

#include "stdafx.h"
#include "CocoCode.h"

#include "CocoCodeDoc.h"
#include "CocoCodeView.h"
#include "CntrItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCocoCodeCntrItem ��ʵ��

IMPLEMENT_SERIAL(CCocoCodeCntrItem, CRichEditCntrItem, 0)

CCocoCodeCntrItem::CCocoCodeCntrItem(REOBJECT* preo, CCocoCodeDoc* pContainer)
	: CRichEditCntrItem(preo, pContainer)
{
	// TODO: �ڴ����һ���Թ������
}

CCocoCodeCntrItem::~CCocoCodeCntrItem()
{
	// TODO: �ڴ˴�����������
}


// CCocoCodeCntrItem ���

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

