#include "fileHeader.h"

void HEADER::initHeader(unsigned int freq[256], unsigned int num)
{
	numberOfFile = num;
	for (int i = 0; i < 256; i++)
	{
		Freq[i] = freq[i];
	}
	file = new fileInfor[numberOfFile];

}

void fileInfor::initFileInfor(char * name, unsigned int sizeBe, unsigned int sizeAf, unsigned int addrBegin)
{
	strcpy_s(nameFile,sizeof(nameFile), name);
	sizeBefor = sizeBe;
	sizeAfter = sizeAf;
	addressBegin = addrBegin;
}
