#include "state.h"

namespace bjfufst{

	State::State()
	{
	}


	State::~State()
	{
	}

	bool State::RemoveArc(size_t n)
	{
		this->arcs.erase(arcs.begin() + n);
		return true;
	}
}