#include<iostream>
#include"PRIORITY_QUEUE.h"
#include<fstream>
#include <string> 
#include <Windows.h>
#include<vector>
//#include"fileHeader.h"
using namespace std;
#define MAX_NODE 511
struct fileInfor {
	char nameFile[256];
	unsigned int sizeBefor;
	unsigned int sizeAfter;
	unsigned int addressBegin;
	unsigned int usebit;
};

struct HEADER {
	char fileCode[8];
	unsigned int Freq[256];
	unsigned int numberOfFile;
};

HEADER headerFile;
HUFFNode HuffTree[MAX_NODE];
struct BITCODE {
	char*	bits;
	int		numBit;
};
const int MAX_BIT_LENGTH = 1000;
BITCODE	bitArray[256];


void initHuffman();
void makeFrequencyStatistics(unsigned char buff[1024], unsigned char stopIndex);
void findFilesInFolder(const string input, string a[], unsigned int sizefile[]);
void moveFreqToQueue(PRIORITY_QUEUE &a);
int  makeHuffmanTree(PRIORITY_QUEUE &a);
void traverseHuffmanTree(int node, char bitcode[], int nbit);
void makeBitCode(int nRoot);
void encodeAChar(unsigned char c, unsigned char &out, unsigned char &bitIndex, ofstream &fileout);
void compressedFile(string inputFile);
void initHeader();
void initFile(fileInfor &a, string namefile, unsigned int size);
void decodeAChar(unsigned char c, char bitOfChar[8]);
void outputHeader(HEADER &dd);
void outFileInfor(fileInfor a);
void makeCharFromBit(int node, char bitcode[], char nbit, ofstream &fileout);
void main()
{
	string f;
	cout << "Link:";
	getline(cin, f);
	initHeader();
	compressedFile(f);
	system("pause");
}
//*************************FILE*******************************************
void initHeader() {
	char*s = "hthuc";
	strcpy_s(headerFile.fileCode,sizeof(headerFile.fileCode), s);
	headerFile.numberOfFile = 0;
}
void initFile(fileInfor &a, string namefile,unsigned int size)
{
	a.addressBegin = 0;
	strcpy_s(a.nameFile,sizeof(a.nameFile), namefile.c_str());
	a.sizeAfter = 0;
	a.sizeBefor = size;
	a.usebit = 0;
}

void outFileInfor(fileInfor a)
{
	cout << "Name file:" << a.nameFile << endl;
	cout << "size before:" << a.sizeBefor << endl;
	cout << "size after:" << a.sizeAfter << endl;
	cout << "address begin:" << a.addressBegin << endl;
	cout << "use bit:" << a.usebit << endl;

}
void outputHeader(HEADER &dd)
{
	cout << "file code:" << dd.fileCode << endl;
	cout << "file num:" << dd.numberOfFile << endl;


}


//**************************HUFFMAN*****************************************

void initHuffman()
{
	for (int i = 0; i < MAX_NODE; i++)
	{
		HuffTree[i].c = i;
		HuffTree[i].freq = 0;
		HuffTree[i].nLeft = -1;
		HuffTree[i].nRight = -1;
	}
}

