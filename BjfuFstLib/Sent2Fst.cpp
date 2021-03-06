#include "Sent2Fst.h"

bool Sent2Fst::Sent2WordFST(const char * sent, WFST * wfst, const char * options/*=""*/)
{
	vstr parts;	//a vector<string> that saves all separated words in a sentence.
	split(sent, parts, " ");
	fst& wordFst = wfst->_fst;
	SymbolTable& iSymbol = wfst->_isymbol;
	SymbolTable& oSymbol = wfst->_osymbol;
	bjfufst::StateId prev_state_id = wordFst.AddState(), next_state_id=0;
	StateId eps_state_id = prev_state_id;
	StateId eps_label = iSymbol.AddSymbol("<eps>");
	oSymbol.AddSymbol("<eps>");
	wordFst.SetStart(prev_state_id);	//add and set the first state.
	bool bIsFirst = true;	//flag indicating first word. First w

	bool bLinearFst = !strcmp(options, "linear");//check if use simple fst network,without eps.



	for (int i = 0; i < parts.size();i++)//(auto it : parts)
	{
		std::string it = parts[i];
		next_state_id = wordFst.AddState();	//create next state
		Arc temp(iSymbol.AddSymbol(it), oSymbol.AddSymbol(it), g_ForwardArcWeight, next_state_id);
		wordFst.AddArc(prev_state_id, temp);	//and next arc.
		if (bLinearFst)//create no eps arcs if bLinearFst is given.
		{
			prev_state_id = next_state_id;

			continue;
		}
		Arc temp1(iSymbol.AddSymbol("<eps>"), oSymbol.AddSymbol("<eps>"), g_BackoffArcWeight, eps_state_id);
		wordFst.AddArc(next_state_id,temp1);	//and backoff arc to eps state.
		if (bIsFirst)
			bIsFirst = false;
		else{
			Arc temp2(iSymbol.AddSymbol(it), oSymbol.AddSymbol(it), g_ForwardArcWeight, next_state_id);
			wordFst.AddArc(eps_state_id, temp2);	//and forwarding arc from eps state.
			}

		Arc temp3(iSymbol.AddSymbol("sil"), oSymbol.AddSymbol("<eps>"), g_BackoffArcWeight, next_state_id);
		wordFst.AddArc(next_state_id, temp3);	//and self-ring arc.

		prev_state_id = next_state_id;
	}
	wordFst.SetFinal(next_state_id);

	return false;
}

bool Sent2Fst::LoadLexDic(const char * filename)
{
	std::ifstream lexFile(filename);

	if (!lexFile)
	{
		std::cout << "Warning! Cannot find " << filename << std::endl;
	}
	Symbol line;
	std::vector<Symbol> parts;
	Symbol word;
	std::string lexline;
	while (getline(lexFile, line, '\n'))
	{
		split(line, parts, " ");
		word = parts[0];
		lexline = line.substr(word.size()+1, line.size());
		lexDict.addLex(word,lexline);
		parts.erase(parts.begin());
		phone_list.insert(parts.begin(), parts.end());

	}

	return false;
}

