#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#else
#include <stdlib.h>
#endif // _DEBUG
#include <iostream>
using namespace std;

// This is only a simple unit test. 
char* MakeSentence(const char* strings[]) {
	const char* space = " ";
	const char* period = ".";
	int SizeOfArray = 0;
	while (strings[SizeOfArray] != NULL) SizeOfArray++;
	int TotalSizeOfArray = 0;
	for (int i = 0; i < SizeOfArray; i++) {
		int x = 0;
		while (strings[i][x] != NULL) x++;
		TotalSizeOfArray += x;
	}
	int SizeOfSentence = TotalSizeOfArray + SizeOfArray + 1;
	char* sentence = (char*)malloc(sizeof(char) * SizeOfSentence);
	strcpy(sentence, "\0");
	for (int i = 0; i < SizeOfArray; i++) {
		strcat(sentence, strings[i]);
		if (i != SizeOfArray - 1) {
			strcat(sentence, space);
		}
	}
	strcat(sentence, period);
	//cout << "Sentence : " << sentence << endl;
	return sentence;
}


int main(int i_argc, char** i_argl)
{
	const char* strings[] = {
		"This",
		"is",
		"a",
		"test",
		NULL
	};

	char* pSentence = MakeSentence(strings);

	cout<<"The Sentence is : "<< pSentence;
	free(pSentence);

	#if defined(_DEBUG)
		_CrtDumpMemoryLeaks();
	#endif // _DEBUG

	return 0;
}