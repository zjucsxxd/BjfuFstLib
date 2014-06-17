#ifndef _WFST_H
#define _WFST_H


#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include "fst.h"
#include "SymbolTable.h"
#include "FastLM.h"
using namespace bjfufst;

void split(Symbol line, std::vector<Symbol> &parts, Symbol pattern);

typedef std::vector<Symbol> vstr;
typedef std::vector<std::vector<Symbol> > vvstr;


typedef std::map<Symbol, vvstr > LexDict;


class WFST
{
public:
	//载入FST文件
	bool LoadFST(const char * filename);	

	//储存FST文件
	bool SaveFST(const char * filename);	

	//执行Minimize操作
	void Minimize();	

	//执行Determinize操作。
	void Determinize();

	//draw fst in dot format.
	void Draw(const char * filename);

	//WFST to CFastLM Converter
	void SaveCFastLM(const char * filename);


	//name of fst.
	std::string fstname;

	//tool func, returning a list of inarcs of all states.
	std::map<StateId, std::vector<Arc_Pos> > WFST::updateArcIn();


	WFST(const std::string fstname)
		:fstname(fstname)
	{

	}

	//copy constructor
	WFST(const WFST& wfst)
		:_fst(wfst._fst), _isymbol(wfst._isymbol), _osymbol(wfst._osymbol), _ssymbol(wfst._ssymbol)
	{

	}


	~WFST()
	{
	}

	void Triphone()
	{
		this->_fst.Triphone();
// 		throw std::logic_error("The method or operation is not implemented.");
	}

	

	//internal openFst implement
	fst _fst;
	SymbolTable _isymbol, _osymbol, _ssymbol;

};

#endif