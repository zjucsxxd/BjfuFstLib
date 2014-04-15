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
	//����FST�ļ�
	bool LoadFST(const char * filename);	

	//����FST�ļ�
	bool SaveFST(const char * filename);	

	//ִ��Minimize����
	void Minimize();	

	//ִ��Determinize������
	void Determinize();

	//name of fst.
	Symbol fstname;

// 	//�����WFST
// 	void Clear();

	WFST(const Symbol fstname)
	{
	}

	~WFST()
	{
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




