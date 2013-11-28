#include "MaxTester.h"
#include <stdlib.h>

static TestList* initTestList(){
	TestList* result=malloc(sizeof(TestList));
	if (result==NULL){
		fprintf(stderr,"MaxTester.c:initTestList:1:Not enough memory\n");
		return NULL;
	}
	result->head=NULL;

	result->tail=NULL;
	result->size=0;

	return result;
}

static void addTestCase(TestList* list,TestFunction test,const char* description, bool toberun){

	char* enddescription=malloc(strlen(description)+1);
	if (enddescription==NULL){
		fprintf(stderr,"MaxTester.c:addTestCase:not enough memory to copy the description.\n");
		return;
	}
	strcpy(enddescription,description);

	TestListElement* newadd=initTestList();
	newadd->function=test;
	newadd->next=NULL;
	newadd->description=enddescription;
	newadd->skip=toberun==true?MCT_UNKNOWN:MCT_SKIPPED;
	newadd->errorMessage=NULL;

	list->tail->next=newadd;
}

static void freeTestList(TestList* list){
	TestListElement* tofree=list->head;
	TestListElement* next;
	//free all the elements of the list
	while (tofree!=NULL){
		next=tofree->next;
		free(tofree->description);
		free(tofree->errorMessage);
		free(tofree);
		tofree=next;
	}
	//free the single structure TestList
	free(list);
}

static void runAllTest(FILE* f,TestList* list, bool postDelete){
	TestListElement* testToRun=list->head;
	int testnumber;
	int oktest;
	int skiptest;
	int failtest;

	while (testToRun!=NULL){
		if (testToRun->skip){
			testToRun=testToRun->next;
			continue;
		}
		currentTest=testToRun;
		//TODO find a way to print out the function name
		fprintf(f,"%s TEST \"%s\" %s\n",TESTSEPARATOR,testToRun->description,TESTSEPARATOR);
		fprintf(f,"\n");
		testToRun->result=testToRun->function()?MCT_SUCCESS:MCT_FAIL;
		fprintf(f,"\n");
		testToRun=testToRun->next;
	}
	fprintf(f,"%s FINAL RESULTS %s\n",TESTSEPARATOR,TESTSEPARATOR,TESTSEPARATOR);
	fprintf(f,"\n");

	testToRun=list->head;
	testnumber=1;
	oktest=0;
	failtest=0;
	skiptest=0;
	while (testToRun!=NULL){
		fprintf(f,"%i) %s\t",testnumber,testToRun->description);
		switch (testToRun->result){
		case MCT_SUCCESS: {
			fprintf(f,"SUCCESS");
			oktest++;
			break;
		}
		case MCT_FAIL: {
			fprintf(f,"FAIL");
			failtest++;
			break;
		}
		case MCT_SKIPPED: {
			skiptest++;
			fprintf(f,"SKIPPED");
			break;
		}
		case MCT_UNKNOWN: {
			fprintf(f,"UNKNOWN");
			break;
		}
		}
		fprintf(f,"\n");
	}
	fprintf(f,"\n");
	fprintf(f,"OK\t%d/%d \%run %2.2f\n",oktest,list->size,(oktest+0.0)/(list->size-skiptest));
	fprintf(f,"FAIL\t%d/%d \%run %2.2f\n",failtest,list->size,(failtest+0.0)/(list->size-skiptest));
	fprintf(f,"SKIPPED\t%d/%d\n",skiptest,list->size);

	if (postDelete){
		freeTestList(list);
	}
}

static char* copyString(const char* msg){
	char* result=malloc(strlen(msg)+1);
	if (result==NULL){
		fprintf(stderr,"MaxTester.c:copyString:not enough memory\n");
		return NULL;
	}
	strcpy(result,msg);
	return result;
}
