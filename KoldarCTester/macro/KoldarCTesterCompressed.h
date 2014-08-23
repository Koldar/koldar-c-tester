
#ifndef KOLDARCTESTER_H_
#define KOLDARCTESTER_H_
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#if __STDC_VERSION__ < 199901L
#	if __GNUC__ >= 2
#		define __func__ __FUNCTION__
#	else
# 		define __func__ "<unknown>"
#	endif
#endif
#ifndef KCT_CONTROLMACRO_TESTSEPARATOR
#	define PKCTM_TESTSEPARATOR "**************"
#else
#	define PKCTM_TESTSEPARATOR KCT_CONTROLMACRO_TESTSEPARATOR
#endif
#ifndef KCT_CONTROLMACRO_ERRORMESSAGE_EXPECTED
#	define PKCTM_ERRORMESSAGE_EXPECTED "\nExpected value: "
#else
#	define PKCTM_ERRORMESSAGE_EXPECTED KCT_CONTROLMACRO_ERRORMESSAGE_EXPECTED
#endif
#ifndef KCT_CONTROLMACRO_ERRORMESSAGE_ACTUAL
#	define PKCTM_ERRORMESSAGE_ACTUAL "\nActual value:   "
#else
#	define PKCTM_ERRORMESSAGE_ACTUAL KCT_CONTROLMACRO_ERRORMESSAGE_ACTUAL
#endif
#ifndef KCT_CONTROLMACRO_STRINGBUFFER
#	define PKCTM_STRINGBUFFER 15
#else
#	define PKCTM_STRINGBUFFER KCT_CONTROLMACRO_STRINGBUFFER
#endif
typedef void (*TestFunction)(void);
typedef struct TestListElement {

	TestFunction testFunction;

	const char* description;

	char* errorMessage;

	short result;

	bool skip;

	struct TestListElement* next;
} pkctt_TestListElement;
typedef struct TestList {

	pkctt_TestListElement* head;

	pkctt_TestListElement* tail;

	int size;
} pkctt_TestList;
#define PKCTM_FAIL 0
#define PKCTM_SUCCESS 1
#define PKCTM_SKIPPED 2
#define PKCTM_UNKNOWN 3
#define PKCTM_COPYSTRING(destinationpointer,constantString) \
		destinationpointer=malloc(strlen(constantString)+1); \
		if (destinationpointer==NULL){ \
			fprintf(stderr,"MaxTester.h:PKCTM_COPYSTRING:not enough memory\n"); \
			exit -1; \
		} \
		strcpy((char*)destinationpointer,constantString)
#define PKCTM_INITSTRING(string,num,conversion) \
		string=malloc(PKCTM_STRINGBUFFER); \
		sprintf(string,conversion,num);
#define PKCTM_FREESTRING(string) \
		free(string);
#define PKCTM_STRCAT(destination,stringslength,strings) { \
		\
		int pkctv_stringslength=(stringslength); \
		\
		int pkctv_length=1; \
		\
		int pkctv_i; \
		for (pkctv_i=0; pkctv_i<pkctv_stringslength; pkctv_i++){ \
			pkctv_length+=strlen(strings[pkctv_i]); \
		} \
		destination=malloc(pkctv_length); \
		strcpy(destination,strings[0]); \
		for (pkctv_i=1; pkctv_i<pkctv_stringslength;pkctv_i++){ \
			strcat(destination,strings[pkctv_i]); \
		} \
}
#define PKCTM_INITTESTLIST(list) \
		list=malloc(sizeof(pkctt_TestList)); \
		if (list==NULL){ \
			fprintf(stderr,"MaxTester.c:initTestList:1:Not enough memory\n"); \
			exit -1; \
		} \
		list->head=NULL; \
		list->tail=NULL; \
		list->size=0
