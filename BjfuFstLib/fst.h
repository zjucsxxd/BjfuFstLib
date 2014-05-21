#pragma once
#include "state.h"
#include "arc.h"
#include <vector>
#include "FstConventions.h"
#include "SymbolTable.h"

namespace bjfufst{



	class fst
	{
	public:

		fst();
		virtual ~fst();
		//constructor from text fst file as follow:
		//arc format : src dest ilabel olabel[weight]
		//final state format : state[weight]
		//lines may occur in any order except initial state must be first line
		//un specified weights default to 0.0 (for the library - default Weight type)
		//	1 a x .5
		//	0 1 b y 1.5
		//	1 2 c z 2.5
		//	2 3.5
		fst(const char* filename);

		//add a state to the end. 
		StateId AddState();

		//remove nth state 
		bool RemoveState(StateId n);

		//set the unique start state.
		bool SetStart(StateId pos);

		//set the unique final state
		bool SetFinal(StateId pos);

		//add an arc from state s
		bool AddArc(StateId s, Arc &arc);

		//remove nth arc on state s
		bool RemoveArc(StateId s, size_t n);

		//write fst to text
		bool WriteText(const char* filename);
		//write fst to text, appending symbols.
		bool WriteText(const char* filename,const SymbolTable & isymbs, const SymbolTable & osymbs);

		//read fst from text.
		bool ReadText(const char* filename);
		//read fst from text, modifying i/o symbs.
		bool ReadText(const char* filename, SymbolTable & isymbs, SymbolTable & osymbs);

		//Minimizing operation
		void Minimize();

		//Determinizing operation
		void Determinize();

		//Removing epsilon arcs and corresponding states.
		void Rmepsilon();

		//Triphone expanding operation.
		void Triphone();

		//number of states
		size_t NumStates() const;


		//draw the fst in .dot format.
		bool Draw(const char* filename, SymbolTable & isymbs, SymbolTable & osymbs, const bool print_symbols = true);

		//get an arc from position
		Arc& findArc(const Arc_Pos & pos);



	public:


		//container of states
		std::vector<State> states;

		//id of start and final.
		StateId startId, finalId;
	};

}