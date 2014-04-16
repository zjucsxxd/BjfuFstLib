#include "SymbolTable.h"
#include <fstream>
#include <iostream>
#include <string>
namespace bjfufst{

	SymbolTable::SymbolTable()
	{
	}

	SymbolTable::SymbolTable(const char * filename)
	{
		this->ReadText(filename);
	}


	SymbolTable::~SymbolTable()
	{
	}

	Label SymbolTable::AddSymbol(const Symbol & symbol)
	{
		Label pos = Find(symbol);
		if (pos == -1)
		{
			pos = this->Size();
		}
		insertPair(_next_available_label, symbol);
		return pos;
	}

	Label SymbolTable::AddSymbol(const Symbol & symbol, const Label label)
	{
		insertPair(label, symbol);
		if (_next_available_label == label)	//if conflict with _next_available_label, then update it.
		{
			findNextAvailableLabel();
		}
		return label;
	}

	size_t SymbolTable::Size()
	{
		return this->_map_lbl_sym.size();
	}

	Label SymbolTable::Find(const Symbol & symbol) const
	{
		auto it = _map_sym_lbl.find(symbol);
		if (it==_map_sym_lbl.end())
		{
			return -1;
		}
		return it->second;
	}

	bool SymbolTable::WriteText(const char * filename)
	{
		std::ofstream ofile(filename);
		for (auto & record:_map_lbl_sym)
		{
			ofile << record.first << '\t' << record.second << '\n';
		}
		return true;
	}

	bool SymbolTable::ReadText(const char * filename)
	{
		std::ifstream ifile(filename);
		Label l;
		Symbol s;
		while (ifile >> l >> s)
		{
			insertPair(l, s);
		}
		return true;

	}

	Symbol SymbolTable::Find(const Label &label) const
	{
		auto it = _map_lbl_sym.find(label);
		if (it == _map_lbl_sym.end())
		{
			return "";
		}
		return it->second;
	}

	void SymbolTable::Clear()
	{
		this->_map_lbl_sym.clear();
		this->_map_sym_lbl.clear();
	}

	void SymbolTable::insertPair(const Label & lbl, const Symbol & sym)
	{
		_map_lbl_sym[lbl] = sym;
		_map_sym_lbl[sym] = lbl;
	}

	void SymbolTable::insertPair(const Symbol & sym, const Label & lbl)
	{
		insertPair(lbl,sym);
	}

	Label SymbolTable::findNextAvailableLabel()
	{
		Label lbl1, lbl2;
		lbl1 = 0;

		for (auto & it:_map_lbl_sym)
		{
			lbl2 = it.first;
			if (lbl2-lbl1!=1)
			{
				break;
			}
			lbl2 = lbl1;
		}
		_next_available_label = lbl1 + 1;
		return _next_available_label;

	}


}