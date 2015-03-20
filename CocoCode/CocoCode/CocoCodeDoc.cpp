
// CocoCodeDoc.cpp : CCocoCodeDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CocoCode.h"
#endif

#include "CocoCodeDoc.h"
#include "CntrItem.h"

#include "CocoCodeView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CCocoCodeDoc

IMPLEMENT_DYNCREATE(CCocoCodeDoc, CRichEditDoc)

BEGIN_MESSAGE_MAP(CCocoCodeDoc, CRichEditDoc)
	// 启用默认的 OLE 容器实现
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()


// CCocoCodeDoc 构造/析构

CCocoCodeDoc::CCocoCodeDoc()
{
	// TODO: 在此添加一次性构造代码

}

CCocoCodeDoc::~CCocoCodeDoc()
{
}

BOOL CCocoCodeDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档

	return TRUE;
}

CRichEditCntrItem* CCocoCodeDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CCocoCodeCntrItem(preo, const_cast<CCocoCodeDoc*>(this));
}




// CCocoCodeDoc 序列化

void CCocoCodeDoc::Serialize(CArchive& ar)
{
	CCocoCodeView* pView = (CCocoCodeView*)GetView();
	AFXASSUME(pView);

        CString str, line;

        if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
                pView->GetWindowText(str);
                ar.WriteString(str);
	}
	else
	{
		// TODO: 在此添加加载代码
                str = L"";
                while (ar.ReadString(line) != FALSE)
                {
                        str += line + L"\n";
                }
                
                pView->SetWindowText(str);
	}

	// 调用基类 CRichEditDoc 将启用
	//  容器文档的 COleClientItem 对象的序列化。
	// TODO: 如果作为文本进行序列化，则设置 CRichEditDoc::m_bRTF = FALSE
//	CRichEditDoc::Serialize(ar);
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CCocoCodeDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CCocoCodeDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CCocoCodeDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CCocoCodeDoc 诊断

#ifdef _DEBUG
void CCocoCodeDoc::AssertValid() const
{
	CRichEditDoc::AssertValid();
}

void CCocoCodeDoc::Dump(CDumpContext& dc) const
{
	CRichEditDoc::Dump(dc);
}
#endif //_DEBUG


// CCocoCodeDoc 命令

CRichEditView* CCocoCodeDoc::GetView(void) const
{
	//find the first view - if there are no views
	//we must return NULL
	POSITION pos = GetFirstViewPosition();
	if (pos == NULL)
		return NULL;

	//find the first view that is a CScintillaView
	CView* pView;
	while (pos != NULL)
	{
		pView = GetNextView(pos);
		if (pView->IsKindOf(RUNTIME_CLASS(CCocoCodeView)))
			return static_cast<CCocoCodeView*>(pView);
	}

	//can't find one--return NULL
	return NULL;
}