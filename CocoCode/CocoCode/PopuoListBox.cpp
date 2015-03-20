// PopuoListBox.cpp : 实现文件
//

#include "stdafx.h"
#include "CocoCode.h"
#include "PopuoListBox.h"

// PopuoListBox

IMPLEMENT_DYNAMIC(PopuoListBox, CListBox)

PopuoListBox::~PopuoListBox()
{

}

PopuoListBox::PopuoListBox(long x, long y, CWnd *parent)
{
        Create(WS_DLGFRAME | WS_CHILD | WS_VISIBLE | 
                LBS_DISABLENOSCROLL | LBS_WANTKEYBOARDINPUT, 
                CRect(x, y, x + width, y + heigh), parent, 201);
}


BEGIN_MESSAGE_MAP(PopuoListBox, CListBox)
END_MESSAGE_MAP()



// PopuoListBox 消息处理程序



wstring PopuoListBox::AddWstringArray(wstring arr[], int size)
{
        for (int i = 0; i < size; i++)
        {
                AddString(arr[i].c_str());
        }

        SetCurSel(0);

        return GetTextW(0);
}


wstring PopuoListBox::GetTextW(int index)
{
        CString str; GetText(index, str);

        return wstring(str.GetBuffer(str.GetLength()));
}


wstring PopuoListBox::ChooseUp(void)
{
        int sel = GetCurSel();
        sel -= (sel > 0);
        SetCurSel(sel);
        
        return GetTextW(sel);
}


wstring PopuoListBox::ChooseDown(void)
{
        int sel = GetCurSel();
        sel += (sel + 1 < GetCount());
        SetCurSel(sel);

        return GetTextW(sel);
}
