#pragma once
#include "stdafx.h"
#include <iostream>
using namespace std;

class Compiler
{
public:
	bool Doit();
	void FileInitialize(HWND hWnd);
	bool FileOpenDlg(HWND hWnd, PTSTR pstrFileName, PTSTR pstrTitleName);
	bool FileSaveDlg(HWND hWnd, PTSTR pstrFileName, PTSTR pstrTitleName);
	string s;
	int si;
};