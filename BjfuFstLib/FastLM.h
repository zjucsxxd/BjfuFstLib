#ifndef _FAST_LM_H_
#define _FAST_LM_H_

typedef unsigned int   int32;
typedef unsigned short int16;

struct CFastNode
{
	int32 edgeStart;
	int32 edgeCount;
	float best;//0
};
struct CFastEdge
{
	int32 nodeTo;
	int16 idata;
	int16 odata;
	float weight;
};
struct CFastLM
{
public: // for decoder
	CFastNode * pNodeArray;
	CFastEdge * pEdgeArray;
	int fastNodeNum;
	int fastEdgeNum;
	int fastStNode;
	int fastEdNode;
public: // for user
	char * textbuffer;
	char ** pWords;
	int     nWords;
	char ** pPhons;//optional
	int     nPhons;
public:
	CFastLM();
	~CFastLM();

	bool SaveFastLM(const char * filename);
	bool LoadFastLM(const char * filename);
};

#endif
