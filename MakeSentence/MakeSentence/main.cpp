#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#else
#include <stdlib.h>
#endif // _DEBUG
#include <conio.h>
#include <iostream>
#include <string>

using namespace std;

// This is only a simple unit test. 
char* MakeSentence(char* strings[]) {
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
	memset(sentence, '\0', sizeof(sentence));
	for (int i = 0; i < SizeOfArray; i++) {
		strcat(sentence, strings[i]);
		if (i != SizeOfArray - 1) {
			strcat(sentence, space);
		}
	}
	strcat(sentence, period);
	cout << "Sentence : " << sentence << endl;
	return sentence;
}


int main(int i_argc, char** i_argl)
{
	
	char* strings[20] = {0};
	int size = 0;
	
	do {
		char InputWord[10];
		int k = 0;
		char ch;
		cout << "Enter a word for the sentence. Enter to end : ";
		while((ch = _getch()) != '\r') {
			InputWord[k] = ch;
			cout << "*";
			k++;
		}
		InputWord[k] = '\0';
		int InputWordSize = 0;
		cout << "Entered word is: " << InputWord << endl;
		while (InputWord[InputWordSize] != NULL) InputWordSize++;
		
		if (InputWordSize > 0) {
			strings[size] = (char*)malloc(InputWordSize + 1);
			strcpy(strings[size], InputWord);
			cout << "strlen : " << strlen(InputWord) << endl;
			cout << "index : " << size << endl;
			cout << "Entered word is: " << strings[size] << endl;
			size++; 
		}
		else {
			//strings[size] = (char*)malloc(1);
			strings[size] = NULL;
			break; 
		}
	} while (1);
	cout << "Out of do while "  << endl;
	for (int i = 0; i < size; i++) {
		cout << "strings[" << i << "] : " << strings[i] << endl;
	}
	char* pSentence = MakeSentence(strings);

	cout<<"The Sentence is : "<< pSentence;
	
	for (int i = 0; i < size; i++) {
		free(strings[i]);
	}
	free(pSentence);
	#if defined(_DEBUG)
		_CrtDumpMemoryLeaks();
	#endif // _DEBUG

	return 0;
}