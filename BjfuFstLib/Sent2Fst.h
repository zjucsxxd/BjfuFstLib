#ifndef _SENT2FST_H
#define _SENT2FST_H
#include "WFST.h"

class Sent2Fst
{
public:

	 // 读取发音词典
	bool LoadLexDic(const char * filename);

	// sent已经分词好了，词和词之间用空格隔开；WFST类的格式由你来定义。
	bool Sent2WordFST(const char * sent, WFST * wfst, const char * options=""); 

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
	void Arc2LexFst(int iState, int idArc,Symbol oWord ,vvstr lexs, WFST * wfst);
};

#endif