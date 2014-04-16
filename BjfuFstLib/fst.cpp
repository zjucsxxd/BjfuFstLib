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

	bool fst::WriteText(const char* filename, const SymbolTable & isymbs, const SymbolTable & osymbs)
	{
		std::ofstream ofile(filename);

		StateId src_state_id = 0;
		for (auto & state_it : states)
		{
			for (auto & arc_it : state_it.arcs)
			{
				ofile << src_state_id << '\t' << arc_it.nextstate << '\t' << isymbs.Find(arc_it.ilabel) << '\t' << osymbs.Find(arc_it.olabel) << '\t' << arc_it.weight << '\n';
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
		std::string line;
		std::vector<std::string> parts;
		bool bStoreSymbolWithFst = true;
		int nLineCounter=0;
// 		SymbolTable


		while (!ifile.eof())
		{
			getline(ifile, line);
		}




		getline(ifile, line);
		nLineCounter++;
		split(line, parts, " \t");


		while (!ifile.eof())
		{

			getline(ifile, line);
			nLineCounter++;
			split(line, parts, " \t");
			
			

			if (parts.size()==5)	//if arc
			{
				
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

	bool fst::ReadText(const char* filename, SymbolTable & isymbs, SymbolTable & osymbs)
	{

		////UNDER COSNTRUCTION////
		std::cout << "fst::ReadText not yet supported." << std::endl;
		return 0;
		///////////////////////////
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

	bool fst::Draw(const char* filename, SymbolTable & isymbs, SymbolTable & osymbs)
	{
		std::ofstream ofile(filename);
		ofile << "digraph G{\n";
		ofile << "edge [fontname=\"Î¢ÈíÑÅºÚ\"];\n";



		StateId src_state_id = 0;
		for (auto & state_it : states)
		{
			for (auto & arc_it : state_it.arcs)
			{
// 				ofile << src_state_id << '\t' << arc_it.nextstate << '\t' << isymbs.Find(arc_it.ilabel) << '\t' << osymbs.Find(arc_it.olabel) << '\t' << arc_it.weight << '\n';
				ofile << src_state_id << " -> " << arc_it.nextstate << " [label=\"" << isymbs.Find(arc_it.ilabel) << ':' << osymbs.Find(arc_it.olabel) << ',' << arc_it.weight << "\"];\n";
			}
			src_state_id++;
		}
		ofile << '}'<<std::endl;
		return true;

	}

}