void findFilesInFolder(const string input,string a[], unsigned int sizefile[])
{

	string strTmp = "\\*.*";
	string nameFolder = input + strTmp;
	const char *chFolderpath = nameFolder.c_str();
	const char *linkFile;
	unsigned char buff[1024];
	string data;
	HANDLE hFind;
	WIN32_FIND_DATAA data2;
	ifstream filein;
	unsigned char stopIndex = 0;
	int i = 0;
	hFind = FindFirstFileA(chFolderpath, &data2);

	if (hFind != INVALID_HANDLE_VALUE)
	{

		do
		{
			if (data2.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				continue;


			data = input + "\\" + "\\" + data2.cFileName;
			a[i] = data2.cFileName;
			sizefile[i] = data2.nFileSizeLow;
			i++;
			linkFile = data.c_str();

			filein.open(linkFile, ios::in|ios::binary);
			while (!filein.eof())
			{
				filein.read((char*)buff, 1024);
				stopIndex = filein.gcount();
				makeFrequencyStatistics(buff, stopIndex);
			}
			filein.close();
			headerFile.numberOfFile++;
			
		} while (FindNextFileA(hFind, &data2));

		FindClose(hFind);
	}

}

void makeFrequencyStatistics(unsigned char buff[1024], unsigned char stopIndex)
{
	for (int i = 0; i <= stopIndex; i++)
	{
		HuffTree[buff[i]].freq++;
	}
}
void moveFreqToQueue(PRIORITY_QUEUE &a)
{
	for (int i = 0; i < 256; i++)
	{
		if(HuffTree[i].freq>0)
			a.enQueue(HuffTree[i]);
	}
}
int makeHuffmanTree(PRIORITY_QUEUE &a)
{
	int nNode = 256;
	HUFFNode value1, value2;
	
	while (true) {
		a.deQueue(value1);
		a.deQueue(value2);
		if (a.isEmpty())
		{
			if (value1.freq == value2.freq)
			{
				if (value1.c > value2.c) {
					HuffTree[nNode].freq = value1.freq + value2.freq;
					HuffTree[nNode].nLeft = value2.c;
					HuffTree[nNode].nRight = value1.c;
				}
				else {
					HuffTree[nNode].freq = value1.freq + value2.freq;
					HuffTree[nNode].nLeft = value1.c;
					HuffTree[nNode].nRight = value2.c;
				}
			}
			else
			{
				HuffTree[nNode].freq = value1.freq + value2.freq;
				HuffTree[nNode].nLeft = value1.c;
				HuffTree[nNode].nRight = value2.c;

			}
			break;
		}
	
		if (value1.freq == value2.freq)
		{
			if (value1.c > value2.c) {
				HuffTree[nNode].freq = value1.freq + value2.freq;
				HuffTree[nNode].nLeft = value2.c;
				HuffTree[nNode].nRight = value1.c;
				a.enQueue(HuffTree[nNode]);
				nNode++;
			}
			else {
				HuffTree[nNode].freq = value1.freq + value2.freq;
				HuffTree[nNode].nLeft = value1.c;
				HuffTree[nNode].nRight = value2.c;
				a.enQueue(HuffTree[nNode]);
				nNode++;
			}
		}
		else
		{
			HuffTree[nNode].freq = value1.freq + value2.freq;
			HuffTree[nNode].nLeft = value1.c;
			HuffTree[nNode].nRight = value2.c;
			a.enQueue(HuffTree[nNode]);
			nNode++;

		}
		
	}

	return nNode ; 

}

void traverseHuffmanTree(int node, char bitcode[], int nbit)
{
	if (node == -1) {
		return;
	}
	if (HuffTree[node].nLeft == -1 && HuffTree[node].nRight == -1)
	{	
		bitArray[node].numBit = nbit;
		bitArray[node].bits = new char[nbit];
		for (int i = 0; i < nbit; i++) {
			bitArray[node].bits[i] = bitcode[i];
		}
		return;
	}
	else {
		
		bitcode[nbit] = 0;
		traverseHuffmanTree(HuffTree[node].nLeft, bitcode, nbit+ 1);

		bitcode[nbit] = 1;
		traverseHuffmanTree(HuffTree[node].nRight, bitcode, nbit + 1);
	}
}

void makeBitCode(int nRoot)
{ 
	for (int i = 0; i < 256; i++) {
		bitArray[i].numBit = 0;
		bitArray[i].bits = NULL;
	};
	char bitcode[MAX_BIT_LENGTH];
	int nbit = 0;

	traverseHuffmanTree(nRoot, bitcode, nbit);
}


void encodeAChar(unsigned char c, unsigned char &out, unsigned char &bitIndex, ofstream &fileout)
{ 

	for (int i = 0; i < bitArray[c].numBit; i++) {
		if (bitArray[c].bits[i] == 1) {
			out = out | (1 << bitIndex);
		}
		if (bitIndex == 0) { 
			bitIndex = 7;
			fileout.write((char*)&out, sizeof(out));
			out = 0;
		}
		else {
			bitIndex--;
		}
	}
}
void decodeAChar(unsigned char c, char bitOfChar[8])
{
	int i;
	for (i = 0; i < 8; ++i) {
		bitOfChar[i] = (c >> i) & 1;
	}
}
void makeCharFromBit(int node, char bitcode[], char nbit, ofstream &fileout)
{
	if (node == -1) {
		return;
	}
	if (HuffTree[node].nLeft == -1 && HuffTree[node].nRight == -1)
	{
		fileout.write((char*)&HuffTree[node].c, sizeof(HuffTree[node].c));
		return;
	}
	else {

		if(bitcode[nbit] = 0)
			makeCharFromBit(HuffTree[node].nLeft, bitcode, nbit + 1,fileout);

		if(bitcode[nbit] = 1)
			makeCharFromBit(HuffTree[node].nRight, bitcode, nbit + 1,fileout);
	}
}

void compressedFile(string inputFile)
{
	string namefile[1000];
	unsigned int sizefile[1000];
	string linkfile;
	const char* linkFile;


	PRIORITY_QUEUE a(256);
	initHuffman();
	findFilesInFolder(inputFile,namefile,sizefile);
	moveFreqToQueue(a);
	int nRoot =makeHuffmanTree(a);

	makeBitCode(nRoot);


	//Header 
	for (int i = 0; i < 256; i++)
	{
		headerFile.Freq[i] = HuffTree[i].freq;
	}
	fileInfor *fileNum=new fileInfor[headerFile.numberOfFile];
	for (int i = 0; i < headerFile.numberOfFile; i++)
	{
		initFile(fileNum[i], namefile[i],sizefile[i]);
	}
	ofstream fileout;
	fileout.open("File.huff", ios::binary|ios::out);
	fileout.write((char*)&headerFile, sizeof(headerFile));
	for (int i = 0; i < headerFile.numberOfFile; i++)
	{
		fileout.write((char*)&fileNum[i], sizeof(fileNum[i]));
	}
	int vx = fileout.tellp();
	fileout.seekp(1300, ios::beg);
	
	fileout.write((char*)&vx, sizeof(vx));
	fileout.seekp(0, ios::end);
	for (int i = 0; i < headerFile.numberOfFile; i++)
	{
		unsigned char c;
		unsigned char out = 0;
		unsigned char bitUse = 0;		
		unsigned char bitIndex = 7;			
		linkfile=inputFile+ "\\" + "\\" + namefile[i];
		linkFile = linkfile.c_str();
		ifstream filein;
		filein.open(linkFile, ios::in);
		while (!filein.eof())
		{
			filein.read((char*)&c, sizeof(c));
			encodeAChar(c, out, bitIndex, fileout);
		}
		bitUse = 7 - bitIndex;
		if (bitUse == 0)
		{
			bitUse = 8;
		}
		else {
			fileout.write((char*)&out, sizeof(out));
		}
	

		filein.close();

	}
	fileout.close();
	fileInfor x;

	ifstream filein;
	filein.open("File.huff", ios::binary | ios::out);
	filein.seekg(1036, ios::beg);
	filein.read((char*)&x, sizeof(x));
	filein.close();
	outFileInfor(x);

}
