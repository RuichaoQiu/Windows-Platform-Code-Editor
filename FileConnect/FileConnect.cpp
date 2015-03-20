#include <string>
#include <iostream>

using namespace std;

extern "C" _declspec(dllexport)void DoFileConnect(char* route)
{
	string fileroute;
	fileroute = ".\\compiler\\Bin\\g++ -o result ";
	fileroute += route;
	fileroute += " 2> compresult.txt";
	system(fileroute.c_str());
	//system("pause");
	system("result > runresult.txt");
	//system("pause");
}