
// CocoCode.h : CocoCode Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CCocoCodeApp:
// �йش����ʵ�֣������ CocoCode.cpp
//

class CCocoCodeApp : public CWinAppEx
{
public:
	CCocoCodeApp();

        HMODULE m_hmodule;


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCocoCodeApp theApp;