bool Sent2Fst::WordFST2PhoneFST(const WFST * wordFST, WFST * phoneFST)
{
	//need to clear phoneFst first

	phoneFST->_fst = wordFST->_fst;	//copy from wordFst
	phoneFST->_osymbol = wordFST->_osymbol;
	fst & wfst = phoneFST->_fst;
	int wordFst_state_number = wordFST->_fst.NumStates();	//save number of old states. 区分新老状态。
	for (int i = 0; i < wordFst_state_number; i++)//iterate over all states
	{
		int nArc = wfst.states[i].arcs.size();
		for (int j = 0; j < nArc; j++)//iterate over all arcs of the state
		{
			Arc & arc = wfst.states[i].arcs[j];
			Label ilbl = arc.ilabel;
			Label olbl = arc.olabel;
			Symbol oWord = wordFST->_osymbol.Find(ilbl);

			if (oWord == "<eps>" || oWord == "eps")
			{
				//				wfst.DeleteArcs(i, 1);
				continue;
				//				arc.nextstate = wfst.AddArc();
			}
			if (oWord != "" && ilbl==olbl)//if arc not replaced yet and the arc is an acceptor arc 
			{
				//TODO:remove old arc 
				//do the replacement.
				Arc2LexFst(i, j, oWord, lexDict[oWord], phoneFST);
				wfst.RemoveArc(i, j);
				j--;
				nArc--;
				//				Arc2LexFst(oWord, i, lexDict[oWord], arc.nextstate, phoneFST);
				// 				wfst.RemoveArc(i, nArc);
				// 				wordFst_state_number--;
				// 				j--;
			}
			else
			{
// 				nArc++;// TODO:?
			}
		}
	}

	return 0;
}

