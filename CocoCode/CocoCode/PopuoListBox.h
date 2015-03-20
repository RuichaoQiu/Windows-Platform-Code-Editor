#pragma once

#include "Completion.h"
#pragma comment(lib, "AutoCompletion.lib")

// PopuoListBox

class PopuoListBox : public CListBox
{
	DECLARE_DYNAMIC(PopuoListBox)

public:
        const static int width =  80;
        const static int heigh = 100;

public:
	PopuoListBox(long x, long y, CWnd *parent);
	virtual ~PopuoListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
        wstring AddWstringArray(wstring arr[], int size);
        wstring GetTextW(int index);
        wstring ChooseUp(void);
        wstring ChooseDown(void);
};


