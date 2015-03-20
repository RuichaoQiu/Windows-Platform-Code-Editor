
// CocoCodeView.cpp : CCocoCodeView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CocoCode.h"
#endif

#include "CocoCodeDoc.h"
#include "CntrItem.h"
#include "resource.h"
#include "CocoCodeView.h"

#include "PopuoListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCocoCodeView

IMPLEMENT_DYNCREATE(CCocoCodeView, CRichEditView)

BEGIN_MESSAGE_MAP(CCocoCodeView, CRichEditView)
	ON_WM_DESTROY()
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CCocoCodeView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
        ON_WM_CHAR()
        ON_WM_TIMER()
        ON_WM_KEYDOWN()
        ON_COMMAND(IDS_STARTCOMPLETION, &CCocoCodeView::OnStartcompletion)
		ON_COMMAND(ID_compiler, &CCocoCodeView::Oncompiler)
END_MESSAGE_MAP()

// CCocoCodeView 构造/析构

_declspec(dllimport) const wchar_t * LoadKeys();
_declspec(dllimport) const wchar_t * LoadMacro();
_declspec(dllimport) const wchar_t * LoadIdChar();
_declspec(dllimport) const wchar_t * LoadSymbol();

#pragma comment(lib, "KeySet.lib")

CCocoCodeView::CCocoCodeView()
        : ed(NULL)
{
        // TODO: 在此处添加构造代码
        ed = &GetRichEditCtrl();

		identi_char = LoadIdChar();
		symbol_char = LoadSymbol();
		keys = LoadKeys();
		macros = LoadMacro();

/*        identi_char = L"_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890#";
        symbol_char = L"~!@$%^&*()+`-=[]{}\\|;:\"'<>,.?/";

        keys = L" asm bool explicit export friend inline mutable operator"
               L" template typeid virtual wchar_t delete namespace typename"
               L" using const_cast dynamic_cast reinterpret_cast static_cast"
               L" class new public private protected this try throw catch true"
               L" false unsigned signed struct enum union auto register extern"
               L" sizeof typedef int char float double short long if else switch"
               L" case default for while do void return continue break const goto volatile static";
        macros = 
               L" #if #include #else #elif #endif #ifdef #ifndef #define #undef #line #error #pragma ";
			   */

        ZeroMemory(&default_char_format, sizeof(default_char_format));
        default_char_format.dwMask = CFM_COLOR | CFM_FACE | CFM_COLOR | CFM_SIZE;
        default_char_format.sStyle; 
        wcscpy(default_char_format.szFaceName, L"Consolas");
        default_char_format.yHeight = 13 * 14;
        default_char_format.crTextColor = color_text;

        update_from_scratch = false;

        rCtrl.SetBaseWindow(this);
        rCtrl.SetKeys(keys);
        rCtrl.RebuildLib(wstring(L""));
		s_szFileName[0] = '\0';
		s_szTitleName[0] = 'b';
		s_szTitleName[1] = '\0';

		MyCompiler.FileInitialize(m_hWnd);

        isCompleting = true;
}

CCocoCodeView::~CCocoCodeView()
{
        //KillTimer(1);
}

BOOL CCocoCodeView::PreCreateWindow(CREATESTRUCT& cs)
{
        // TODO: 在此处通过修改
        //  CREATESTRUCT cs 来修改窗口类或样式

        return CRichEditView::PreCreateWindow(cs);
}

void CCocoCodeView::OnInitialUpdate()
{
        CRichEditView::OnInitialUpdate();

        ed->SetDefaultCharFormat(default_char_format);
        SetSelColor(color_text);

        UpdateFromScratch();

        // 设置打印边距(720 缇 = 1/2 英寸)

        SetTimer(1, 10000, NULL);
        SetTimer(2, 10000, NULL);

        rCtrl.SetKeys(keys);
        rCtrl.RebuildLib(wstring(L""));


        SetMargins(CRect(720, 720, 720, 720));
}


// CCocoCodeView 打印


void CCocoCodeView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
        AFXPrintPreview(this);
#endif
}

BOOL CCocoCodeView::OnPreparePrinting(CPrintInfo* pInfo)
{
        // 默认准备
        return DoPreparePrinting(pInfo);
}


