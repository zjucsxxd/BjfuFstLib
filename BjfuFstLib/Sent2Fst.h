#ifndef _SENT2FST_H
#define _SENT2FST_H
#include "WFST.h"

class Sent2Fst
{
public:

	 // ��ȡ�����ʵ�
	bool LoadLexDic(const char * filename);

	// sent�Ѿ��ִʺ��ˣ��ʺʹ�֮���ÿո������WFST��ĸ�ʽ���������塣
	bool Sent2WordFST(const char * sent, WFST * wfst, const char * options=""); 

	// �Ѵʵ�FSTת�����ӵ�FST
	bool WordFST2PhoneFST(const WFST * wordFST, WFST * phoneFST); 

	// ������FSTת��������FST
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