#include "arc.h"

namespace bjfufst{

	Arc::Arc()
	{
	}

	Arc::Arc(Label i, Label o, Weight w, StateId s)
		:ilabel(i), olabel(o), weight(w), nextstate(s)
	{

	}


	Arc::~Arc()
	{
	}

}