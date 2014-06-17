#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "FastLM.h"

CFastLM::CFastLM() :pWords(0),pPhons(0),nWords(0),nPhons(0),textbuffer(0),pNodeArray(0),pEdgeArray(0){}

CFastLM::~CFastLM()	{
	delete[] pWords;
	delete[] pPhons;
	delete[] textbuffer;
	delete[] pEdgeArray;
	delete[] pNodeArray;
}

bool CFastLM::SaveFastLM(const char * filename)
{
	char hdr[32] = {0};
	FILE * fid = fopen(filename,"wb");
	if (!fid) return false;
	fwrite(hdr,1,32,fid);

	fwrite(&fastNodeNum,1,4,fid);
	fwrite(&fastEdgeNum,1,4,fid);
	fwrite(&fastStNode,1,4,fid);
	fwrite(&fastEdNode,1,4,fid);
	fwrite(pNodeArray,sizeof(pNodeArray[0]),fastNodeNum,fid);
	fwrite(pEdgeArray,sizeof(pEdgeArray[0]),fastEdgeNum,fid);

	fwrite(&nWords,1,4,fid);
	fwrite(&nPhons,1,4,fid);
	int ntextbuffer = (pWords[nWords-1] - textbuffer) + strlen(pWords[nWords-1]) + 1;
	fwrite(&ntextbuffer,1,4,fid);

	fwrite(textbuffer,1,ntextbuffer,fid);

	int i,offset;
	for (i=0; i<nPhons; i++)
	{
		offset = pPhons[i] - textbuffer;
		fwrite(&offset,1,4,fid);
	}
	for (i=0; i<nWords; i++)
	{
		offset = pWords[i] - textbuffer;
		fwrite(&offset,1,4,fid);
	}
	
	fclose(fid);
	return true;
}

bool CFastLM::LoadFastLM(const char * filename)
{
	delete [] textbuffer;
	delete [] pNodeArray;
	delete [] pEdgeArray;
	delete [] pWords;
	delete [] pPhons;

	char hdr[32] = {0};

	FILE * fid = fopen(filename,"rb");
	if (!fid) return false;
	
	fread(hdr,1,32,fid);

	fread(&fastNodeNum,1,4,fid);
	fread(&fastEdgeNum,1,4,fid);
	fread(&fastStNode,1,4,fid);
	fread(&fastEdNode,1,4,fid);

	pNodeArray = new CFastNode[fastNodeNum];
	pEdgeArray = new CFastEdge[fastEdgeNum];

	fread(pNodeArray,sizeof(pNodeArray[0]),fastNodeNum,fid);
	fread(pEdgeArray,sizeof(pEdgeArray[0]),fastEdgeNum,fid);

	fread(&nWords,1,4,fid);
	fread(&nPhons,1,4,fid);
	int ntextbuffer;
	fread(&ntextbuffer,1,4,fid);

	textbuffer = new char[ntextbuffer];
	fread(textbuffer,1,ntextbuffer,fid);

	int i,offset;
	pPhons = new char *[nPhons];
	for (i=0; i<nPhons; i++)
	{
		fread(&offset,1,4,fid);
		pPhons[i] = textbuffer + offset;
	}
	pWords = new char *[nWords];
	for (i=0; i<nWords; i++)
	{
		fread(&offset,1,4,fid);
		pWords[i] = textbuffer + offset;
	}

	printf("Load %s successfully!\n",filename);

	fclose(fid);
	return true;
}
