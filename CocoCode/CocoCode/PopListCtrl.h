#pragma once


#include "PopuoListBox.h"
#include <vector>

using namespace std;

// CPopListCtrl

class CPopListCtrl : public Completion
{
private:
        CWnd * base_window;
        vector<wstring> keys, retval;
        wstring identi_char;

public:
        wstring selected_word;

private:
        PopuoListBox *box;

public:
	CPopListCtrl();           // 动态创建所使用的受保护的构造函数
	virtual ~CPopListCtrl();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
        void SetBaseWindow(CWnd * parent);
        void RebuildLib(wstring & arr);
        void SetKeys(wstring & keyset);
        void read_keys(wstring & text, vector<wstring> & retval);
        wstring get_wstring(wstring & text, long left, long righ);
        bool CompletionOn(CPoint pnt, wstring word);
        void CompletionOff(void);
        void ChooseUp(void);
        void ChooseDown(void);
        wstring Choose(void);
};


