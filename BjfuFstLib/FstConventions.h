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
void split(std::string line, std::vector<std::string> &parts, std::string pattern);

//integer checker
bool isNum(std::string str);
