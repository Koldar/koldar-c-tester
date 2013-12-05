//#include "KoldarCTester.h"

#define KCT_CONTROLMACRO_TESTSEPARATOR "%%%%%%%%%%%%%%%"

#include "KoldarCTester.h"
#include <stdbool.h>
#include <stdio.h>

bool testOK(){
	printf("executing testOK\n");
	return true;
}

bool testOK2(){
	printf("executing testOK2\n");
	return false;
}
//	COPYSTRING TEST
//	char* dest;
//	char src[]="WONDERFUL";
//	KCT_COPYSTRING(dest,src);
//	printf("\"%s\"\n",dest);

//	INITTESTLIST TEST
//	TestList* list=NULL;
//	KCT_INITTESTLIST(list);
//	printf("%p\n",list);
//	printf("head %p\n",list->head);
//	printf("tail %p\n",list->tail);
//	printf("size %d\n",list->size);

	//KCT_INITTESTLISTELEMENT TEST
//	TestListElement* el=NULL;
//	KCT_INITTESTLISTELEMENT(el,testOK,"a test function",true);
//	printf("pointer %p\n",el);
//	printf("description \"%s\"\n",el->description);
//	printf("error message \"%s\"\n",el->errorMessage);
//	printf("next %p\n",el->next);
//	printf("skip %s\n",el->skip==0?"false":"true");
//	printf("test function %p\n",el->testFunction);


	//KCT_ADDTESTCASE TEST
//	TestList* list=NULL;
//	KCT_INITTESTLIST(list);
//	KCT_ADDTESTCASE(list,testOK,"a beatiful test!",true);
//	KCT_ADDTESTCASE(list,testOK2,"some other beatiful test!",true);
//	TestListElement* tmp=list->head;
//	while (tmp!=NULL){
//		printf("pointer %p\n",tmp);
//		printf("description \"%s\"\n",tmp->description);
//		printf("error message \"%s\"\n",tmp->errorMessage);
//		printf("next %p\n",tmp->next);
//		printf("skip %s\n",tmp->skip==0?"false":"true");
//		printf("test function %p\n",tmp->testFunction);
//		tmp=tmp->next;
//	}
//	printf("size %d\n",list->size);


	//KCT_FREETESTLIST TEST
//	TestList* list=NULL;
//	KCT_INITTESTLIST(list);
//	KCT_ADDTESTCASE(list,testOK,"a beatiful test!",true);
//	KCT_ADDTESTCASE(list,testOK2,"some other beatiful test!",true);
//	KCT_FREETESTLIST(list);

	//KCT_RUNALLTEST TEST
//	TestList* list=NULL;
//	KCT_INITTESTLIST(list);
//	KCT_ADDTESTCASE(list,testOK,"a beatiful test!",true);
//	KCT_ADDTESTCASE(list,testOK2,"some other beatiful test!",true);
//	KCT_RUNALLTEST(stdout,list,true);


	//kct_addTest TEST

//	kct_addTest(testOK);

//	kct_runAllTest(stdout);

	//Create 100 random number between 0 and 9 and test whether or
	//not the upper and lower bound are not violated.
	void testRandomX(){
		int i;
		int randomNumber;
		for (i=0;i<100;i++){
			randomNumber=rand()%10;
			assertExtremis(0,9,true,true,randomNumber);
		}
	}

	//Test if the power of 2 is greater than 0
	void testPower(){
		int i;
		int num;
		for (i=0;i<100;i++){
			num=rand()%100;
			assertCondition((num*num)>=0);
		}
	}

	void testNumber(){
		assertEqualIntMsg("Number are mismatching",4,4);
		ok();
	}

	int main(){

		//add tests in the function list
		kct_addTest(testRandomX);
		kct_addTest(testPower);
		kct_addTest(testNumber);
		//run the tests
		kct_runAllTest(stdout);
		return 0;
	}

