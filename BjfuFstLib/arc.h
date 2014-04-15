#pragma once
#include "FstConventions.h"

namespace bjfufst{

	class Arc
	{
	public:
		Arc();
		virtual ~Arc();

		//standard constructor
		Arc(Label i, Label o, Weight w, StateId s);

		//input label of arc
		Label ilabel;

		//output label of arc
		Label olabel;

		//weight of arc
		Weight weight;

		//id of next state
		StateId nextstate;
	};

}