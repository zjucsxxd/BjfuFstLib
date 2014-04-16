#pragma once
#include "WFST.h"

class Sent2Fst
{
public:

	 // 读取发音词典
	bool LoadLexDic(const char * filename);

	// sent已经分词好了，词和词之间用空格隔开；WFST类的格式由你来定义。
	bool Sent2WordFST(const char * sent, WFST * wfst); 

	// 把词的FST转成音子的FST
	bool WordFST2PhoneFST(const WFST * wordFST, WFST * phoneFST); 

	// 把音子FST转成三音子FST
	bool PhoneFST2TriphoneFST(const WFST * phoneFST, WFST * triphoneFST);


	Sent2Fst()
	{
		
	}

	~Sent2Fst()
	{

	}

	LexDict lexDict;
	std::set<Symbol> phone_list;

private:
	//replace an arc from iState to oState with lexs
	void Arc2LexFst(Symbol oWord, int iState, vvstr lexs, int oState, WFST * wfst);

};





bool Sent2Fst::Sent2WordFST(const char * sent, WFST * wfst)
{
	vstr parts;	//a vector<string> that saves all separated words in a sentence.
	split(sent, parts," ");
	auto& wordFst = wfst->_fst;
	auto& iSymbol = wfst->_isymbol;
	auto& oSymbol = wfst->_osymbol;
	bjfufst::StateId prev_state_id = wordFst.AddState(),next_state_id;
	auto eps_state_id = prev_state_id;
	auto eps_label = iSymbol.AddSymbol("<eps>");
	oSymbol.AddSymbol("<eps>");
	wordFst.SetStart(prev_state_id);	//add and set the first state.
	bool bIsFirst=true;	//flag indicating first word. First w

	for (auto it : parts)
	{
		next_state_id = wordFst.AddState();	//create next state
		wordFst.AddArc(prev_state_id, Arc(iSymbol.AddSymbol(it), oSymbol.AddSymbol(it), 0.8,next_state_id));	//and next arc.

		wordFst.AddArc(next_state_id, Arc(iSymbol.AddSymbol("<eps>"), oSymbol.AddSymbol("<eps>"), 0.2, eps_state_id));	//and backoff arc to eps state.
		if (bIsFirst)	
			bIsFirst = false;
		else
			wordFst.AddArc(eps_state_id, Arc(iSymbol.AddSymbol(it), oSymbol.AddSymbol(it), 0.5, next_state_id));	//and forwarding arc from eps state.

		wordFst.AddArc(next_state_id, Arc(iSymbol.AddSymbol("<eps>"), oSymbol.AddSymbol("<eps>"), 0.2, next_state_id));	//and self-ring arc.

		prev_state_id = next_state_id;
	}
	
	return false;
}

bool Sent2Fst::LoadLexDic(const char * filename)
{
	std::ifstream lexFile(filename);
	Symbol line;
	std::vector<Symbol> parts;
	Symbol word;

	while (getline(lexFile, line,'\n'))
	{
		split(line, parts," ");
		word = parts[0];
		parts.erase(parts.begin());	
		phone_list.insert(parts.begin(),parts.end());
		lexDict[word].push_back(parts);
	}

	return false;
}



