final : main.o arc.o FastLM.o fst.o FstConventions.o Sent2Fst.o state.o SymbolTable.o WFST.o
	g++ -std=c++11 -O3 -o final main.o arc.o FastLM.o fst.o FstConventions.o Sent2Fst.o state.o SymbolTable.o WFST.o

main.o : main.cpp WFST.h Sent2Fst.h
	g++ -O3 -c -std=c++11 main.cpp

arc.o : arc.cpp arc.h
	g++ -O3 -c -std=c++11 arc.cpp 

FastLM.o : FastLM.cpp FastLM.h
	g++ -O3 -c -std=c++11 FastLM.cpp

fst.o : fst.cpp fst.h FastLM.h
	g++ -O3 -c -std=c++11 fst.cpp

FstConventions.o : FstConventions.cpp FstConventions.h
	g++ -O3 -c -std=c++11 FstConventions.cpp

Sent2Fst.o : Sent2Fst.cpp Sent2Fst.h FstConventions.h
	g++ -O3 -c -std=c++11 Sent2Fst.cpp

state.o : state.cpp state.h
	g++ -O3 -c -std=c++11 state.cpp

SymbolTable.o : SymbolTable.cpp SymbolTable.h
	g++ -O3 -c -std=c++11 SymbolTable.cpp

WFST.o : WFST.cpp WFST.h
	g++ -O3 -c -std=c++11 WFST.cpp

clean :
	rm final main.o arc.o FastLM.o fst.o FstConventions.o Sent2Fst.o state.o SymbolTable.o WFST.o
