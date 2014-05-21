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

	struct Arc_Pos
	{
		Arc_Pos(StateId s, size_t a)
			:s(s), a(a)
		{}
		StateId s;//state
		size_t a;//arc position

	};
	struct Arc_Pos_Compare
	{
		bool operator ()(const Arc_Pos& lhs, const Arc_Pos& rhs) const
		{
			if (lhs.s < rhs.s)
			{
				return true;
			} 
			if (lhs.s > rhs.s)
			{
				return false;
			} 
			if (rhs.a < rhs.a)
			{
				return true;
			}
			if (rhs.a > rhs.a)
			{
				return false;
			}
			return false;

		}
	};



}
