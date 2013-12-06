#include "KoldarCTesterObject.h"

char* private_kct_copystring(char* destinationpointer,char* sourceString){
	char* result=malloc(strlen(sourceString)+1);
	if (destinationpointer==NULL){
		fprintf(stderr,"KoldarCTesterObject.c:PRIVATE_KCT_COPYSTRING:not enough memory\n");
		return NULL;
	}
	strcpy(result,sourceString);
	return result;
}

TestList* private_kct_initTestList(){
	TestList* list=malloc(sizeof(TestList));
	if (list==NULL){
		fprintf(stderr,"KoldarCTesterObject.c:initTestList:1:Not enough memory\n");
		return NULL;
	}
	list->head=NULL;
	list->tail=NULL;
	list->size=0;
	return list;
}

TestListElement* private_kct_initTestListElement(TestFunction _function,char* _description,bool _toberun){
	TestListElement* element=malloc(sizeof(TestListElement));
	if (element==NULL){
		fprintf(stderr,"KoldarCTesterObject.h:PRIVATE_KCT_INITTESTLISTELEMENT:not enough memory");
		return NULL;
	}
	element->testFunction=_function;
	element->description=private_kct_copystring(_description);
	element->errorMessage=NULL;
	element->result=PRIVATE_KCT_UNKNOWN;
	element->skip=!_toberun;
	element->next=NULL;
	return element;
}

void private_kct_addTestCase(TestList* _list,TestFunction _function,char* _description,bool _toberun){
	TestListElement* newadd;
	newadd=private_kct_initTestListElement(_function,_description,_toberun);
	if (_list->head==NULL){
		_list->head=newadd;
		_list->tail=newadd;
	}else {
		_list->tail->next=newadd;
		_list->tail=newadd;
	}
	_list->size++;
}

void private_kct_freeTestList(TestList* _list) {
	TestListElement* tofree=_list->head;
	TestListElement* next;
	while (tofree!=NULL){
		next=tofree->next;
		free((char*)tofree->description);
		free((char*)tofree->errorMessage);
		free(tofree);
		tofree=next;
	}
	free(_list);
}

void private_kct_runAlltests(FILE* _f,TestList** _list,bool _postDelete) {
	TestListElement* testToRun=(*_list)->head;
	int testnumber;
	int oktest;
	int skiptest;
	int failtest;
	while (testToRun!=NULL){
		if (testToRun->skip==true){
			testToRun->result=PRIVATE_KCT_SKIPPED;
			testToRun=testToRun->next;
			continue;
		}
		currentTest=testToRun;
		fprintf(_f,"%s TEST \"%s\" %s\n",KCT_TESTSEPARATOR,testToRun->description,KCT_TESTSEPARATOR);
		fprintf(_f,"\n");
		testToRun->testFunction();
		if (testToRun->result == PRIVATE_KCT_UNKNOWN){
			testToRun->result=PRIVATE_KCT_SUCCESS;
		}else {
			fprintf(_f,"!!!ERROR!!! \"%s\"\n",testToRun->errorMessage);
		}
		fprintf(_f,"\n");
		testToRun=testToRun->next;
	}
	fprintf(_f,"%s FINAL RESULTS %s\n",KCT_TESTSEPARATOR,KCT_TESTSEPARATOR);
	fprintf(_f,"\n");
	testToRun=(*_list)->head;
	testnumber=1;
	oktest=0;
	failtest=0;
	skiptest=0;
	while (testToRun!=NULL){
		fprintf(_f,"%i) %s\t",testnumber,testToRun->description);
		switch (testToRun->result){
		case PRIVATE_KCT_SUCCESS: {
			fprintf(_f,"SUCCESS");
			oktest++;
			break;
		}
		case PRIVATE_KCT_FAIL: {
			fprintf(_f,"FAIL");
			failtest++;
			break;
		}
		case PRIVATE_KCT_SKIPPED: {
			skiptest++;
			fprintf(_f,"SKIPPED");
			break;
		}
		case PRIVATE_KCT_UNKNOWN: {
			fprintf(_f,"UNKNOWN");
			break;
		}
		}
		fprintf(_f,"\n");
		testToRun=testToRun->next;
		testnumber++;
	}
	fprintf(_f,"\n");
	fprintf(_f,"OK\t%d/%d %%run %2.3f\n",oktest,(*_list)->size,(oktest*100.00)/((*_list)->size-skiptest));
	fprintf(_f,"FAIL\t%d/%d %%run %2.3f\n",failtest,(*_list)->size,(failtest*100.00)/((*_list)->size-skiptest));
	fprintf(_f,"SKIPPED\t%d/%d\n",skiptest,(*_list)->size);
	if (_postDelete){
		private_kct_freeTestList((*_list));
		*_list=NULL;
	}
}


void kct_addTestImproved(TestFunction function,bool torun){
	if (testList==NULL){
		testList=private_kct_initTestList();
	}
	//FIXME find a way to print the name of the function. Hint: imho i need some sort of Macro definition
	private_kct_addTestCase(testList,function,"function",true);
}

void kct_addTest(TestFunction function){
	kct_addTestImproved(function,true);
}

void kct_runAllTests(FILE* f) {
	private_kct_runAlltests(f,testList,true);
}