bool Sent2Fst::PhoneFST2TriphoneFST(const WFST * phoneFST, WFST * triphoneFST)
{
	//TODO:LR-Biphone triphone constructor.
	triphoneFST->_fst = phoneFST->_fst;
	triphoneFST->_ssymbol = phoneFST->_ssymbol;
	triphoneFST->_isymbol = phoneFST->_isymbol;
	triphoneFST->_osymbol = phoneFST->_osymbol;
	//copying original fst

	fst &fst_Triphone = triphoneFST->_fst;
	SymbolTable &con_symbs = triphoneFST->_isymbol;
	SymbolTable &lex_symbs = triphoneFST->_osymbol;
	SymbolTable &c_state_symbs = triphoneFST->_ssymbol;


	//step1:L-Biphone expanding. No labeling yet.

	//map of state-iarc
	std::map<StateId, std::vector<Arc_Pos> > arcs_in = triphoneFST->updateArcIn();
	for (int i = 0; i < fst_Triphone.NumStates(); i++)//iterating over all states, expanding all corresponding arcs and states.
	{
		std::vector<Arc_Pos> &arcin = arcs_in[i];
		if (arcin.empty())
		{
			std::cout << "warning: state " << i << " has no arcin." << std::endl;
			continue;
		}

		if (arcin.size()>1)//try expanding if more than one arcin exists.
		{
			std::map<Symbol, std::vector<Arc_Pos> > prev_string2id;//建立<字符串,弧ID数组>的映射，对应多组入弧
			for (int j = 0; j < arcin.size(); j++)
			{
				Symbol thisarcinput = con_symbs.Find(fst_Triphone.findArc(arcin[j]).ilabel);
				prev_string2id[thisarcinput].push_back(arcin[j]);
			}
			if (prev_string2id.size()>1)//expand of differs.
			{
				std::map<Symbol, std::vector<Arc_Pos> >::iterator it = prev_string2id.begin(); //初始化map的迭代器it
				it++;//跳过第一组入弧，让它们和原来的状态连着就好
				for (; it != prev_string2id.end(); it++) //依次处理各组弧
				{
					std::vector<Arc_Pos> &prev_ids = it->second; //引用出一组同输入的Arc_Pos元素列表
					int newstate_id = fst_Triphone.AddState();//分裂出新状态并且记下其ID
					arcs_in[newstate_id] = prev_ids;//顺便在arcs_in列表中加入此新状态的入弧


					for (int k = 0; k < prev_ids.size(); k++) //链接本组同输入的弧和新状态
					{
						Arc & in_arc = fst_Triphone.findArc(prev_ids[k]);
						in_arc.nextstate = newstate_id;
						// 						fst_Triphone.AddArc(prev_ids[k].s,Arc(in_arc.ilabel,in_arc.olabel,in_arc.weight,newstate_id) );
					}
					//linking new state with following states.
					fst_Triphone.states[newstate_id].arcs = fst_Triphone.states[i].arcs;
				}
			}
		}
	}
	//step2:R-Biphone expanding.
	arcs_in = triphoneFST->updateArcIn();
	for (int i = 0; i < fst_Triphone.NumStates(); i++)//iterating over all states, expanding all corresponding arcs and states.
	{
		State &it_state = fst_Triphone.states[i];
		std::vector<Arc> &arcout = it_state.arcs;
		if (arcout.empty())
		{
			std::cout << "warning: state " << i << " has no arcout." << std::endl;
			continue;
		}

		if (arcout.size()>1)//try expanding if more than one arcin exists.
		{
			std::map<Symbol, std::vector<Arc> > next_string2id;//建立<字符串,弧ID数组>的映射，对应多组入弧
			for (int j = 0; j < arcout.size(); j++)
			{
				Symbol thisarcinput = con_symbs.Find(arcout[j].ilabel);
				next_string2id[thisarcinput].push_back(arcout[j]);
			}
			if (next_string2id.size()>1)//expand of differs.
			{
				std::map<Symbol, std::vector<Arc> >::iterator it = next_string2id.begin(); //初始化map的迭代器it


				it_state.arcs = it->second;
				it++;//跳过第一组出弧，让它们和原来的状态连着就好

				for (; it != next_string2id.end(); it++) //依次处理各组弧
				{
					std::vector<Arc> &next_ids = it->second; //引用出一组同输入的Arc_Pos元素列表
					int newstate_id = fst_Triphone.AddState();//分裂出新状态并且记下其ID
					// 					arcs_in[newstate_id] = next_ids;//顺便在arcs_in列表中加入此新状态的入弧
					fst_Triphone.states[newstate_id].arcs = next_ids;
					arcs_in = triphoneFST->updateArcIn();//TODO:可以优化掉？
					std::vector<Arc_Pos> & in_arcs = arcs_in[i];
					for (int k = 0; k < in_arcs.size(); k++) //链接本组同输入的弧和新状态
					{
						Arc_Pos & in_arc_pos = in_arcs[k];
						Arc in_arc = fst_Triphone.findArc(in_arc_pos);
						Arc temp4(in_arc.ilabel, in_arc.olabel, in_arc.weight, newstate_id);
						fst_Triphone.AddArc(in_arc_pos.s, temp4);
					}
				}
			}
		}
	}

	//step 3: iterate over arcs and generate triphone.
	arcs_in = triphoneFST->updateArcIn();
	std::map<Arc_Pos, Label, Arc_Pos_Compare> monoiLabel;
	for (StateId s = 0; s < fst_Triphone.states.size(); s++)
	{
		State & it_state = fst_Triphone.states[s];
		for (size_t a = 0; a < it_state.arcs.size(); a++)
		{
			Arc &it_arc = it_state.arcs[a];
			monoiLabel[Arc_Pos(s, a)] = it_arc.ilabel;
		}
	}//keeping old monophone iLabel

	Label lbl_prev, lbl_curr, lbl_next;
	size_t states_size, arcs_size;

	states_size = fst_Triphone.states.size();
	for (StateId s = 0; s < states_size ; s++)
	{
		State it_state = fst_Triphone.states[s];
		arcs_size = it_state.arcs.size();
		for (size_t a = 0; a < arcs_size; a++)
		{
			Arc it_arc = it_state.arcs[a];
			lbl_curr = monoiLabel[Arc_Pos(s, a)];
			if (lbl_curr == EPS) //no triphone for eps arc.
			{
				//TODO: rewrite eps operation.
				continue;
			}

			if (arcs_in[s].size() > 0)//find prev lbl
			{
				lbl_prev = monoiLabel[arcs_in[s][0]];

				if (lbl_prev == EPS)//no triphone for arcs close to eps arc.
				{
					continue;
				}
				// 				Arc_Pos it_arc_pos(s, a);
				// 				while (lbl_prev == EPS)
				// 				{
				// 					if (arcs_in[s].size() == 0)
				// 					{
				// 						lbl_prev = 0;
				// 						break;
				// 					}
				// 					it_arc_pos = arcs_in[it_arc_pos.s][0]; //iterate to prev arc.
				// 					lbl_prev = monoiLabel[it_arc_pos];
				// 				}
			}
			else
			{
				lbl_prev = 0;
			}

			
			if (fst_Triphone.states[it_arc.nextstate].arcs.size() > 0)//find next lbl
			{
				lbl_next = monoiLabel[Arc_Pos(it_arc.nextstate, 0)];

				if (lbl_next==EPS)//no triphone for arcs close to eps arc.
				{

//					continue;
					//make two arcs that jump over the arc and succeeding non-eps arc .
					std::vector<StateId> closure_states = fst_Triphone.eps_closure(it_arc.nextstate);
					for (std::vector<StateId>::iterator it_closure_state_id = closure_states.begin();
						it_closure_state_id != closure_states.end();
						it_closure_state_id++)
					{
						//add the arcs and state
						//---->X---x-a+b---->A----a-b+y---->B---->Y
						lbl_next = fst_Triphone.states[*it_closure_state_id].arcs[0].ilabel;
						StateId newState = fst_Triphone.AddState();
						Symbol triphone_symbol_eps;
						triphone_symbol_eps=
							triphoneFST->_isymbol.Find(lbl_prev)+
							triphoneFST->_isymbol.Find(lbl_curr)+
							triphoneFST->_isymbol.Find(lbl_next);
						Arc newarc;
						newarc = Arc(
							triphoneFST->_isymbol.AddSymbol(triphone_symbol_eps),
							it_arc.olabel,
							it_arc.weight,
							newState
							);
						fst_Triphone.AddArc(s,newarc);

						triphone_symbol_eps =
							triphoneFST->_isymbol.Find(lbl_curr) +
							triphoneFST->_isymbol.Find(lbl_next) +
							triphoneFST->_isymbol.Find(fst_Triphone.states[fst_Triphone.states[*it_closure_state_id].arcs[0].nextstate].arcs[0].ilabel);
						
						newarc = Arc(
							triphoneFST->_isymbol.AddSymbol(triphone_symbol_eps),
							lbl_next,
							it_state.arcs[0].olabel,
							fst_Triphone.states[*it_closure_state_id].arcs[0].nextstate
							);

						fst_Triphone.AddArc(newState,newarc);

					}

				}

// 				Arc_Pos it_arc_pos(s, a);
// 				while (lbl_next == 0)
// 				{
// 					it_arc_pos = Arc_Pos(fst_Triphone.findArc(it_arc_pos).nextstate, 0); //iterate to next arc.
// 					lbl_next = monoiLabel[it_arc_pos];
// 					if (fst_Triphone.states[fst_Triphone.findArc(it_arc_pos).nextstate].arcs.size() == 0)
// 					{
// 						lbl_next = 0;
// 						break;
// 					}
// 
// 				}
			}
			else
			{
				lbl_next = 0;
			}







			Symbol triphone_symbol = triphoneFST->_isymbol.Find(lbl_curr);//relabeling phone->triphone
			if (lbl_prev != 0)
			{
				triphone_symbol = triphoneFST->_isymbol.Find(lbl_prev) + '-' + triphone_symbol;
			}
			if (lbl_next != 0)
			{
				triphone_symbol = triphone_symbol + '+' + triphoneFST->_isymbol.Find(lbl_next);
			}
			it_arc.ilabel = triphoneFST->_isymbol.AddSymbol(triphone_symbol);
		}
	}

#ifdef DEBUG
	triphoneFST->Draw("triphone_notiedlist.dot");
#endif // DEBUG




	for (StateId s = 0; s < fst_Triphone.states.size(); s++)
	{
		State & it_state = fst_Triphone.states[s];
		for (size_t a = 0; a < it_state.arcs.size(); a++)
		{
			Arc &it_arc = it_state.arcs[a];
			std::map < std::string, std::string >::iterator it_map = mapTiedList.find(con_symbs.Find(it_arc.ilabel));
			if (it_map != mapTiedList.end())
			{
				it_arc.ilabel = con_symbs.AddSymbol(it_map->second);
			}
		}
	}



	return 0;
}