#define PKCTM_INITTESTLISTELEMENT(element,_function,_description,toberun) \
		element=malloc(sizeof(pkctt_TestListElement)); \
		if (element==NULL){ \
			fprintf(stderr,"KoldarCTester.h:PKCTM_INITTESTLISTELEMENT:not enough memory"); \
			exit -1; \
		} \
		element->testFunction=_function; \
		PKCTM_COPYSTRING(element->description,_description); \
		element->errorMessage=NULL; \
		element->result=PKCTM_UNKNOWN; \
		element->skip=!toberun; \
		element->next=NULL
#define PKCTM_ADDTESTCASE(_list,_test,_description,_toberun) { \
		pkctt_TestListElement* newadd; \
		PKCTM_INITTESTLISTELEMENT(newadd,_test,_description,_toberun); \
		if (_list->head==NULL){ \
			_list->head=newadd; \
			_list->tail=newadd; \
		}else { \
			_list->tail->next=newadd; \
			_list->tail=newadd; \
		} \
		_list->size++; \
		}
#define PKCTM_FREETESTLIST(_list) { \
		pkctt_TestListElement* tofree=_list->head; \
		pkctt_TestListElement* next; \
		while (tofree!=NULL){ \
			next=tofree->next; \
			free((char*)tofree->description); \
			free((char*)tofree->errorMessage); \
			free(tofree); \
			tofree=next; \
		} \
		free(_list); \
}
#define PKCTM_RUNALLTEST(_f,_list,_postDelete) { \
		pkctt_TestListElement* testToRun=_list->head; \
		int testnumber; \
		int oktest; \
		int skiptest; \
		int failtest; \
		while (testToRun!=NULL){ \
			if (testToRun->skip==true){ \
				testToRun->result=PKCTM_SKIPPED; \
				testToRun=testToRun->next; \
				continue; \
			} \
			pkctv_currentTest=testToRun; \
			fprintf(_f,"%s TEST \"%s\" %s\n",PKCTM_TESTSEPARATOR,testToRun->description,PKCTM_TESTSEPARATOR); \
			fprintf(_f,"\n"); \
			testToRun->testFunction(); \
			if (testToRun->result == PKCTM_UNKNOWN){ \
				testToRun->result=PKCTM_SUCCESS; \
			}else { \
				fprintf(_f,"!!!ERROR!!!\n%s",testToRun->errorMessage); \
			} \
			fprintf(_f,"\n"); \
			testToRun=testToRun->next; \
		} \
		fprintf(_f,"%s FINAL RESULTS %s\n",PKCTM_TESTSEPARATOR,PKCTM_TESTSEPARATOR); \
		fprintf(_f,"\n"); \
		testToRun=_list->head; \
		testnumber=1; \
		oktest=0; \
		failtest=0; \
		skiptest=0; \
		while (testToRun!=NULL){ \
			fprintf(_f,"%i) %s\t",testnumber,testToRun->description); \
			switch (testToRun->result){ \
			case PKCTM_SUCCESS: { \
				fprintf(_f,"SUCCESS"); \
				oktest++; \
				break; \
			} \
			case PKCTM_FAIL: { \
				fprintf(_f,"FAIL"); \
				failtest++; \
				break; \
			} \
			case PKCTM_SKIPPED: { \
				skiptest++; \
				fprintf(_f,"SKIPPED"); \
				break; \
			} \
			case PKCTM_UNKNOWN: { \
				fprintf(_f,"UNKNOWN"); \
				break; \
			} \
			} \
			fprintf(_f,"\n"); \
			testToRun=testToRun->next; \
			testnumber++; \
		} \
		fprintf(_f,"\n"); \
		fprintf(_f,"OK\t%d/%d %%run %2.3f\n",oktest,_list->size,(oktest*100.00)/(_list->size-skiptest)); \
		fprintf(_f,"FAIL\t%d/%d %%run %2.3f\n",failtest,_list->size,(failtest*100.00)/(_list->size-skiptest)); \
		fprintf(_f,"SKIPPED\t%d/%d\n",skiptest,_list->size); \
		if (_postDelete){ \
			PKCTM_FREETESTLIST(_list); \
			_list=NULL; \
		} \
}
pkctt_TestList* pkctv_testList=NULL;
pkctt_TestListElement* pkctv_currentTest;
#define kct_addTestImproved(function,torun) \
		if (pkctv_testList==NULL){ \
			PKCTM_INITTESTLIST(pkctv_testList); \
		} \
		PKCTM_ADDTESTCASE(pkctv_testList,function,#function,torun)
#define kct_addTest(function) \
		kct_addTestImproved(function,true)
#define kct_runAllTests(outputfile) \
		PKCTM_RUNALLTEST(outputfile,pkctv_testList,true)
#define kct_fail() \
		pkctv_currentTest->result=PKCTM_FAIL; \
		return;
#define kct_failMsg(message) \
		PKCTM_COPYSTRING(pkctv_currentTest->errorMessage,message); \
		kct_fail();
#define kct_ok() \
		return;
#define kct_assertCondition(condition) \
		if ((condition)==false){ \
			kct_fail(); \
		}
#define kct_assertConditionMsg(message,condition) \
		if ((condition)==false){ \
			kct_failMsg(message); \
		}
#define kct_assertNotCondition(condition) \
		if ((condition)==true){ \
			kct_fail(); \
		}
#define kct_assertNotConditionMsg(message,condition) \
		if ((condtion)==true){ \
			kct_failMsg(message); \
		}
#define kct_assertEqualStrMsg(optmessage,expected,actual){ \
		\
		char* pkctv_optmessage=(optmessage); \
		\
		char* pkctv_expected=(expected); \
		\
		char* pkctv_actual=(actual); \
		if (strcmp(pkctv_expected,pkctv_actual)!=0){ \
			char* pkctv_array[]={ \
					pkctv_optmessage, \
					PKCTM_ERRORMESSAGE_EXPECTED, \
					"\"", \
					pkctv_expected, \
					"\"", \
					PKCTM_ERRORMESSAGE_ACTUAL, \
					"\"", \
					pkctv_actual, \
					"\"" \
		}; \
		PKCTM_STRCAT( \
				pkctv_currentTest->errorMessage, \
				9, \
				pkctv_array \
		); \
		kct_fail(); \
		} \
}
#define kct_assertEqualStr(expected,actual){ \
		kct_assertEqualStrMsg("",expected,actual) \
}
#define kct_assertEqualPrimitiveMsg(message,type,conversion,expected,actual) { \
		type pkctv_intexpected=(expected); \
		type pkctv_intactual=(actual); \
		if ((pkctv_intactual)!=(pkctv_intexpected)){ \
			\
			char* pkctv_strexp; \
			\
			char* pkctv_stract; \
			PKCTM_INITSTRING(pkctv_strexp,pkctv_intexpected,conversion); \
			PKCTM_INITSTRING(pkctv_stract,pkctv_intactual,conversion); \
			\
			char* pkctv_array[]={ \
					message, \
					PKCTM_ERRORMESSAGE_EXPECTED, \
					pkctv_strexp, \
					PKCTM_ERRORMESSAGE_ACTUAL, \
					pkctv_stract \
			}; \
			PKCTM_STRCAT( \
					pkctv_currentTest->errorMessage, \
					5, \
					pkctv_array \
			); \
			PKCTM_FREESTRING(pkctv_strexp); \
			PKCTM_FREESTRING(pkctv_stract); \
			kct_fail(); \
		} \
}
#define kct_assertNotEqualPrimitiveMsg(message,type,conversion,expected,actual) { \
		type pkctv_intexpected=(expected); \
		type pkctv_intactual=(actual); \
		if ((pkctv_intactual)==(pkctv_intexpected)){ \
			\
			char* pkctv_strexp; \
			\
			char* pkctv_stract; \
			PKCTM_INITSTRING(pkctv_strexp,pkctv_intexpected,conversion); \
			PKCTM_INITSTRING(pkctv_stract,pkctv_intactual,conversion); \
			\
			char* pkctv_array[]={ \
					message, \
					PKCTM_ERRORMESSAGE_EXPECTED, \
					pkctv_strexp, \
					PKCTM_ERRORMESSAGE_ACTUAL, \
					pkctv_stract \
			}; \
			PKCTM_STRCAT( \
					pkctv_currentTest->errorMessage, \
					5, \
					pkctv_array \
			); \
			PKCTM_FREESTRING(pkctv_strexp); \
			PKCTM_FREESTRING(pkctv_stract); \
			kct_fail(); \
		} \
}
#define kct_assertEqualStructMsg(message,type,toStringFunction,compareFunction,expected,actual) { \
		type pkctv_structexpected=expected; \
		type pkctv_structactual=actual; \
		if (compareFunction(pkctv_structexpected,pkctv_structactual)!=0){ \
			char* pkctv_strexp=toStringFunction(pkctv_structexpected); \
			char* pkctv_stract=toStringFunction(pkctv_structactual); \
			char* pkctv_array[]={ \
					message, \
					PKCTM_ERRORMESSAGE_EXPECTED, \
					"\"", \
					pkctv_strexp, \
					"\"", \
					PKCTM_ERRORMESSAGE_ACTUAL, \
					"\"", \
					pkctv_stract, \
					"\"" \
			}; \
			PKCTM_STRCAT( \
					pkctv_currentTest->errorMessage, \
					9, \
					pkctv_array \
			); \
			PKCTM_FREESTRING(pkctv_strexp); \
			PKCTM_FREESTRING(pkctv_stract); \
			kct_fail(); \
		} \
}
#define kct_assertNotEqualStructMsg(message,type,toStringFunction,compareFunction,expected,actual) { \
		type pkctv_structexpected=(expected); \
		type pkctv_structactual=(actual); \
		if (compareFunction(pkctv_structexpected,pkctv_structactual)==0){ \
			char* pkctv_strexp=toStringFunction(pkctv_structexpected); \
			char* pkctv_stract=toStringFunction(pkctv_structactual); \
			char* pkctv_array[]={ \
					message, \
					PKCTM_ERRORMESSAGE_EXPECTED, \
					"\"", \
					pkctv_strexp, \
					"\"", \
					PKCTM_ERRORMESSAGE_ACTUAL, \
					"\"", \
					pkctv_stract, \
					"\"" \
			}; \
			PKCTM_STRCAT( \
					pkctv_currentTest->errorMessage, \
					9, \
					pkctv_array \
			); \
			PKCTM_FREESTRING(pkctv_strexp); \
			PKCTM_FREESTRING(pkctv_stract); \
			kct_fail(); \
		} \
}
#define kct_assertEqualPtrMsg(message,expected,actual) \
		kct_assertEqualPrimitiveMsg(message,void*,"%p",expected,actual)
