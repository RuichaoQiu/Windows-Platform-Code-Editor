// PopListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "CocoCode.h"
#include "PopListCtrl.h"


// CPopListCtrl

CPopListCtrl::CPopListCtrl()
        : base_window(NULL), box(NULL)
{
        identi_char = L"qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_1234567890";
        box = NULL;
}

CPopListCtrl::~CPopListCtrl()
{
        if (box != NULL)
        {
                box->DestroyWindow();
                delete box; box = NULL;
        }
}

BOOL CPopListCtrl::InitInstance()
{
	// TODO: 在此执行任意逐线程初始化
	return TRUE;
}

int CPopListCtrl::ExitInstance()
{
	// TODO: 在此执行任意逐线程清理
        return TRUE;
}

// CPopListCtrl 消息处理程序


void CPopListCtrl::SetBaseWindow(CWnd * parent)
{
        base_window = parent;
}


void CPopListCtrl::RebuildLib(wstring & arr)
{
        read_keys(arr, retval); int size, size1;
        wstring *words = new wstring [size = (
                (size1 = retval.size()) + keys.size())];
        for (int i = 0; i < size1; i++)
        {
                words[i] = retval[i];
        }
        for (int i = size1; i < size; i++)
        {
                words[i] = keys[i - size1];
        }

        rebuild(words, size); delete [] words;
}


void CPopListCtrl::SetKeys(wstring & keyset)
{
        read_keys(keyset, keys);
}


void CPopListCtrl::read_keys(wstring & text, vector<wstring> & retval)
{
        //TODO: insert return statement here
        retval.clear();

        long left = 0, righ = 0, size = text.size();
        while (left < size)
        {
                if (identi_char.find(text[left]) == string::npos)
                {
                        left++; righ = left; continue;
                }
                if (righ < size && identi_char.find(text[righ]) != string::npos)
                {
                        righ++; continue;
                }
                else 
                {
                        retval.push_back(get_wstring(text, left, righ));
                        left = righ + 1; righ = left;
                }
        }
}


wstring CPopListCtrl::get_wstring(wstring & text, long left, long righ)
{
        //TODO: insert return statement here
        return wstring(text.c_str() + left, righ - left);
}


bool CPopListCtrl::CompletionOn(CPoint pnt, wstring word)
{
        if (box != NULL)
        {
                box->DestroyWindow();
                delete box; box = NULL;
        }
        box = new PopuoListBox(pnt.x, pnt.y, base_window);

        AutoCompletion(word);

        if (match_num == 0)
        {
                CompletionOff(); 
                return false;
        }
        else
        {
                selected_word = box->AddWstringArray(match, match_num);
                return true;
        }
}


void CPopListCtrl::CompletionOff(void)
{
        if (box != NULL)
        {
                box->DestroyWindow();
                delete box; box = NULL;
        }
}


void CPopListCtrl::ChooseUp(void)
{
        selected_word = box->ChooseUp();
}


void CPopListCtrl::ChooseDown(void)
{
        selected_word = box->ChooseDown();
}



wstring CPopListCtrl::Choose(void)
{
        CompletionOff();
        return selected_word;
}