void CCocoCodeView::OnDestroy()
{
        // 析构时停用此项；这在
        // 使用拆分视图时非常重要 
        COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
        if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
        {
                pActiveItem->Deactivate();
                ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
        }
        CRichEditView::OnDestroy();
}


void CCocoCodeView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
        ClientToScreen(&point);
        OnContextMenu(this, point);
}

void CCocoCodeView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
        theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CCocoCodeView 诊断

#ifdef _DEBUG
void CCocoCodeView::AssertValid() const
{
        CRichEditView::AssertValid();
}

void CCocoCodeView::Dump(CDumpContext& dc) const
{
        CRichEditView::Dump(dc);
}

CCocoCodeDoc* CCocoCodeView::GetDocument() const // 非调试版本是内联的
{
        ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCocoCodeDoc)));
        return (CCocoCodeDoc*)m_pDocument;
}
#endif //_DEBUG


// 设置当前光标所在单词的颜色

void CCocoCodeView::SetWordColor(long offset, COLORREF color)
{
        long left, righ; GetWordRange(offset, left, righ);
        
        SetRangeColor(left, righ, color);
}

// 得到光标所在单词的范围
/* 
   单词的定义：将字符分成 标识符 符号 分隔符 三种，其中
        标识符为标准标识符定义加上'#'
        符号为除了空格和换行以外的可见字符（tab用四个空格代替）
        分隔符为空格 换行 和 EOF

        前两者连续组成的字符串为一个单词 
*/


void CCocoCodeView::GetWordRange(long offset, long & left, long & righ)
{
        ed->GetSel(left, righ);

        if (left != righ)
        {
                return ;
        }

        left = righ += offset;

        wchar_t c = GetCharAt(left, c);
        if (c == ' ' || c == '\n' || c == EOF)
        {
                return ;
        }
        wstring set = (identi_char.find(c) != 
                string::npos) ? identi_char : symbol_char;

        while (identi_char.find(c) != string::npos)
        {
                c = GetCharAt(--left, c);
        } 
        left++;

        c = GetCharAt(++righ, c);
        while (identi_char.find(c) != string::npos)
        {
                c = GetCharAt(++righ, c);
        }
}


// 得到从零开始第pos个字符
wchar_t CCocoCodeView::GetCharAt(long pos, wchar_t & ch)
{
        if (pos < 0 || pos >= GetWindowTextLength())
        {
                return ch = EOF;
        }

        wstring text; GetTextWstring(text);
        if (pos >= text.size())
        {
                return EOF;
        }
        else
        {
                return text[pos];
        }
}



// 得到所指定光标相对位置上的单词的颜色
void CCocoCodeView::GetWordColor(long offset, COLORREF & color)
{
        long left, righ; GetWordRange(offset, left, righ);

        CHARRANGE cr; ed->GetSel(cr); ed->SetSel(left, righ);

        CHARFORMAT2 char_format; ed->GetSelectionCharFormat(char_format);
        
        ed->SetSel(cr); color = char_format.crTextColor;
}


void CCocoCodeView::SetSelColor(COLORREF color)
{
        default_char_format.crTextColor = color;

        ed->SetSelectionCharFormat(default_char_format);

        default_char_format.crTextColor = color_text;
}


void CCocoCodeView::GetSelColor(COLORREF & color)
{
        CHARFORMAT2 cf; 

        ed->GetSelectionCharFormat(cf); 
        
        color = cf.crTextColor;
}


void CCocoCodeView::GetRangeColor(long left, long righ, COLORREF & color)
{
        CHARRANGE cr; ed->GetSel(cr);
        
        ed->SetSel(left, righ); GetSelColor(color);

        ed->SetSel(cr);
}

void CCocoCodeView::SetRangeColor(long left, long righ, COLORREF color)
{
        if (update_from_scratch)
        {
                paint.push_back(CPaintStruct(left, righ, color));
                return ;
        }

        CHARRANGE cr; ed->GetSel(cr);

        ed->SetSel(left, righ); SetSelColor(color);

        ed->SetSel(cr);
}



void CCocoCodeView::ReplaceText(long left, long righ, LPCTSTR text)
{
        CHARRANGE cr; ed->GetSel(cr);

        ed->SetSel(left, righ); ed->ReplaceSel(text);

        ed->SetSel(cr);
}



void CCocoCodeView::ReplaceWord(long offset, LPCTSTR word)
{
        CHARRANGE cr; ed->GetSel(cr);

        long left, righ; GetWordRange(offset, left, righ);

        ReplaceText(left, righ, word);

        ed->SetSel(cr);
}



