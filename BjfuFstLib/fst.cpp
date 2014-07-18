#include "fst.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include "FastLM.h"

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
		this->states.push_back(State());
		//this->states.emplace_back();
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
		for (int s = 0; s < states.size(); s++)//(auto & state_it : states)
		{
			State & state_it = states[s];
			for (int a = 0; a < state_it.arcs.size();a++)//(auto & arc_it : state_it.arcs)
			{
				Arc & arc_it = state_it.arcs[a];
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
		for (int s = 0; s < states.size(); s++)//(auto & state_it : states)
		{
			State & state_it = states[s];
			for (int a = 0; a < state_it.arcs.size(); a++)//(auto & arc_it : state_it.arcs)
			{
				Arc & arc_it = state_it.arcs[a];
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

	bool fst::Draw(const char* filename, SymbolTable & isymbs, SymbolTable & osymbs, const bool print_symbols /*= true*/)
	{
		std::ofstream ofile(filename);
		ofile << "digraph G{\n";
		ofile << "edge [fontname=\"Î¢ÈíÑÅºÚ\"];\n";



		StateId src_state_id = 0;
		for (int s = 0; s < states.size(); s++)//(auto & state_it : states)
		{
			State & state_it = states[s];
			for (int a = 0; a < state_it.arcs.size(); a++)//(auto & arc_it : state_it.arcs)
			{
				Arc & arc_it = state_it.arcs[a];
				// ofile << src_state_id << '\t' << arc_it.nextstate << '\t' << isymbs.Find(arc_it.ilabel) << '\t' << osymbs.Find(arc_it.olabel) << '\t' << arc_it.weight << '\n';
				if (print_symbols)
				{
					ofile << src_state_id << " -> " << arc_it.nextstate << " [label=\"" << arc_it.ilabel << '.' << isymbs.Find(arc_it.ilabel) << ':' << arc_it.ilabel << '.'<< osymbs.Find(arc_it.olabel) << ',' << arc_it.weight << "\"];\n";
				}
				else
				{
					ofile << src_state_id << " -> " << arc_it.nextstate << " [label=\"" << arc_it.ilabel << ':' << arc_it.olabel << ',' << arc_it.weight << "\"];\n";
				}
			}
			src_state_id++;
		}
		ofile << '}'<<std::endl;
		return true;

	}

	void fst::Rmepsilon()
	{
		//TODO : fill the operation
	}

	void fst::Triphone()
	{

	}

	Arc& fst::findArc(const Arc_Pos & pos)
	{
		return states[pos.s].arcs[pos.a];
	}

}