bool Sent2Fst::WordFST2PhoneFST(const WFST * wordFST, WFST * phoneFST)
{
	//need to clear phoneFst first

	phoneFST->_fst = wordFST->_fst;	//copy from wordFst
//	phoneFST->_osymbol = wordFST->_osymbol;
	auto & wfst = phoneFST->_fst;
	int wordFst_state_number = wordFST->_fst.NumStates();	//save number of old states. 区分新老状态。
	for (int i = 0; i < wordFst_state_number;i++)
	{
		int nArc = 0;
		for (int j = 0; j < wfst.states[i].arcs.size();j++)
		{
			auto & arc = wfst.states[i].arcs[j];
			Label ilbl = arc.ilabel;
			Symbol oWord = wordFST->_osymbol.Find(ilbl);
			if (oWord=="<eps>" || oWord=="eps")
			{
//				wfst.DeleteArcs(i, 1);
				continue;
//				arc.nextstate = wfst.AddArc();
			}
			if (oWord!="")//if arc not replaced yet 
			{
				//TODO:remove old arc 
// 				wfst.RemoveArc(i,nArc);
				//do the replacement.
				Arc2LexFst(oWord, i, lexDict[oWord], arc.nextstate, phoneFST);
			}
			nArc++;
		}
	}



	/*
	static int counter = 0;
	counter++;
	if (fst.state_e != fst.state_s)
	{
		FST tmpfst = fst;
		if (fst.arcs.size() == 1)
		{
			arcs[arc_id].input = fst.arcs[0].input;
			arcs[arc_id].output = fst.arcs[0].output;
		}
		else
		{
			int state_size = states.size(); //记下替换前的FST的大小作为偏差量
			int arc_size = arcs.size();
			int state_i = arcs[arc_id].state_i; //保存下被替换弧的前后两个节点的ID state_i和state_o
			int state_o = arcs[arc_id].state_o;
			float W = arcs[arc_id].weight;

			for (int i = 0; i < tmpfst.arcs.size(); i++)
			{
				if (tmpfst.arcs[i].state_i == tmpfst.state_s) tmpfst.arcs[i].state_i = -2; //用-2,-3来暂代头尾节点的ＩＤ，以防被修改
				else tmpfst.arcs[i].state_i += state_size; //将arc指向后面新增的弧
				if (tmpfst.arcs[i].state_o == tmpfst.state_e) tmpfst.arcs[i].state_o = -3;
				else tmpfst.arcs[i].state_o += state_size;
			}

			tmpfst.states.erase(tmpfst.states.begin() + tmpfst.state_e);
			tmpfst.states.erase(tmpfst.states.begin() + tmpfst.state_s); //清除源FST中的state_s和state_e，



			for (int i = 0; i < tmpfst.arcs.size(); i++)
			{
				if (tmpfst.arcs[i].state_o >= tmpfst.state_e + state_size)  tmpfst.arcs[i].state_o--;
				if (tmpfst.arcs[i].state_i >= tmpfst.state_e + state_size)  tmpfst.arcs[i].state_i--; //纠正上一步造成的偏差
			}
			for (int i = 0; i < tmpfst.arcs.size(); i++)
			{
				if (tmpfst.arcs[i].state_o >= tmpfst.state_s + state_size)  tmpfst.arcs[i].state_o--;
				if (tmpfst.arcs[i].state_i >= tmpfst.state_s + state_size)  tmpfst.arcs[i].state_i--; //纠正上一步造成的偏差
			}

			for (int i = 0; i < tmpfst.arcs.size(); i++)
			{
				if (tmpfst.arcs[i].state_i == -2)
				{
					tmpfst.arcs[i].state_i = state_i;
					if (i > 0) states[state_i].arc_o.push_back(i + arc_size - 1);
				}
				if (tmpfst.arcs[i].state_o == -3)
				{
					tmpfst.arcs[i].state_o = state_o;    //恢复头尾节点ID
					tmpfst.arcs[i].weight = W;
				}
			}

			arcs[arc_id] = tmpfst.arcs[0]; //先把源fst的第一个弧替换掉arcs[arc_id]
			for (int i = 1; i < tmpfst.arcs.size(); i++)
			{
				arcs.push_back(tmpfst.arcs[i]);//添加处理好的弧
			}
			for (int i = 0; i < tmpfst.states.size(); i++)
			{
				for (int j = 0; j < tmpfst.states[i].arc_o.size(); j++)
				{
					if (tmpfst.states[i].arc_o[j] >= 0) tmpfst.states[i].arc_o[j] += (arc_size - 1); //修改节点的arc_out
				}

				states.push_back(tmpfst.states[i]);//添加处理好的状态
			}


			//          //tmpfst.arcs[0].state_i=state_i;//先用源FST的第一条弧替换被替换弧
			//          //tmpfst.arcs[0].state_o+=state_size;
			//          //tmpfst.arcs[state_e].state_o=;
			//          arcs[arc_id]=tmpfst.arcs[0];
			//          for (int i=0;i<tmpfst.arcs.size();i++)//遍历修改源fst中的节点和弧 并且将其加入到this fst的最后
			//          {
			//              tmpfst.arcs[i].state_i+=state_size;
			//              if (tmpfst.arcs[i].state_i==tmpfst.state_s)
			//              {
			//                  tmpfst.arcs[i].state_i=state_i;
			//              }
			//              if (tmpfst.arcs[i].state_o==tmpfst.state_e)//如果此弧指向源FST的末尾，则让其直接指向state_o
			//              {
			//                  tmpfst.arcs[i].state_o=state_o;
			//              }
			//              else    tmpfst.arcs[i].state_o+=state_size;
			//
			//              arcs.push_back(tmpfst.arcs[i]);
			//          }
			//
			//          for(int i=1;i<tmpfst.states[0].arc_o.size();i++)//将源FST的头结点的其余出弧连接上
			//          {
			//              states[state_i].arc_o.push_back(tmpfst.states[0].arc_o[i]+arc_size);
			//          }
			//
			//          for (int i=0;i<tmpfst.states.size();i++)//合并
			//          {
			//              for (int j=0;j<tmpfst.states[i].arc_o.size();j++)
			//              {
			//                  tmpfst.states[i].arc_o[j]+=arc_size;
			//              }
			//              states.push_back(states[i]);
			//          }
		}
		if (counter % 100 == 0) cout << counter << ":Phones of word" << fst.arcs[fst.arcs.size() - 1].output << "added." << endl;
	}
	else
	{
		cout << counter << ":OOPS, this is N-Gram" << endl;
	}
	*/
return 0;
}

bool Sent2Fst::PhoneFST2TriphoneFST(const WFST * phoneFST, WFST * triphoneFST)
{
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


void Sent2Fst::Arc2LexFst(Symbol oWord,int iState, vvstr lexs, int oState,WFST * wfst)
{
	//locate arc first
	fst &thefst = wfst->_fst;
	for (auto lex:lexs)
	{
		int prev_state_id = iState;
		int new_state_id;
		auto it_Phone = lex.begin();
		for ( ; it_Phone != lex.end() - 1;it_Phone++)
		{
			//insert state .
			new_state_id=thefst.AddState();
			//and arc to the new state ->
			thefst.AddArc(prev_state_id, Arc(wfst->_isymbol.AddSymbol(*it_Phone), wfst->_osymbol.AddSymbol("<eps>"), 0, new_state_id));
			prev_state_id = new_state_id;
		}
		//finally add the arc with output to the oState.
		thefst.AddArc(prev_state_id, Arc(wfst->_isymbol.AddSymbol(*it_Phone), wfst->_osymbol.AddSymbol(oWord), 0, oState));
	}
}
