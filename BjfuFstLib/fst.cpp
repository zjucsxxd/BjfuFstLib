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
// 		std::cout << "fst::ReadText not yet supported." << std::endl;
// 		return 0;
		/////////////////////////
		
		std::ifstream ifile(filename);
		std::string line;
		std::vector<std::string> parts;
		bool bStoreSymbolWithFst = true;
		int nLineCounter = 0;
		int nMaxStateId = 0;
// 		SymbolTable


		while (!ifile.eof())//iterate over the file to find max state id.
		{
			getline(ifile, line);
			split(line, parts, " \t");
			if (parts.size()>2)
			{
				StateId S1, S2;
				S1 = atoi(parts[0].c_str());
				S2 = atoi(parts[1].c_str());
				
				if (S1>nMaxStateId)
				{
					nMaxStateId = S1;
				}
				if (S2>nMaxStateId)
				{
					nMaxStateId = S2;
				}
			}
		}

		for (int i = 0; i <= nMaxStateId; i++)//Create states first
		{
			AddState();
		}

		ifile.close();

		ifile.open(filename);//read the file over

		while (!ifile.eof())//and fill arcs later.
		{

			getline(ifile, line);
			nLineCounter++;
			split(line, parts, " \t");
			StateId S1, S2;
			Label IA, OA;
			float cost;
			

			if (parts.size()==4)	//if unweighted arc
			{
				S1 = atoi(parts[0].c_str());
				S2 = atoi(parts[1].c_str());
				IA = atoi(parts[2].c_str());
				OA = atoi(parts[3].c_str());
				cost = 1.0;//Cost is fst::Weight::One() by default. In this case, 1.0
				AddArc(S1, Arc(IA, OA, cost, S2));
			}
			else if (parts.size() == 5)//if weighted arc
			{
				S1 = atoi(parts[0].c_str());
				S2 = atoi(parts[1].c_str());
				IA = atoi(parts[2].c_str());
				OA = atoi(parts[3].c_str());
				cost = atof(parts[4].c_str());
				AddArc(S1, Arc(IA, OA, cost, S2));
			}
			else if (parts.size()<=2)	//if state
			{
				//should add weight of final state here, not yet needed.
			}
			else	//anything else
			{
				std::cout << "Parsing error at " << filename << ':' << nLineCounter << std::endl;
			}
		}
		return 0;
	}

	bool fst::ReadText(const char* filename, SymbolTable & isymbs, SymbolTable & osymbs)
	{

		////UNDER COSNTRUCTION////
		std::cout << "fst::ReadText(const char* filename, SymbolTable & isymbs, SymbolTable & osymbs) not yet supported." << std::endl;
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
		ofile << "edge [fontname=\"΢���ź�\"];\n";



		StateId src_state_id = 0;
		for (int s = 0; s < states.size(); s++)//(auto & state_it : states)
		{
			State state_it = states[s];
			for (int a = 0; a < state_it.arcs.size(); a++)//(auto & arc_it : state_it.arcs)
			{
				Arc arc_it = state_it.arcs[a];
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

	std::vector<StateId> fst::eps_closure(StateId stateid)
{
		//TODO
// 		std::set<Arc_Pos> eps_arcpos_iterated;
		std::vector<StateId> stateid_ret;
		stateid_ret.push_back(stateid);
		for (size_t i = 0; i < stateid_ret.size(); i++)
		{
			StateId s = stateid_ret[i];
			State this_state = states[s];
			for (size_t a = 0; a < this_state.arcs.size();a++)
			{
				Arc_Pos this_arcpos(s, a);
				Arc this_arc = findArc(this_arcpos);
				if (this_arc.ilabel==0)//if eps arc
				{
// 					if (eps_arcpos_iterated.find(this_arcpos)==eps_arcpos_iterated.end())//and the arc not iterated
					{
						//and the state not in the vector
						if (std::find(stateid_ret.begin(), stateid_ret.end(), this_arc.nextstate) == stateid_ret.end())
						{
							stateid_ret.push_back(this_arc.nextstate);//insert the next state of the arc into set for further discovering.
						}
					}
					
				}

			}
		}
		return  stateid_ret;

	}

}