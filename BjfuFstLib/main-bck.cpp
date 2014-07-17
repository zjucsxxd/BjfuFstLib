#include "WFST.h"
#include "Sent2Fst.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int main(const int argc, const char ** argv)
{
	long start, end;
	freopen("stdout.txt", "w", stdout);
	start = clock();
	std::fstream infile;
	infile.open("out.txt");
	//测试范例：
	Sent2Fst sent2fst;//句子-FST网络转换器
	WFST *wordFst;
	WFST *phoneFst;
	WFST *triphoneFst;//FST网络类
	sent2fst.LoadLexDic("dict");//载入lex词典

	//std::string sent(argv[0]);//被转换的句子字符串默认为参数0
	//sent = "<s> which buses should i take if i want to get to nangao </s>";//TEST PURPOSE ONLY.手动指定一个源句子，记得包含句首句尾标签。
	char sent[250];
	int a = 0;
	char c[5];
	while (!infile.eof()){
		infile.getline(sent, 200);
		if (strlen(sent) < 5)
			continue;

		wordFst = new WFST("wordfst");
		phoneFst = new WFST("phonefst");
		triphoneFst = new WFST("triphonefst");


		sent2fst.Sent2WordFST(sent, wordFst, "linear");//step1：从句子生成简单的fst。第三个参数为linear时生成单线的fst，无回退弧等。
// 		wordFst.SaveFST("wordFst.fst");//保存为标准fst格式文件
// 		wordFst.Draw("wordFst.dot");//画成dot图形文件，可以用graphviz或者xdot打开查看。

		sent2fst.WordFST2PhoneFST(wordFst, phoneFst);//step2:将Wordfst的弧一条条替换成发音fst，成为有phone的fst。依赖于前面载入的lex词典。
		//phoneFst.SaveFST("phoneFst.fst");
		//phoneFst.Draw("phoneFst.dot");

		sent2fst.PhoneFST2TriphoneFST(phoneFst, triphoneFst);//step3:从phonefst扩展成triphone fst
// 		triphoneFst.SaveFST("triphoneFst.fst");
// 		triphoneFst.Draw("triphoneFst.dot");

		char wfst_name[15];
		itoa(a, c, 10);
		strcpy(wfst_name,"wfst");
		std::cout << c << std::endl;
		strcat(wfst_name, c);
		std::cout << wfst_name << std::endl;
		triphoneFst->SaveCFastLM(wfst_name);//保存为CFastLM格式。以供ASR解码器调用。
		
		delete wordFst, phoneFst, triphoneFst;
		
		a++;
		if (a > 1000)
			break;
	}
	
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
	end = clock();
	printf("%ld\n", start - end);
	return 0;
}
