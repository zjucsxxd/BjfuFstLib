#include "Sent2Fst.h"
#include "WFST.h"

int main(const int argc, const char ** argv)
{

	Sent2Fst sent2fst;
	
// 	WFST wordFst("wordFst");
// 	WFST phoneFst("phoneFst");
// 	WFST triphoneFst("triphoneFst");
// 	sent2fst.LoadLexDic("lexdic.txt");
// 	std::string sent;
// 	sent = "A B";
// 	sent2fst.Sent2WordFST(sent.c_str(), &wordFst);
// 	wordFst.SaveFST("wordFst.fst");
// 	wordFst.Draw("wordFst.dot");
// 
// 	sent2fst.WordFST2PhoneFST(&wordFst, &phoneFst);
// 	phoneFst.SaveFST("phoneFst.fst");
// 	phoneFst.Draw("phoneFst.dot");
// 
// 	sent2fst.PhoneFST2TriphoneFST(&phoneFst, &triphoneFst);
// 	triphoneFst.SaveFST("triphoneFst.fst");
// 	triphoneFst.Draw("triphoneFst.dot");
	
	/////////////////linear simple fst test//////////
	WFST linearFst("linearFst");
	WFST triphone_linearFst("triphone_linearFst");

	StateId stateid;

	linearFst._fst.AddState();//state0
	linearFst._fst.AddState();//state1
	linearFst._fst.AddState();//state2
	linearFst._fst.AddState();//state3
	linearFst._fst.AddState();//state4
	linearFst._fst.AddState();//state5



	linearFst._fst.AddArc(0, Arc(1, 1, 0.1, 1));//0-->1
	linearFst._fst.AddArc(1, Arc(2, 2, 0.2, 2));//1-->2
	linearFst._fst.AddArc(2, Arc(0, 0, 0.3, 3));//2-->3
	linearFst._fst.AddArc(3, Arc(4, 4, 0.4, 4));//3-->4

	linearFst._fst.AddArc(2, Arc(0, 0, 0.5, 5));//4-->5
	linearFst._fst.AddArc(5, Arc(2, 2, 0.6, 4));//5-->6



	linearFst._isymbol.AddSymbol("n");
	linearFst._isymbol.AddSymbol("i");
	linearFst._isymbol.AddSymbol("d");
	linearFst._isymbol.AddSymbol("e");

	linearFst._osymbol.AddSymbol("n");
	linearFst._osymbol.AddSymbol("i");
	linearFst._osymbol.AddSymbol("d");
	linearFst._osymbol.AddSymbol("e");

	linearFst.Draw("linear.dot");

	sent2fst.PhoneFST2TriphoneFST(&linearFst, &triphone_linearFst);
	triphone_linearFst.Draw("triphone_linear.dot");

	return 0;
}