long CCocoCodeView::GetLineIndent(long index, long & indent)
{
        wchar_t s[255]; 
        memset(s, 0, sizeof(char) * 255);
        
        ed->GetLine(index, s, 255);

        indent = 0;
        for (int i = 0; i < 255; i++)
        {
                if (s[i] != ' ')
                {
                        indent = i; return wcslen(s);
                }
        }
}

void CCocoCodeView::SetLineIndent(long index, long indent)
{
        long old_indent; GetLineIndent(index, old_indent);
        
        long count = 0, tmp;
        for (int i = 0; i < index; i++)
        {
                count += GetLineIndent(i, tmp);
        }

        ReplaceText(count, count + old_indent, L"");
        for (int i = 0; i < indent; i++)
        {
                ReplaceText(count, count, L" ");
        }
        long left, righ; ed->GetSel(left, righ);
        ed->SetSel(left + indent, left + indent);
}


void CCocoCodeView::GetTextWstring(wstring & text)
{
        CString str; GetWindowText(str);

        text = str.GetBuffer(str.GetLength());

        long npos = 0;
        while ((npos = text.find(
                L"\r\n"), npos) != string::npos)
        {
                text.replace(npos, 2, L"\n");
        }
}


void CCocoCodeView::SetTextWstring(wstring text)
{
        SetWindowText(text.c_str());
}

void CCocoCodeView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
        // TODO: 在此添加消息处理程序代码和/或调用默认值

        long left, righ; ed->GetSel(left, righ);
        if (left == righ && NotInComment())
        {
                SetSelColor(color_text);
        }

        if (nChar == VK_TAB)
        {
                ed->ReplaceSel(L"    ");
        }
        else if (!isCompleting || nChar != VK_UP && nChar != VK_DOWN 
                && nChar != VK_LEFT && nChar != VK_RIGHT
                && nChar != VK_RETURN && nChar != VK_ESCAPE)
        {
                CRichEditView::OnChar(nChar, nRepCnt, nFlags);
        }
        else
        {
                return ;
        }

        if (identi_char.find(nChar) != string::npos)
        {
                isCompleting = true;
        }
        if (isCompleting && !ContinueOnChar(nChar))
        {
                return ;
        }

        GetTextWstring(window_text);

        long line, indent;
        if (nChar == VK_RETURN)
        {
                GetCurrentLine(line);
                AutoIndent(line);
        }
        else if (nChar == '(')
        {
                ed->ReplaceSel(L")");
                SendMessage(WM_KEYDOWN, VK_LEFT, 1);
                ed->GetSel(left, righ);
                ed->SetSel(left, left);
        }
        else if (nChar == '[')
        {
                ed->ReplaceSel(L"]");
                SendMessage(WM_KEYDOWN, VK_LEFT, 1);
        }
        else if (nChar == '{')
        {
                ed->ReplaceSel(L"\n}");
                GetCurrentLine(line);
                AutoIndent(line);
                ed->GetSel(left, righ);
                GetLineIndent(line, indent);
                ed->SetSel(righ - indent - 2, righ - indent - 2);
        }

        if (nChar != VK_LEFT && nChar != VK_RIGHT && nChar != VK_UP && nChar != VK_DOWN)
        {
                HilightWord(-2, false);
                HilightWord(+0, false);
        }
}


void CCocoCodeView::GetWordWstring(long offset, wstring & word)
{
        long left, righ; GetWordRange(offset, left, righ);

        CString str; ed->GetTextRange(left, righ, str);

        word = str.GetBuffer(str.GetLength());
}


long CCocoCodeView::GetCurrentLine(long & index)
{
        long count = 0, tmp, left, righ;

        ed->GetSel(left, righ);
        for (int i = 0; i < ed->GetLineCount(); i++)
        {
                count += GetLineIndent(i, tmp);
                if (count > left) 
                {
                        return index = i;
                }
        }
}



