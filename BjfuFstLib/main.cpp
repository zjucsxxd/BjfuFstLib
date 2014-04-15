#include "Sent2Fst.h"
#include "WFST.h"

int main()
{
	Sent2Fst sent2fst;
	WFST wordFst("wordFst");
	WFST phoneFst("phoneFst");
	WFST triphoneFst("triphoneFst");
	sent2fst.LoadLexDic("lexdic.txt");
	std::string sent;
	sent = "A B C";
	sent2fst.Sent2WordFST(sent.c_str(), &wordFst);
	wordFst.SaveFST("wordFst.fst");
	
	sent2fst.WordFST2PhoneFST(&wordFst, &phoneFst);
	phoneFst.SaveFST("phoneFst.fst");

	sent2fst.PhoneFST2TriphoneFST(&phoneFst, &triphoneFst);
	triphoneFst.SaveFST("triphoneFst.fst");

	return 0;
}