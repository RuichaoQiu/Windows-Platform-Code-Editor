#define COMPLETION_DLL_API _declspec(dllexport)
#include "Completion.h"

Completion::Completion()
{
	root = new node();
	max_num = 0;
}
/*
void Completion::setKeys(wstring keys[],int _tot)
{
	cleantree();
	
	wstring str;
	for (int i = 0; i < _tot; ++ i)
	{
		insert(keys[i],true);
	}

}
*/
/*
void Completion::quick(wstring words[],int l,int r)
{
	int i,j;
	wstring mid,tmp;
	i = l;
	j = r;
	mid = words[(i+ j) / 2];
	while (i < j)
	{
		while (words[i] < mid) ++ i;
		while (words[j] > mid) -- j;
		if (i <= j)
		{
			tmp = words[i];
			words[i] = words[j];
			words[j] = tmp;
			++ i;
			-- j;
		}
	}
	if (i < r) quick(i,r);
	if (l < j) quick(l,j);
}
*/
void Completion::rebuild(wstring words[],int _tot)
{
	//quick(words,0,_tot-1);
	for ( int i = 0; i < _tot; ++ i)
	{
		AddWord(words[i]);
		//wcout << "haha ::" << words[i] << endl;
	}
	maintain(root,0);
}

void Completion::maintain(node* p,int index)
{
	if (p != root && !(p->visit))
	{
		p->father->next[index] = NULL;
		deletetree(p);
		return;
	}
	p->count = p->newcount;
	p->newcount = 0;
	p->visit = false;
	for (int i = 0; i < kind; ++ i)
	{
		if (p->next[i] != NULL)
		{
			maintain(p->next[i],i);
		}
	}
}

bool Completion::check(wstring str)
{
	for ( int i = 0; i < str.size(); ++ i)
	{
		if (str[i] >= 'a' && str[i] <= 'z')	continue;
		else if (str[i] >= 'A' && str[i] <= 'Z') continue;
		else if (str[i] >= '0' && str[i] <= '9') continue;
		else if (str[i] == '_') continue;
		else return true;
	}
	return false;
}

void Completion::insert(wstring str)
{
	node *p = root;
	node *prev;
	int i = 0,index;
	if (check(str)) return;
	while (i < str.size())
	{
		if (str[i] >= 'a' && str[i] <= 'z')	index = str[i] - 'a';
		else if (str[i] >= 'A' && str[i] <= 'Z') index = str[i] - 'A' + 26;
		else if (str[i] >= '0' && str[i] <= '9') index = str[i] - '0' + 52;
		else if (str[i] == '_') index = 62;
		if (p->next[index] == NULL) p->next[index] = new node();
		prev = p;
		p = p->next[index];
		p->father = prev;
		i ++;
		p->visit = true;
	}
	p->newcount ++;
}

void Completion::search(node *p,wstring str)
{
	if (p->count > 0)
	{
		match[match_num] = str;
		++ match_num;
	}
	for (int i = 0; i < kind; ++ i)
	{
		if (p->next[i] != NULL) search(p->next[i],str + alp[i]);
	}
}

node* Completion::automation(wstring strKey)
{
	bool ok = true;
	node *p = root;
	int index;
	if (check(strKey)) return NULL;
	for (int i = 0; i < strKey.size(); ++ i)
	{
		if (strKey[i] >= 'a' && strKey[i] <= 'z')	index = strKey[i] - 'a';
		else if (strKey[i] >= 'A' && strKey[i] <= 'Z') index = strKey[i] - 'A' + 26;
		else if (strKey[i] >= '0' && strKey[i] <= '9') index = strKey[i] - '0' + 52;
		else if (strKey[i] == '_') index = 62;
		if (p->next[index] == NULL)
		{
			ok = false;
			break;
		}
		else
		{
			p = p->next[index];
		}
	}
	if (!ok)
	{
		return NULL;
	}
	return p;
	
	
}

void Completion::AutoCompletion(wstring strKey)
{
	node *p;
	match_num = 0;
	p = automation(strKey);
	if (p != NULL) search(p,strKey);
}
/*
void Completion::releasenode(node *p)
{
	if (p->count > 0) return;
	bool ok = true;
	for (int i = 0; i < kind; ++ i)
		if (p->next[i] != NULL )
		{
			ok = false;
			break;
		}
	if (ok)
	{
		node *prev = p->father;
		delete p;
		releasenode(prev);
	}
}

void Completion::DeleteWord(wstring str)
{
	node *p;
	p = automation(str);
	if (p == NULL || p->count == 0) return;
	-- p->count;
	releasenode(p);
}
*/
void Completion::AddWord(wstring str)
{
	insert(str);
}

void Completion::deletetree(node *p)
{
	
	for (int i = 0; i < kind; ++ i)
	{
		
		if (p->next[i] != NULL) 
		{
			deletetree(p->next[i]);
			p->next[i] = NULL;
		}
		
	}
	
	delete p;
	
}



Completion::~Completion()
{
	if (root != NULL) deletetree(root);
}