void Sent2Fst::Arc2LexFst(int iState, int idArc, Symbol oWord, vvstr lexs, WFST * wfst)
{
	//locate arc first
	fst &thefst = wfst->_fst;
	Arc thearc = thefst.states[iState].arcs[idArc];
	StateId oState = thearc.nextstate;

	for (int i = 0; i < lexs.size();i++)//(auto lex : lexs)//support multi-tone
	{
		std::vector<std::string> lex = lexs[i];
		int prev_state_id = iState;
		int new_state_id;
		std::vector<std::string>::iterator it_Phone = lex.begin();
		for (; it_Phone != lex.end() - 1; it_Phone++)
		{
			//insert state o
			new_state_id = thefst.AddState();
			//and arc to the new state ->o
			Arc temp5(wfst->_isymbol.AddSymbol(*it_Phone), wfst->_osymbol.AddSymbol("<eps>"), 0, new_state_id);
			thefst.AddArc(prev_state_id, temp5);
			prev_state_id = new_state_id;
#ifdef _DEBUG
			wfst->Draw("debug.dot");
#endif // _DEBUG

		}
		//finally add the arc with output to the oState. o->o->o
		Arc temp6(wfst->_isymbol.AddSymbol(*it_Phone), wfst->_osymbol.AddSymbol(oWord), thearc.weight, oState);
		thefst.AddArc(prev_state_id, temp6);
#ifdef _DEBUG
		wfst->Draw("debug.dot");
#endif // _DEBUG

	}
}

