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
	sent = "A B";
	sent2fst.Sent2WordFST(sent.c_str(), &wordFst);
	wordFst.SaveFST("wordFst.fst");
	wordFst.Draw("wordFst.dot");

	sent2fst.WordFST2PhoneFST(&wordFst, &phoneFst);
	phoneFst.SaveFST("phoneFst.fst");
	phoneFst.Draw("phoneFst.dot");

	sent2fst.PhoneFST2TriphoneFST(&phoneFst, &triphoneFst);
	triphoneFst.SaveFST("triphoneFst.fst");
	triphoneFst.Draw("triphoneFst.dot");
	
	/////////////////linear simple fst test//////////
// 	WFST linearFst("linearFst");
// 	WFST triphone_linearFst("triphone_linearFst");
// 
// 	StateId stateid;
// 
// 	linearFst._fst.AddState();//state0
// 	linearFst._fst.AddState();//state1
// 	linearFst._fst.AddState();//state2
// 	linearFst._fst.AddState();//state3
// 	linearFst._fst.AddState();//state4
// 	linearFst._fst.AddState();//state5
// 	linearFst._fst.AddState();//state6
// 	linearFst._fst.AddState();//state7
// 	linearFst._fst.AddState();//state8
// 	linearFst._fst.AddState();//state9
// 
// 
// 	linearFst._fst.AddArc(0, Arc(1, 1, 0.1, 1));//0-->1
// 	linearFst._fst.AddArc(1, Arc(2, 2, 0.2, 2));//1-->2
// 	linearFst._fst.AddArc(2, Arc(3, 3, 0.3, 3));//2-->3
// 	linearFst._fst.AddArc(3, Arc(1, 1, 0.4, 4));//3-->4
// 	linearFst._fst.AddArc(4, Arc(3, 3, 0.5, 5));//4-->5
// 	linearFst._fst.AddArc(5, Arc(1, 1, 0.6, 6));//5-->6
// 	linearFst._fst.AddArc(6, Arc(2, 2, 0.7, 7));//6-->7
// 
// 	linearFst._fst.AddArc(2, Arc(4, 4, 0.8, 8));//2-->3
// 	linearFst._fst.AddArc(8, Arc(2, 2, 0.9, 9));//3-->4
// 	linearFst._fst.AddArc(9, Arc(4, 4, 0.5, 5));//4-->5
// 
// 
// 
// 	linearFst._isymbol.AddSymbol("a");
// 	linearFst._isymbol.AddSymbol("b");
// 	linearFst._isymbol.AddSymbol("c");
// 	linearFst._isymbol.AddSymbol("d");
// 	linearFst._osymbol.AddSymbol("a");
// 	linearFst._osymbol.AddSymbol("b");
// 	linearFst._osymbol.AddSymbol("c");
// 	linearFst._osymbol.AddSymbol("d");
// 
// 	sent2fst.PhoneFST2TriphoneFST(&linearFst, &triphone_linearFst);
// 	linearFst.Draw("linear.dot");
// 	triphone_linearFst.Draw("triphone_linear.dot");

	return 0;
}