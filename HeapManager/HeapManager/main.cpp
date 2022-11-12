#include "HeapManagerProxy.h"
#include "HeapManager_UnitTest.h"
#include <iostream>
using namespace std;

int main() {
	bool flag =  HeapManager_UnitTest();
	cout << "result is : "<<flag;
	return 0;
}