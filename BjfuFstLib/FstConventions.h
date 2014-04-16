#pragma once
#pragma warning( disable : 4996) 

#include <string>
#include <vector>
namespace bjfufst{

	typedef int Label;
	typedef float Weight;
	typedef int StateId;
	typedef std::string Symbol;

}
//splitting 
void split(std::string line, std::vector<std::string> &parts, std::string pattern)
{
	int n = line.size() + 1;
	char *buffer = new char[n];
	strcpy(buffer, line.c_str());
	char *p = (char *)buffer, *q = 0;

	const char *seg = "\r \n\t";
	if (pattern.size() > 0)
		seg = pattern.c_str();

	parts.clear();

	while (1 == 1)
	{
		// trim left
		while (*p && strchr(seg, *p))p++;
		if (*p == 0) break;

		// get info
		q = p;
		while (*q && !strchr(seg, *q))q++;
		char ch = *q; *q = 0;
		parts.push_back(p);
		*q = ch; p = q;
	}

	delete[] buffer;
}

//integer checker
bool isNum(std::string str)
{
	return (atoi(str.c_str()) == 0 && !(str.size() == 1 && str[0] == '0'));
}
