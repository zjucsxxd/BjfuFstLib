#ifndef _SYMBOL_TABLE_H
#define _SYMBOL_TABLE_H

#include <vector>
#include <map>
#include <algorithm>
#include "FstConventions.h"
namespace bjfufst{
	class SymbolTable
	{
	public:
		SymbolTable();
		virtual ~SymbolTable();
		SymbolTable(const char * filename);
	public:
		//size of symbol table.
		size_t Size();

		//add a symbol and return the new label.or return existing label if exists.
		Label AddSymbol(const Symbol & symbol);	
		Label AddSymbol(const Symbol & symbol, const Label label);

		//find label from symbol. return -1 if not found.
		Label Find(const Symbol & symbol) const;

		//find symbol from label. return "" if not found.
		Symbol Find(const Label &label) const;

		//write symbol table to text
		bool WriteText(const char * filename);

		//read symbol table from texts
		bool ReadText(const char * filename);

		//clear all content.
		void Clear();

	private:
		//base structure, vector of symbol. Unique elements.
		Label _next_available_label;

		//v2.Dual-way mapping, for fast & unique indexing 
		std::map<Label, Symbol> _map_lbl_sym;
		std::map<Symbol, Label> _map_sym_lbl;

		//primitive operation, inserting a pair of data. May overwrite old data.
		void insertPair(const Label & lbl, const Symbol & sym);
		void insertPair(const Symbol & sym, const Label & lbl);

		//find next available label and update it.
		Label findNextAvailableLabel();
	};
}
#endif 