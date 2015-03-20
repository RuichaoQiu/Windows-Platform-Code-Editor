
// CocoCodeView.h : CCocoCodeView ��Ľӿ�
//

#pragma once

#include "PopListCtrl.h"
#include "Compiler.h"

#include <string>
#include <vector>

using namespace std;

class CCocoCodeCntrItem;

struct CPaintStruct
{
        long left, righ;
        COLORREF color;

        CPaintStruct(long _left, long _righ, COLORREF _color)
                : left(_left), righ(_righ), color(_color) {}
};

class CCocoCodeView : public CRichEditView
{
protected: // �������л�����
	CCocoCodeView();
	DECLARE_DYNCREATE(CCocoCodeView)

// ����
public:
	CCocoCodeDoc* GetDocument() const;

// ����
public:

// ����
public:
        const static COLORREF color_text = RGB(0, 0, 0);
        const static COLORREF color_keys = RGB(65, 86, 122);
        const static COLORREF color_comm = RGB(0, 128, 0);
        const static COLORREF color_stri = RGB(163, 21, 21);

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // ������һ�ε���
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CCocoCodeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnDestroy();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
        CRichEditCtrl *ed;
        CHARFORMAT2 default_char_format;
        wstring identi_char, symbol_char;
        wstring keys, macros;

        wstring window_text;

public:
        void SetWordColor(long offset, COLORREF color);
private:
        void GetWordRange(long offset, long & left, long & righ);

        vector<CPaintStruct> paint;
        bool update_from_scratch;
        
public:
		Compiler MyCompiler;
		TCHAR s_szFileName[1024];
		TCHAR s_szTitleName[1024];
        // �õ����㿪ʼ��pos���ַ�
        wchar_t GetCharAt(long pos, wchar_t & ch);
        // �õ���ָ��������λ���ϵĵ��ʵ���ɫ
        void GetWordColor(long offset, COLORREF & color);
        void SetSelColor(COLORREF color);
        void GetSelColor(COLORREF & color);
        void GetRangeColor(long left, long righ, COLORREF & color);
        void SetRangeColor(long left, long righ, COLORREF color);
        void ReplaceText(long left, long righ, LPCTSTR text);
        void ReplaceWord(long offset, LPCTSTR word);
        long GetLineIndent(long index, long & indent);
        void SetLineIndent(long index, long indent);
        void GetTextWstring(wstring & text);
        void SetTextWstring(wstring text);
        afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
        void GetWordWstring(long offset, wstring & word);
        long GetCurrentLine(long & index);
        void HilightWord(long offset, bool hilight_anyway);
        void AutoIndent(long line);
        void IndentPlus(long line);
        bool NotInComment(void);
        void UpdateFromScratch(void);
        void CheckComment(long start);
        afx_msg void OnTimer(UINT_PTR nIDEvent);
        void SetRangeColor(const CPaintStruct & st);

        CPopListCtrl rCtrl;
        bool isCompleting;
        bool ContinueOnChar(UINT key);
        CPoint GetCurCharPos(void);
        afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        void StartCompleting(void);
        afx_msg void OnStartcompletion();
		afx_msg void Oncompiler();
};

#ifndef _DEBUG  // CocoCodeView.cpp �еĵ��԰汾
inline CCocoCodeDoc* CCocoCodeView::GetDocument() const
   { return reinterpret_cast<CCocoCodeDoc*>(m_pDocument); }
#endif

