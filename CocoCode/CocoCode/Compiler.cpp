#include "stdafx.h"
#include "Compiler.h"
#include <iostream>
#include <fstream>

OPENFILENAME Ofile;
using namespace std;
//extern "C" _declspec(dllimport) void DoFileConnect(char* route);
bool Compiler::FileSaveDlg (HWND hWnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
	Ofile.hwndOwner                 = hWnd ;
	Ofile.lpstrFile                 = pstrFileName ;
	Ofile.lpstrFileTitle            = pstrTitleName ;
	Ofile.Flags                     = OFN_OVERWRITEPROMPT ;
	return GetSaveFileName (&Ofile) ;
} 

bool Compiler::FileOpenDlg(HWND hWnd, PTSTR pstrFileName, PTSTR pstrTitleName)
{
	Ofile.hwndOwner                 = hWnd ;
	Ofile.lpstrFile                 = pstrFileName ;
	Ofile.lpstrFileTitle            = pstrTitleName ;
	Ofile.Flags                     = OFN_HIDEREADONLY | OFN_CREATEPROMPT ;
	return GetOpenFileName (&Ofile) ;
} 

void Compiler::FileInitialize(HWND hWnd)
{
	Ofile.lStructSize = sizeof(OPENFILENAME);
	Ofile.hwndOwner = hWnd;
	Ofile.hInstance = NULL;
	Ofile.lpTemplateName = NULL;
	Ofile.lpstrFilter = _T("*.All Files\0*.*\0\0");
	Ofile.lpstrCustomFilter = NULL;
	Ofile.nMaxCustFilter = 0;
	Ofile.nFilterIndex = 1;
	Ofile.lpstrFile = NULL;
	Ofile.nMaxFile = MAX_PATH;
	Ofile.lpstrFileTitle = NULL;
	Ofile.nMaxFileTitle = MAX_PATH;
	Ofile.lpstrInitialDir = NULL;
	Ofile.lpstrTitle = NULL;
	Ofile.Flags = 0;
	Ofile.nFileOffset = 0;
	Ofile.nFileExtension = 0;
	Ofile.lpstrDefExt = _T("");
	Ofile.lCustData = 0;
	Ofile.lpfnHook = NULL;
	Ofile.dwReserved = 0;
	Ofile.FlagsEx =0;
} 

string convert(LPWSTR in)
{
	string ret;
	while (*in != '\0')
	{
		ret.push_back(*in);
		in++;
	}
	return ret;
}



bool Compiler::Doit()
{
	s = "";
	s = convert(Ofile.lpstrFile);
	if (s.size() == 0) return false;
	for ( si = s.size() - 1; si >= 0; -- si)
		if (s[si] == '\\') break;
	++ si;
	s = s.substr(si);

	HMODULE funcDll = NULL;
	funcDll = LoadLibraryA("FileConnect.dll");
	DWORD dw = GetLastError();
	ASSERT(funcDll != NULL);
	typedef void (*DoFileConnect)(char* route);
	DoFileConnect doConnect = NULL;
	doConnect = (DoFileConnect)GetProcAddress(funcDll,"DoFileConnect");
	ASSERT(doConnect != NULL);
	doConnect((char*)s.c_str());
	::FreeLibrary(funcDll);

//	FileConnect(s);
	return true;
}

