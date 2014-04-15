#include "fst.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
namespace bjfufst{

	fst::fst()
	{
	}

	fst::fst(const char* filename)
	{

	}


	fst::~fst()
	{
	}

	bool fst::RemoveState(StateId n)
	{
		this->states.erase(states.begin() + n);

		return true;
	}

	StateId fst::AddState()
	{
		this->states.emplace_back();
		return this->states.size() - 1;
	}



	bool fst::SetStart(StateId pos)
	{
		this->startId = pos;
		return true;
	}

	bool fst::SetFinal(StateId pos)
	{
		this->finalId = pos;
		return true;
	}

	bool fst::AddArc(StateId s, Arc &arc)
	{
		this->states[s].arcs.push_back(arc);
		return true;
	}

	bool fst::WriteText(const char* filename)
	{
		std::ofstream ofile(filename);

		StateId src_state_id = 0;
		for (auto & state_it : states)
		{
			for (auto & arc_it : state_it.arcs)
			{
				ofile << src_state_id << '\t' << arc_it.nextstate << '\t' << arc_it.ilabel << '\t' << arc_it.olabel << '\t'<<arc_it.weight<<'\n';
			}
			src_state_id++;
		}
		ofile << finalId << '\t' << 0 << std::endl;
		return true;
	}

	bool fst::ReadText(const char* filename)
	{

		////UNDER CONSTRUCTION///
		std::cout << "fst::ReadText not yet supported." << std::endl;
		return 0;
		/////////////////////////
		
		std::ifstream ifile(filename);
		Symbol line;
		std::vector<Symbol> parts;
		bool bStoreSymbolWithFst = true;
		int nLineCounter=0;
// 		SymbolTable

		getline(ifile, line);
		nLineCounter++;
		split(line, parts, " \t");

// 		if (isNum(parts[3]) && isNum(parts[4])) //if pure digit
// 		{
// 			bStoreSymbolWithFst = false;
// 		}

		while (!ifile.eof())
		{

			getline(ifile, line);
			nLineCounter++;
			split(line, parts, " \t");
			
			

			if (parts.size()==5)	//if arc
			{
				if (bStoreSymbolWithFst)
				{

				}
				else
				{

				}
			}
			else if (parts.size()==2)	//if state
			{
					
			}
			else	//anything else
			{
				std::cout << "Parsing error at " << filename << ':' << nLineCounter << std::endl;
			}
		}
	}

	void fst::Minimize()
	{
		throw std::logic_error("The minimize operation is not implemented.");
	}

	void fst::Determinize()
	{
		throw std::logic_error("The determinize operation is not implemented.");
	}

	size_t fst::NumStates() const
	{
		return states.size();
	}

	bool fst::RemoveArc(StateId s, size_t n)
	{
		return states[s].RemoveArc(n);
	}

}