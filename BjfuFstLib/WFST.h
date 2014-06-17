#pragma once
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
	void SaveCFastLM(const char * filename)
	{
		CFastLM fastlm;
		fastlm.fastNodeNum = _fst.NumStates();
		fastlm.fastEdgeNum = 0;
		for (int s = 0; s < _fst.states.size(); s++)
		{
			fastlm.fastEdgeNum += _fst.states[s].arcs.size();
		}
		fastlm.pEdgeArray = new CFastEdge[fastlm.fastEdgeNum];

		int edge_pos = 0;
		for (int s = 0; s < _fst.states.size(); s++)
		{
			State & it_state = _fst.states[s];
			fastlm.fastEdgeNum += it_state.arcs.size();
			fastlm.pNodeArray[s].edgeStart = edge_pos;
			fastlm.pNodeArray[s].edgeCount = it_state.arcs.size();
			fastlm.pNodeArray[s].best = 0;
			for (int a = 0; a < it_state.arcs.size(); a++)
			{
				Arc & it_arc = it_state.arcs[a];
				fastlm.pEdgeArray[edge_pos].idata = it_arc.ilabel;
				fastlm.pEdgeArray[edge_pos].odata = it_arc.olabel;
				fastlm.pEdgeArray[edge_pos].nodeTo = it_arc.nextstate;
				fastlm.pEdgeArray[edge_pos].weight = it_arc.weight;
				if (it_arc.weight>fastlm.pNodeArray[s].best)
				{
					fastlm.pNodeArray[s].best = it_arc.weight;
				}
				edge_pos++;
			}
		}

		fastlm.pPhons = new char *[this->_isymbol.Size()];//potential bug due to noncontinuous label.
		for (int i = 0; i < this->_isymbol.Size();i++)
		{
			strcpy(fastlm.pPhons[i], this->_isymbol.Find(i).c_str());
		}

		fastlm.pWords = new char *[this->_osymbol.Size()];//potential bug due to noncontinuous label.
		for (int i = 0; i < this->_osymbol.Size(); i++)
		{
			strcpy(fastlm.pWords[i], this->_osymbol.Find(i).c_str());
		}

		fastlm.fastStNode = fastlm.fastEdNode = 0;//temporarily define start&end node at ZERO.

		fastlm.SaveFastLM(filename);
	}


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
