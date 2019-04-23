#pragma once
#include<iostream>
using namespace std;
class fileInfor {
private:
	char nameFile[255];
	unsigned int sizeBefor;
	unsigned int sizeAfter;
	unsigned int addressBegin;
	char usebit;
public:
	fileInfor() {
		sizeBefor = 0;
		sizeAfter = 0;
		addressBegin = 0;
		usebit = 0;
	}
	void initFileInfor(char*name, unsigned int sizeBe, unsigned int sizeAf, unsigned int addrBegin);

};
class HEADER {
private:
	char fileCode[6];
	unsigned int Freq[256];
	unsigned int numberOfFile;
	fileInfor *file;
public:
	HEADER()
	{
		char* s = "XYswq";
		strcpy_s(fileCode,sizeof(fileCode), s);
		numberOfFile = 0;
		file = NULL;
	}
	void initHeader(unsigned int freq[256], unsigned int num);

};