#define kct_assertNotEqualPtrMsg(message,expected,actual) \
		kct_assertNotEqualPrimitiveMsg(message,void*,"%p",expected,actual)
#define kct_assertIsNotNull(pointer) { \
		void* pkctv_p=(pointer); \
		if (pkctv_p==NULL){ \
			kct_fail(); \
		} \
}
#define kct_assertIsNotNullMsg(message,pointer) { \
		void* pkctv_p=(pointer); \
		if (pkctv_p==NULL){ \
			kct_failMsg(message); \
		} \
}
#define kct_assertIsNull(pointer) { \
		void* pkctv_p=(pointer); \
		if (pkctv_p!=NULL){ \
			kct_fail(); \
		}
#define kct_assertIsNullMsg(message,pointer) { \
		void* pkctv_p=(pointer); \
		if (pkctv_p!=NULL){ \
			kct_failMsg(message); \
		} \
}
#define kct_assertEqualInt(expected,actual) \
		kct_assertEqualPrimitiveMsg("",int,"%d",expected,actual)
#define kct_assertEqualIntMsg(message,expected,actual) \
		kct_assertEqualPrimitiveMsg(message,int,"%d",expected,actual)
#define kct_assertNotEqualIntMsg(message,expected,actual) \
		kct_assertNotEqualPrimitiveMsg(message,int,"%d",expected,actual)
