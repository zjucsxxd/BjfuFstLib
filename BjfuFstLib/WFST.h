#pragma once
#include <map>
#include <vector>
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include "fst.h"
#include "SymbolTable.h"
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

bool WFST::LoadFST(const char * filename)
{
	return _fst.ReadText(filename);
}

bool WFST::SaveFST(const char * filename)
{
	this->_isymbol.WriteText((fstname + "isymbs.txt").c_str());
	this->_osymbol.WriteText((fstname + "osymbs.txt").c_str());
	this->_ssymbol.WriteText((fstname + "ssymbs.txt").c_str());

	return _fst.WriteText(filename);
}

void WFST::Minimize()
{
	_fst.Minimize();
}

void WFST::Determinize()
{
	_fst.Determinize();
}

void WFST::Draw(const char * filename)
{
	_fst.Draw(filename, _isymbol, _osymbol);
}

std::map<StateId,std::vector<Arc_Pos> > WFST::updateArcIn()
{
	std::map<StateId, std::vector<Arc_Pos> > retval;

	for (int i = 0; i < _fst.states.size(); i++)
	{
		auto& ref_state = _fst.states[i];
		for (int j = 0; j < ref_state.arcs.size();j++)
		{
			auto& ref_arc = ref_state.arcs[j];
			retval[ref_arc.nextstate].push_back(Arc_Pos(i, j));
		}
	}
	return retval;
}
