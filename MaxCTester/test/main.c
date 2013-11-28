#include "MaxTester.h"

bool testOK(){
	OK();
}

int main(){

	addTest(testOK,true);

	RUN_TESTS();
	return 0;
}