#define kct_assertNotEqualInt(message,expected,actual) \
		kct_assertNotEqualPrimitiveMsg("",int,"%d",expected,actual)
#define kct_assertEqualFloatMsg(message,expected,actual) \
		kct_assertEqualPrimitiveMsg(message,float,"%2.3f",expected,actual)
#define kct_assertNotEqualFloatMsg(message,expected,actual) \
		kct_assertNotEqualPrimitiveMsg(message,float,"%2.3f",expected,actual)
#define kct_assertEqualArrayPrimitiveMsg(message,type,conversion,expectedlength,expected,actual) { \
		type* pkctv_expected=(expected); \
		type* pkctv_actual=(actual); \
		type pkctv_tmp_expected,pkctv_tmp_actual; \
		int pkctv_expectedlength=(expectedlength); \
		int pkctv_i; \
		for (pkctv_i=0;pkctv_i<pkctv_expectedlength;pkctv_i++){ \
			pkctv_tmp_expected=pkctv_expected[pkctv_i]; \
			pkctv_tmp_actual=pkctv_actual[pkctv_i]; \
			if ((pkctv_tmp_actual)!=(pkctv_tmp_expected)){ \
				\
				char* pkctv_strexp; \
				\
				char* pkctv_stract; \
				PKCTM_INITSTRING(pkctv_strexp,pkctv_tmp_expected,conversion); \
				PKCTM_INITSTRING(pkctv_stract,pkctv_tmp_actual,conversion); \
				\
				char* pkctv_array[]={ \
						message, \
						PKCTM_ERRORMESSAGE_EXPECTED, \
						pkctv_strexp, \
						PKCTM_ERRORMESSAGE_ACTUAL, \
						pkctv_stract \
				}; \
				PKCTM_STRCAT( \
						pkctv_currentTest->errorMessage, \
						5, \
						pkctv_array \
				); \
				PKCTM_FREESTRING(pkctv_strexp); \
				PKCTM_FREESTRING(pkctv_stract); \
				kct_fail(); \
			} \
		} \
}
#define kct_assertExtremisMsg(optmessage,type,conversion,lowerbound,upperbound,lowin,upin,actual) { \
		type pkctv_actual=(actual); \
		type pkctv_lowerbound=(lowerbound); \
		type pkctv_upperbound=(upperbound); \
		if ((pkctv_actual<pkctv_lowerbound) || \
				(pkctv_actual>pkctv_upperbound) || \
				((lowin==false) && (pkctv_actual==pkctv_lowerbound)) || \
				((upin==false) && (pkctv_actual==pkctv_upperbound)) \
		){ \
			char* pkctv_str_lowerbound; \
			char* pkctv_str_upperbound; \
			char* pkctv_str_actual; \
			PKCTM_INITSTRING(pkctv_str_lowerbound,pkctv_lowerbound,conversion); \
			PKCTM_INITSTRING(pkctv_str_upperbound,pkctv_upperbound,conversion); \
			PKCTM_INITSTRING(pkctv_str_actual,pkctv_actual,conversion); \
			char* pkctv_array[]={ \
					optmessage, \
					PKCTM_ERRORMESSAGE_EXPECTED, \
					(lowin==true?"[":"]"), \
					pkctv_str_lowerbound, \
					"; ", \
					pkctv_str_upperbound, \
					(upin==true?"]":"["), \
					PKCTM_ERRORMESSAGE_ACTUAL, \
					"\"", \
					pkctv_str_actual, \
					"\"" \
			}; \
			PKCTM_STRCAT(pkctv_currentTest->errorMessage,11,pkctv_array); \
			PKCTM_FREESTRING(pkctv_str_lowerbound); \
			PKCTM_FREESTRING(pkctv_str_upperbound); \
			PKCTM_FREESTRING(pkctv_str_actual); \
			kct_fail(); \
		} \
}
#define kct_assertExtremis(type,conversion,lowerbound,upperbound,lowin,upin,actual) \
		kct_assertExtremisMsg("",type,conversion,lowerbound,upperbound,lowin,upin,actual);
#endif
