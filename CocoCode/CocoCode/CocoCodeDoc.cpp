
// CocoCodeDoc.cpp : CCocoCodeDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ����Ĭ�ϵ� OLE ����ʵ��
	ON_UPDATE_COMMAND_UI(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnUpdateEditLinksMenu)
	ON_COMMAND(ID_OLE_EDIT_LINKS, &CRichEditDoc::OnEditLinks)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OLE_VERB_FIRST, ID_OLE_VERB_LAST, &CRichEditDoc::OnUpdateObjectVerbMenu)
END_MESSAGE_MAP()


// CCocoCodeDoc ����/����

CCocoCodeDoc::CCocoCodeDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CCocoCodeDoc::~CCocoCodeDoc()
{
}

BOOL CCocoCodeDoc::OnNewDocument()
{
	if (!CRichEditDoc::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�

	return TRUE;
}

CRichEditCntrItem* CCocoCodeDoc::CreateClientItem(REOBJECT* preo) const
{
	return new CCocoCodeCntrItem(preo, const_cast<CCocoCodeDoc*>(this));
}




// CCocoCodeDoc ���л�

void CCocoCodeDoc::Serialize(CArchive& ar)
{
	CCocoCodeView* pView = (CCocoCodeView*)GetView();
	AFXASSUME(pView);

        CString str, line;

        if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
                pView->GetWindowText(str);
                ar.WriteString(str);
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
                str = L"";
                while (ar.ReadString(line) != FALSE)
                {
                        str += line + L"\n";
                }
                
                pView->SetWindowText(str);
	}

	// ���û��� CRichEditDoc ������
	//  �����ĵ��� COleClientItem ��������л���
	// TODO: �����Ϊ�ı��������л��������� CRichEditDoc::m_bRTF = FALSE
//	CRichEditDoc::Serialize(ar);
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CCocoCodeDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CCocoCodeDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CCocoCodeDoc ���

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


// CCocoCodeDoc ����

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