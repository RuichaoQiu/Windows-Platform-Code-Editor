#include <string>
#include <iostream>

using namespace std;

//extern "C" 
//{
	_declspec(dllexport) const wchar_t *  LoadKeys(void)
	{
		return L" asm bool explicit export friend inline mutable operator"
			L" template typeid virtual wchar_t delete namespace typename"
			L" using const_cast dynamic_cast reinterpret_cast static_cast"
			L" class new public private protected this try throw catch true"
			L" false unsigned signed struct enum union auto register extern"
			L" sizeof typedef int char float double short long if else switch"
			L" case default for while do void return continue break const goto volatile static";
	}

	_declspec(dllexport) const wchar_t *  LoadMacro(void)
	{
		return L" #if #include #else #elif #endif #ifdef #ifndef #define #undef #line #error #pragma ";
	}

	_declspec(dllexport) const wchar_t *  LoadIdChar(void)
	{
		return L"_qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890#";
	}

	_declspec(dllexport) const wchar_t *  LoadSymbol(void)
	{
		return L"~!@$%^&*()+`-=[]{}\\|;:\"'<>,.?/";
	}
//}