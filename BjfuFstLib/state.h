#pragma once
#include<vector>
#include "arc.h"
#include "FstConventions.h"
namespace bjfufst{

	class State
	{
	public:
		State();
		virtual ~State();

		//add an arc to the state.
		bool AddArc(const Arc& arc);

		//remove the nth arc.
		bool RemoveArc(size_t n);

		//arcs from the states
		std::vector<Arc> arcs;
	};

}