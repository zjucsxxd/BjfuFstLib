#include "WFST.h"
#include "Sent2Fst.h"
#include <string.h>

using namespace bjfufst;

char *intoa(int value, char *string ,int radix){
	if (radix!=10){
		printf("radix wrong in intoa!\n");
		return NULL;
	}
	int a,b,c,d;
	a = value/1000;
	b = (value%1000)/100;
	c = (value%100)/10;
	d = value%10;
	char *str = new char[10];
	int pos = 0;
	if (a>0)
		str[pos++] = a + '0';
	if (b>0 || pos>0)
		str[pos++] = b + '0';
	if (c>0 || pos>0)
		str[pos++] = c + '0';
	if (d>0 || pos>0)
		str[pos++] = d + '0';
	if (pos==0)
		str[pos++] = '0';
	str[pos] = '\0';
	strcpy(string ,str);
	delete [] str;
	return "ok";
}

int main(const int argc, const char ** argv)
{
// 	WFST inFst("fromopenfst");
// 	inFst.LoadFST("wfst853.txt","bjfu.c.isyms","bjfu.t.osyms");
// 	inFst.SaveCFastLM("wfst853.flm");
// 	return 0;
 

	double start, end;
	freopen("stdout.txt", "w", stdout);
	start = clock();
	std::fstream infile;
	infile.open("sentences.txt");
	//测试范例：
	Sent2Fst sent2fst;//句子-FST网络转换器
	WFST *wordFst;
	WFST *phoneFst;
	WFST *triphoneFst;//FST网络类
	sent2fst.LoadLexDic("dict");//载入lex词典
	sent2fst.LoadTiedList("tiedlist");

	std::string sent("<s> hello world </s>");
	std::string wordfst_name("TESTEPS");
	std::cout << wordfst_name << std::endl;
	wordFst = new WFST(wordfst_name);
	phoneFst = new WFST(std::string("phone") + wordfst_name);
	triphoneFst = new WFST(std::string("triphone") + wordfst_name);

	sent2fst.Sent2WordFST(sent.c_str(), wordFst/*, "linear"*/);
	wordFst->Draw(wordfst_name.c_str());
	sent2fst.WordFST2PhoneFST(wordFst, phoneFst);
	delete wordFst;
	sent2fst.PhoneFST2TriphoneFST(phoneFst, triphoneFst);
	delete phoneFst;
	// 		wordFst->SaveFST(wordfst_name);
	wordFst->Draw("wordFst.dot");//画成dot图形文件，可以用graphviz或者xdot打开查看。
	triphoneFst->SaveCFastLM(wordfst_name.c_str());
	triphoneFst->Draw("triPhoneFst.dot");
	delete triphoneFst;
	return 0;




	//std::string sent(argv[0]);//被转换的句子字符串默认为参数0
	//sent = "<s> which buses should i take if i want to get to nangao </s>";//TEST PURPOSE ONLY.手动指定一个源句子，记得包含句首句尾标签。
// 	char sent[250];
// 	int a = 0;
// 	char c[10];
// 	while (!infile.eof()){
// 		infile.getline(sent, 200);
// 		if (strlen(sent) < 5)
// 			continue;
// 
// 		char wordfst_name[15];
// 		intoa(a, c, 10);
// 		strcpy(wordfst_name,"wordfst");
// 		strcat(wordfst_name, c);
// 		std::cout << wordfst_name << std::endl;
// 		wordFst = new WFST(wordfst_name);
// 		phoneFst = new WFST(std::string("phone")+wordfst_name);
// 		triphoneFst = new WFST(std::string("triphone") + wordfst_name);
// 		
// 		sent2fst.Sent2WordFST(sent, wordFst/*, "linear"*/);
// 		wordFst->Draw(wordfst_name);
// 		sent2fst.WordFST2PhoneFST(wordFst, phoneFst);
// 		delete wordFst;
// 		sent2fst.PhoneFST2TriphoneFST(phoneFst, triphoneFst);
// 		delete phoneFst;
// // 		wordFst->SaveFST(wordfst_name);
// 		wordFst->Draw("wordFst.dot");//画成dot图形文件，可以用graphviz或者xdot打开查看。
// 		triphoneFst->SaveCFastLM(wordfst_name);
// 		triphoneFst->Draw("triPhoneFst.dot");
// 		delete triphoneFst;
// 		a++;
// 		if (a > 999)
// 			break;
// 	}
	
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
	printf("%lf\n", (end - start)/CLOCKS_PER_SEC);
	return 0;
}
