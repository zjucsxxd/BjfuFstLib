#include "WFST.h"

void WFST::SaveCFastLM(const char * filename)
{
	CFastLM fastlm;
	fastlm.fastNodeNum = _fst.NumStates();
	fastlm.pNodeArray = new CFastNode[fastlm.fastNodeNum];
	fastlm.fastEdgeNum = 0;
	for (int s = 0; s < _fst.states.size(); s++)
	{
		fastlm.fastEdgeNum += _fst.states[s].arcs.size();
	}
	fastlm.pEdgeArray = new CFastEdge[fastlm.fastEdgeNum];

	int edge_pos = 0;
	for (int s = 0; s < _fst.states.size(); s++)
	{
		State & it_state = _fst.states[s];
// 		fastlm.fastEdgeNum += it_state.arcs.size();
		fastlm.pNodeArray[s].edgeStart = edge_pos;
		fastlm.pNodeArray[s].edgeCount = it_state.arcs.size();
		fastlm.pNodeArray[s].best = 0;
		for (int a = 0; a < it_state.arcs.size(); a++)
		{
			Arc & it_arc = it_state.arcs[a];
			fastlm.pEdgeArray[edge_pos].idata = it_arc.ilabel;
			fastlm.pEdgeArray[edge_pos].odata = it_arc.olabel;
			fastlm.pEdgeArray[edge_pos].nodeTo = it_arc.nextstate;
			fastlm.pEdgeArray[edge_pos].weight = it_arc.weight;
			if (it_arc.weight>fastlm.pNodeArray[s].best)
			{
				fastlm.pNodeArray[s].best = it_arc.weight;
			}
			edge_pos++;
		}
	}

	int offset = 0;
	int offset_phones;//offset of words,i.e. fastlm.pWords=fastlm.textbuffer[offset_words]


	fastlm.nWords = this->_osymbol.Size();
	char ** pWords = new char *[fastlm.nWords];//potential bug due to noncontinuous label.
	for (int i = 0; i < this->_osymbol.Size(); i++)//obtaining raw strings into pWords, meanwhile counting offset.
	{
		Symbol osymbol = _osymbol.Find(i);
		pWords[i] = new char[osymbol.size() + 1];
		offset += osymbol.size() + 1;
		strcpy(pWords[i], osymbol.c_str());
	}

	offset_phones = offset;
	fastlm.nPhons = this->_isymbol.Size();
	char ** pPhons = new char *[fastlm.nPhons];//potential bug due to noncontinuous label.
	for (int i = 0; i < this->_isymbol.Size(); i++)
	{
		Symbol isymbol = _isymbol.Find(i);
		pPhons[i] = new char[isymbol.size()+1];
		strcpy(pPhons[i], isymbol.c_str());
		offset += isymbol.size() + 1;
	}
	fastlm.textbuffer = new char[offset];

	offset = 0;


	fastlm.pWords = new char *[fastlm.nWords];
	for (int i = 0; i < fastlm.nWords; i++)
	{
		strcpy(fastlm.textbuffer + offset, pWords[i]);//saving raw string into textbuffer
		fastlm.pWords[i] = fastlm.textbuffer + offset;//pointing fastlm.pWords to textbuffer.
		offset += strlen(pWords[i])+1;
	}

	fastlm.pPhons = new char *[fastlm.nPhons];
	for (int i = 0; i < fastlm.nPhons; i++)
	{
		strcpy(fastlm.textbuffer + offset, pPhons[i]);//saving raw string into textbuffer
		fastlm.pPhons[i] = fastlm.textbuffer + offset;
		offset += strlen(pPhons[i])+1;
	}

	fastlm.fastStNode = fastlm.fastEdNode = 0;//temporarily define start&end node at ZERO.

	fastlm.fastStNode = this->_fst.startId;
	fastlm.fastEdNode = this->_fst.finalId;

	for (int i = 0; i < fastlm.nPhons;i++)
	{
		delete pPhons[i];
	}
	delete pPhons;

	for (int i = 0; i < fastlm.nWords; i++)
	{
		delete pWords[i];
	}
	delete pWords;

	fastlm.SaveFastLM(filename);
}

bool WFST::LoadFST(const char * filename)
{
	return _fst.ReadText(filename);
}

bool WFST::SaveFST(const char * filename)
{
	this->_isymbol.WriteText((fstname + "isymbs.txt").c_str());
	this->_osymbol.WriteText((fstname + "osymbs.txt").c_str());
	this->_ssymbol.WriteText((fstname + "ssymbs.txt").c_str());

	return _fst.WriteText(filename);
}

void WFST::Minimize()
{
	_fst.Minimize();
}

void WFST::Determinize()
{
	_fst.Determinize();
}

void WFST::Draw(const char * filename)
{
	_fst.Draw(filename, _isymbol, _osymbol);
}

std::map<StateId, std::vector<Arc_Pos> > WFST::updateArcIn()
{
	std::map<StateId, std::vector<Arc_Pos> > retval;

	for (int i = 0; i < _fst.states.size(); i++)
	{
		auto& ref_state = _fst.states[i];
		for (int j = 0; j < ref_state.arcs.size(); j++)
		{
			auto& ref_arc = ref_state.arcs[j];
			retval[ref_arc.nextstate].push_back(Arc_Pos(i, j));
		}
	}
	return retval;
}