void CCocoCodeView::HilightWord(long offset, bool hilight_anyway)
{
        wstring word; GetWordWstring(offset, word);

        if (word == L"")
        {
                return ;
        }
        word = L" " + word + L" ";

        COLORREF col; GetWordColor(offset, col);
        if (hilight_anyway || col != color_comm && col != color_stri)
        {
                if (keys.find(word) != string::npos)
                {
                        SetWordColor(offset, color_keys);
                }
                else if (macros.find(word) != string::npos)
                {
                        SetWordColor(offset, color_keys);
                }
                else 
                {
                        SetWordColor(offset, color_text);
                }
        }
}


void CCocoCodeView::AutoIndent(long line)
{
        long inde; 
        GetLineIndent(line - 1, inde); 
        SetLineIndent(line, inde);
}


void CCocoCodeView::IndentPlus(long line)
{
        long inde; 
        GetLineIndent(line - 1, inde); 
        SetLineIndent(line, inde + 4);
}


bool CCocoCodeView::NotInComment(void)
{
        COLORREF col; GetSelColor(col);
        return col != color_stri && col != color_comm;
}


void CCocoCodeView::UpdateFromScratch(void)
{
        CHARRANGE cr; ed->GetSel(cr);

        long left, righ;

        update_from_scratch = true;
        paint.clear();

        CheckComment(0);

        ed->SetReadOnly(true);
        ed->SetSel(0, -1);
        SetSelColor(color_text);

        ed->SetSel(0, 0);

        update_from_scratch = false;
        for (int i = 0; i < paint.size(); i++)
        {
                SetRangeColor(paint[i]);
        }

        ed->SetReadOnly(false);

        update_from_scratch = true;
        paint.clear();

        long size = GetWindowTextLength();
        for (long i = 0; i < size; i += 2)
        {
                HilightWord(i, false);
                GetWordRange(i, left, righ);
                i = righ - (left != righ);
        }

        ed->SetReadOnly(true);

        update_from_scratch = false;

        for (int i = 0; i < paint.size(); i++)
        {
                SetRangeColor(paint[i]);
        }

        ed->SetReadOnly(false);

        ed->SetSel(cr);
}


void CCocoCodeView::CheckComment(long start)
{
        wstring text; GetTextWstring(text);

        if (start >= text.size())
        {
                return ;
        }

        long nea, pos; wstring str = L"/*"; 
        
        nea = text.find(L"/*", start);
        pos = text.find(L"//", start);
        if (pos != string::npos && pos < nea || nea == string::npos)
        {
                nea = pos; str = L"//";
        }
        pos = text.find(L"\"", start);
        if (pos != string::npos && pos < nea || nea == string::npos)
        {
                nea = pos; str = L"\"";
        }

        if (nea == string::npos)
        {
                return ;
        }

        if (str == L"/*")
        {
                pos = text.find(L"*/", nea);
                if (pos == string::npos)
                {
                        pos = text.size() - 2;
                }
                SetRangeColor(nea, pos + 2, color_comm);
                CheckComment(pos + 2);
        }
        else if (str == L"//")
        {
                pos = text.find(L"\n", nea);
                if (pos == string::npos)
                {
                        pos = text.size() - 1;
                }
                SetRangeColor(nea, pos + 1, color_comm);
                CheckComment(pos + 1);
        }
        else if (str == L"\"")
        {
                long tmp; pos = nea;
                while (true)
                {
                        tmp = text.find(L"\"", pos + 1);
                        pos = text.find(L"\\\"", pos);
                        if (tmp == string::npos)
                        {
                                break;
                        }
                        if (pos + 1 == tmp)
                        {
                                pos = tmp + 1;
                        }
                        else 
                        {
                                break;
                        }
                }
                if (tmp == string::npos)
                {
                        tmp = text.size() - 1;
                }
                SetRangeColor(nea, tmp + 1, color_stri);
                CheckComment(tmp + 1);
        }
}


void CCocoCodeView::OnTimer(UINT_PTR nIDEvent)
{
        // TODO: 在此添加消息处理程序代码和/或调用默认值

        switch (nIDEvent)
        {
        case 1:
                UpdateFromScratch(); break;
        case 2:
                wstring text; 
                GetTextWstring(text);
                rCtrl.RebuildLib(text); break;
        }

        CRichEditView::OnTimer(nIDEvent);
}


void CCocoCodeView::SetRangeColor(const CPaintStruct & st)
{
        SetRangeColor(st.left, st.righ, st.color);
}

/* 
        继续补全的条件：
        1. 上下键
        2. 打入一个标识符字符
        3. 候选项不为空
 */