bool Sent2Fst::LoadTiedList(const char * filename)
{
	//tied-list processing.
	std::ifstream ifTiedList("tiedlist");
	if (!ifTiedList)
	{
		std::cout << "Can't open file tiedlist!" << std::endl;
		exit(0);
	}
	std::string line;
	std::vector < std::string >  parts;
	while (getline(ifTiedList, line))
	{
		split(line, parts, " ");
		if (parts.size() != 2)
		{
			continue;
		}
		mapTiedList[parts[0]] = parts[1];
	}
	return EXIT_SUCCESS;
}

// void Sent2Fst::Arc2LexFst(Symbol oWord, int iState, vvstr lexs, int oState, WFST * wfst)
// {
// 	//locate arc first
// 	fst &thefst = wfst->_fst;
// #ifdef _DEBUG
// 	wfst->Draw("debug.dot");
// #endif // _DEBUG
// 
// 	for (auto lex : lexs)
// 	{
// 		int prev_state_id = iState;
// 		int new_state_id;
// 		auto it_Phone = lex.begin();
// 		for (; it_Phone != lex.end() - 1; it_Phone++)
// 		{
// 			//insert state .
// 			new_state_id = thefst.AddState();
// 			//and arc to the new state ->
// 			thefst.AddArc(prev_state_id, Arc(wfst->_isymbol.AddSymbol(*it_Phone), wfst->_osymbol.AddSymbol("<eps>"), 0, new_state_id));
// 			prev_state_id = new_state_id;
// #ifdef _DEBUG
// 			wfst->Draw("debug.dot");
// #endif // _DEBUG
// 
// 		}
// 		//finally add the arc with output to the oState.
// 		thefst.AddArc(prev_state_id, Arc(wfst->_isymbol.AddSymbol(*it_Phone), wfst->_osymbol.AddSymbol(oWord), 0, oState));
// #ifdef _DEBUG
// 		wfst->Draw("debug.dot");
// #endif // _DEBUG
// 
// 	}
// }


