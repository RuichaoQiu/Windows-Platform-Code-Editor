#ifdef COMPLETION_DLL_API
#else
#define COMPLETION_DLL_API _declspec(dllimport)
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

//#define KEYWORD_FILE "getdata.txt"
const int kind = 63;
struct node
{
	node *next[kind];
	node *father;
	int count,newcount;
	bool visit;
	node()
	{
		father = NULL;
		count = 0;
		newcount = 0;
		visit = false;
		memset(next,NULL,sizeof(next));
	}
};

const wchar_t alp[kind] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z','0','1','2','3','4','5','6','7','8','9','_'};

class COMPLETION_DLL_API Completion
{
public:
	Completion();
	~Completion();
	void rebuild(wstring words[],int _tot);
	void AutoCompletion(wstring strKey);

	int tot;
	wstring match[2000];
	int match_num;
	int max_num;

private:
	bool check(wstring str);
	void insert(wstring str);
	void search(node *p,wstring str);
	node* automation(wstring strKey);
	void releasenode(node *p);
	void deletetree(node *p);
	void cleantree();
	void maintain(node *p,int index);
	void AddWord(wstring str);
	void DeleteWord(wstring str);
	node *root;
};