#include "FstConventions.h"
#include <vector>
#include <string>
#include <map>
#include "CompactSet.h"
#include <iostream>
namespace bjfufst{

	class LexDict
	{
	public:
		LexDict();
		~LexDict();
		void addLex(std::string word, std::string lexs);
		std::vector<std::vector<std::string> >  getLex(std::string word);
		std::vector<std::vector<std::string> >  operator[](std::string word);


	private:
		std::map<std::string,std::vector<std::string> > _dict;
	};

	

}