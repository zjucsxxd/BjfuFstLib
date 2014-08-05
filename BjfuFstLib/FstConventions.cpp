#include "FstConventions.h"




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

bool isNum(std::string str)
{
	return (atoi(str.c_str()) == 0 && !(str.size() == 1 && str[0] == '0'));
}

char * intoa(int value, char *string, int radix)
{
	if (radix != 10){
		printf("radix wrong in intoa!\n");
		return NULL;
	}
	int a, b, c, d;
	a = value / 1000;
	b = (value % 1000) / 100;
	c = (value % 100) / 10;
	d = value % 10;
	char *str = new char[10];
	int pos = 0;
	if (a > 0)
		str[pos++] = a + '0';
	if (b > 0 || pos > 0)
		str[pos++] = b + '0';
	if (c > 0 || pos > 0)
		str[pos++] = c + '0';
	if (d > 0 || pos > 0)
		str[pos++] = d + '0';
	if (pos == 0)
		str[pos++] = '0';
	str[pos] = '\0';
	strcpy(string, str);
	delete[] str;
	return "ok";
}
