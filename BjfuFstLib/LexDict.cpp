#include "LexDict.h"
namespace bjfufst{

	void LexDict::addLex(std::string word, std::string words)
	{
		_dict[word].push_back(words);
		_dict[word].shrink_to_fit();
	}

	std::vector<std::vector<std::string> > LexDict::getLex(std::string word)
	{
		if (_dict.find(word) == _dict.end())
		{
			std::cout << "ERROR: no lex for " << word << std::endl;
		}
		std::vector<std::string> lexs;
		std::vector<std::vector<std::string> > lexs_ret;
		std::vector<std::string> phones;

		lexs = _dict[word];
		for (size_t i = 0; i < lexs.size(); i++)
		{
 			std::string lex  = lexs[i];
			split(lex, phones, " \t");
			lexs_ret.push_back(phones);
		}
		return lexs_ret;
	}

	std::vector<std::vector<std::string> >LexDict::operator[](std::string word)
	{
		return this->getLex(word);
	}

	LexDict::LexDict()
	{

	}

	LexDict::~LexDict()
	{

	}

}