/*
bool Sent2Fst::PhoneFST2TriphoneFST(const WFST * phoneFST, WFST * triphoneFST)

////dict-based triphone constructor
auto &fst_Triphone=triphoneFST->_fst;
auto &con_symbs = triphoneFST->_isymbol;
auto &lex_symbs = triphoneFST->_osymbol;
auto &c_state_symbs = triphoneFST->_ssymbol;
phone_list.erase("sil");

//step0:generate eps-sil part.
con_symbs.AddSymbol("<eps>");
con_symbs.AddSymbol("sil");
// 	con_symbs.AddSymbol("<s>");
// 	con_symbs.AddSymbol("</s>");
c_state_symbs.Clear();
fst_Triphone.AddState();
c_state_symbs.AddSymbol("eps");//add the eps state, as the begin state.
fst_Triphone.SetStart(0);
fst_Triphone.AddState();
c_state_symbs.AddSymbol("sil");//add the silence state, as end state.
fst_Triphone.SetFinal(1);
fst_Triphone.AddArc(0, Arc(con_symbs.Find("sil"), lex_symbs.Find("sil"), 0, 1));

Symbol triphone;	//the triphone on the arc
Symbol prev_state_label, next_state_label;	//labels on the states
int prev_state_id, next_state_id;	//id of the states


//step1:add states(a,sil) & arcs sil->(a,sil) sil-a+sil:a & arcs (a,sil)->sil sil:sil
for (auto phone : phone_list) //C++11 Range-based for.
{
next_state_id = fst_Triphone.AddState();//add the new state a,sil
next_state_label = phone + ",sil";
c_state_symbs.AddSymbol(next_state_label, next_state_id);//and its label. For quick locating.
triphone = "sil-" + phone + "+sil";
fst_Triphone.AddArc(1, Arc(con_symbs.AddSymbol(triphone), lex_symbs.Find(phone), 0, next_state_id));	//add the new arc sil:x
fst_Triphone.AddArc(next_state_id, Arc(con_symbs.Find("sil"), lex_symbs.Find("sil"), 0, 1));	//and the backoff arc.
}

//step2:add states(a,b) & arcs sil->(a,b) sil-a+b:a  & arcs (a,b)->(b,sil) a-b+sil:b
for (auto & phone_a : phone_list)	//C++11 range-based for. With auto ref type.
{
for (auto & phone_b : phone_list)
{
next_state_label = phone_a + ',' + phone_b;	//make next state "a,b"
next_state_id = fst_Triphone.AddState();
c_state_symbs.AddSymbol(next_state_label, next_state_id);

// 			if (next_state_id == kNoLabel)
// 			{
// 				next_state_id = fst_Triphone.AddState();
// 				c_state_symbs.AddSymbol(next_state_label, next_state_id);
// 			}
triphone = "sil-" + phone_a + '+' + phone_b;//generate triphone
fst_Triphone.AddArc(1, Arc(con_symbs.AddSymbol(triphone), lex_symbs.Find(phone_a), 0, next_state_id)); //add arc sil-a+b:a

fst_Triphone.AddArc(next_state_id, Arc(con_symbs.AddSymbol(triphone), lex_symbs.Find(phone_b), 0, c_state_symbs.Find(phone_b + ",sil")));
}
}

//step3: add inter-state arcs (a,b)->(b,c) a-b+c:b
for (auto & phone_a : phone_list)
{
for (auto & phone_b : phone_list)
{
prev_state_label = phone_a + ',' + phone_b;
prev_state_id = c_state_symbs.Find(prev_state_label);
for (auto & phone_c : phone_list)
{
next_state_label = phone_b + ',' + phone_c;
next_state_id = c_state_symbs.Find(next_state_label);
triphone = phone_a + '-' + phone_b + '+' + phone_c;

fst_Triphone.AddArc(prev_state_id, Arc(con_symbs.AddSymbol(triphone), lex_symbs.Find(phone_b), 0, next_state_id));
}
}
}
return true;
}
*/
