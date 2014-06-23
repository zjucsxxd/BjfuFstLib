#include "WFST.h"
#include "Sent2Fst.h"

int main(const int argc, const char ** argv)
{
	std::fstream infile;
	infile.open("outtest.txt");
	//���Է�����
	Sent2Fst sent2fst;//����-FST����ת����
	WFST *wordFst;
	WFST *phoneFst;
	WFST *triphoneFst;//FST������
	sent2fst.LoadLexDic("dict");//����lex�ʵ�

	std::string sent(argv[0]);//��ת���ľ����ַ���Ĭ��Ϊ����0
	sent = "<s> it is a fine match with hockley </s>";//TEST PURPOSE ONLY.�ֶ�ָ��һ��Դ���ӣ��ǵð������׾�β��ǩ��
// 	char sent[250];
// 	int a = 0;
// 	char c[5];
// 	while (!infile.eof()){
// 		infile.getline(sent, 200);
// 		if (strlen(sent) < 5)
// 			continue;

		wordFst = new WFST("wordfst");
		phoneFst = new WFST("phonefst");
		triphoneFst = new WFST("triphonefst");


		sent2fst.Sent2WordFST(sent.c_str(), wordFst, "linear");//step1���Ӿ������ɼ򵥵�fst������������Ϊlinearʱ���ɵ��ߵ�fst���޻��˻��ȡ�
// 		wordFst.SaveFST("wordFst.fst");//����Ϊ��׼fst��ʽ�ļ�
 		wordFst->Draw("wordFst.dot");//����dotͼ���ļ���������graphviz����xdot�򿪲鿴��

		sent2fst.WordFST2PhoneFST(wordFst, phoneFst);//step2:��Wordfst�Ļ�һ�����滻�ɷ���fst����Ϊ��phone��fst��������ǰ�������lex�ʵ䡣
		//phoneFst.SaveFST("phoneFst.fst");
		phoneFst->Draw("phoneFst.dot");

		sent2fst.PhoneFST2TriphoneFST(phoneFst, triphoneFst);//step3:��phonefst��չ��triphone fst
// 		triphoneFst.SaveFST("triphoneFst.fst");
		triphoneFst->Draw("triphoneFst.dot");


		triphoneFst->SaveCFastLM("wfst");//����ΪCFastLM��ʽ���Թ�ASR���������á�
		
		delete wordFst, phoneFst, triphoneFst;
		

	
	/////////////////linear simple fst unit test//////////
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
// 
// 
// 
// 	linearFst._fst.AddArc(0, Arc(1, 1, 0.1, 1));//0-->1
// 	linearFst._fst.AddArc(1, Arc(2, 2, 0.2, 2));//1-->2
// 	linearFst._fst.AddArc(2, Arc(0, 0, 0.3, 3));//2-->3
// 	linearFst._fst.AddArc(3, Arc(4, 4, 0.4, 4));//3-->4
// 
// 	linearFst._fst.AddArc(2, Arc(0, 0, 0.5, 5));//4-->5
// 	linearFst._fst.AddArc(5, Arc(2, 2, 0.6, 4));//5-->6
// 
// 
// 
// 	linearFst._isymbol.AddSymbol("n");
// 	linearFst._isymbol.AddSymbol("i");
// 	linearFst._isymbol.AddSymbol("d");
// 	linearFst._isymbol.AddSymbol("e");
// 
// 	linearFst._osymbol.AddSymbol("n");
// 	linearFst._osymbol.AddSymbol("i");
// 	linearFst._osymbol.AddSymbol("d");
// 	linearFst._osymbol.AddSymbol("e");
// 
// 	linearFst.Draw("linear.dot");
// 
// 	sent2fst.PhoneFST2TriphoneFST(&linearFst, &triphone_linearFst);
// 	triphone_linearFst.Draw("triphone_linear.dot");
	return 0;
}