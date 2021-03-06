#pragma once
#pragma warning( disable : 4996) 

#include <string>
#include <vector>

const float g_ForwardArcWeight = 0.0;
const float g_BackoffArcWeight = 0.0;

namespace bjfufst{

	typedef int Label;
	typedef float Weight;
	typedef int StateId;
	typedef std::string Symbol;
	const Label EPS=0;

}


//splitting 
void split(std::string line, std::vector<std::string> &parts, std::string pattern);

//integer checker
bool isNum(std::string str);

//int to ascii converter
char *intoa(int value, char *string, int radix);