bool CCocoCodeView::ContinueOnChar(UINT key)
{
        CRect rc; GetClientRect(rc);

        CPoint pnt = ed->GetCharPos(0);

        wstring word; 
        if (key == VK_UP)
        {
                rCtrl.ChooseUp();
                isCompleting = true;
                return false;
        }
        if (key == VK_DOWN)
        {
                rCtrl.ChooseDown();
                isCompleting = true;
                return false;
        }
        if (key == VK_RETURN)
        {
                word = rCtrl.Choose();
                ReplaceWord(-1, word.c_str());

                isCompleting = false;
                rCtrl.CompletionOff();
                return false;
        }
        if (key == VK_ESCAPE)
        {
                rCtrl.CompletionOff();
                isCompleting = false;
                return false;
        }

        if (identi_char.find(key) != string::npos)
        {
                isCompleting = true; 
        }
        else 
        {
                isCompleting = false; 
                rCtrl.CompletionOff();
                return true;
        }

        GetWordWstring(-1, word);
        if (rCtrl.CompletionOn(GetCurCharPos(), word))
        {
                isCompleting = true; 
        }
        else
        {
                isCompleting = false; 
                rCtrl.CompletionOff();
        }
}


CPoint CCocoCodeView::GetCurCharPos(void)
{
        long left, righ; ed->GetSel(left, righ);
        return ed->GetCharPos(righ);
}


void CCocoCodeView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
        // TODO: 在此添加消息处理程序代码和/或调用默认值

        if (!isCompleting || nChar != VK_UP && nChar != VK_DOWN 
                && nChar != VK_LEFT && nChar != VK_RIGHT
                && nChar != VK_RETURN && nChar != VK_ESCAPE)
        {
                CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
        }
        else
        {
                ContinueOnChar(nChar);
        }
}


void CCocoCodeView::StartCompleting(void)
{
        long left, righ; ed->GetSel(left, righ);
        if (left != righ)
        {
                return ;
        }

        isCompleting = true;
        wstring word; GetWordWstring(-1, word);
        if (rCtrl.CompletionOn(GetCurCharPos(), word))
        {
                isCompleting = true; 
        }
        else
        {
                isCompleting = false; 
                rCtrl.CompletionOff();
        }
}


void CCocoCodeView::OnStartcompletion()
{
        // TODO: 在此添加命令处理程序代码
        StartCompleting();
}



void CCocoCodeView::Oncompiler()
{
	HWND hWnd = m_hWnd; 
	
	MyCompiler.FileOpenDlg(hWnd,s_szFileName,s_szTitleName);
	if (!MyCompiler.Doit())
	{
		return;
	}
	/*
	CFile file(L"compresult.txt",CFile::modeRead); 		
	char *pBuf;		
	DWORD dwFileLen;		
	dwFileLen=file.GetLength();		
	pBuf=new char[dwFileLen+1];		
	pBuf[dwFileLen]=0;		
	file.Read(pBuf,dwFileLen);        
	CString FileData;
	FileData.Format(_T("%s"), pBuf);
	delete pBuf;

	CFile file1(L"runresult.txt",CFile::modeRead); 		
	char *pBuf1;		
	DWORD dwFileLen1;		
	dwFileLen1=file1.GetLength();		
	pBuf1=new char[dwFileLen1+1];		
	pBuf1[dwFileLen1]=0;		
	file1.Read(pBuf1,dwFileLen1);        
	CString FileData1;
	FileData1.Format(_T("%s"), pBuf1);
	delete pBuf1;
	*/
	CString FileData;
	
	wstring wtmp;
	CFile file(L"compresult.txt",CFile::modeRead);
	CArchive ar(&file,CArchive::load);
	if (file.GetLength() == 0) 
	{
			MessageBox(L"Compile Successfully!",L"Compile Result");
	}
	else
	{
		ar >> FileData;
		MessageBox(FileData,L"Compile Result");
		return;
	}
	wifstream wfin1("runresult.txt");
	CString FileData1;
	while(getline(wfin1,wtmp))
	{	
		FileData1 += wtmp.c_str();
		FileData1 += L"\n";
	}
	/*
	CFile file1(L"runresult.txt",CFile::modeRead);
	CArchive ar1(&file1,CArchive::load);
	FileData1 = ar1.GetFile();
	 */
	MessageBox(FileData1,L"Running Result");

}
