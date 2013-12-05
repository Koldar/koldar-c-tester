/**
 * \mainpage Koldar C Tester: a simple C tester
 * 
 * \section section01 Introduction
 *
 * The KoldarCTester is a single header that can be used to quickly implement
 * a test case for C code. C++ pure code is not supported though.
 *
 * \section section02 To All People who want to setup it quickly
 *
 * If you're in a hurry and want to setup KoldarCTester now, here the basic example
 * to make it work:
 *
 * \code
 * 	#include "KoldarCTester.h"
 *
 * 	//All test function must have no formal parameters and void as return type
 * 	void testSomething(){
 * 		//code something here
 * 		if (a!=5){
 * 			failMsg("a is not 5!\n");
 * 		}
 * 	}
 *
 * 	void testSomethingElse(){
 * 		//code something here
 * 		if (a>4){
 * 			fail();
 * 		}
 * 	}
 *
 * 	int main(){
 *		kct_addTest(testSomething);
 *		kct_addTest(testSomethingElse);
 *
 *		kct_runAllTest(stdout);
 * 	}
 * \endcode
 *
 *	\section section03 How to use KoldarCTester
 *
 *	Now that all the guys in a hurry are satisfied we can discuss how to correctly
 *	use this header. To use it you have to copy "KoldarCTester.h" within your header paths. After
 *	that you can create a new .c files that will be used as a single test suite: if you want
 *	more test suites, you'll have to create more .c files. A single test file .c is divided into 2 parts:
 *	 -# some test functions;
 *	 -# a main function;
 *
 *	 Basically KoldarCTester is a function list manager: you create some function and then you use
 *	 some header function to add them in a list. After that you can execute them all in one command: the
 *	 header handles all the work for you.
 *	 So, first of all, you can create a new file (call it TestInteger.c fro example) and include the header:
 *
 *	 \code
 *	 	#include "KoldarCTester.h"
 *	 \endcode
 *
 *	 You have to be sure the header is inside in your "include paths" (maybe through the -I compiler directive)
 *	 Secondly you have to create some test functions. For the example, I have created these:
 *
 *	 \code
 *	 	//Create 100 random number between 0 and 9 and test whether or
 *	 	//not the upper and lower bound are not violated.
 *	 	void testRandomX(){
 *			int i;
 *			for (i=0;i<100;i++){
 *				assertExtremis(0,9,true,true,rand()%10);
 *			}
 *	 	}
 *
 *		//Test if the power of 2 is greater than 0
 *	 	void testPower(){
 *	 		int i;
 *	 		int num;
 *	 		for (i=0;i<100;i++){
 *	 			num=rand()%100;
 *				assertCondition(num*num>=0);
 *	 		}
 *	 	}
 *	 \endcode
 *
 *	 Let's skip up for the moment the "assertExtremis" and "assertCondition" part. Now you have created
 *	 all the test you need to verify some feature of your program. Now you need to run these; the main
 *	 function does exactly that:
 *
 *	 \code
 *	 	int main(){
 *	 		//add tests in the function list
 *	 		kct_addTest(testRandomX);
 *	 		kct_addTest(testPower);
 *	 		//run the tests
 *	 		kct_runAllTest(stdout);
 *	 	}
 *	 \endcode
 *
 *	 Now you can build it. The compiler will probably complaints about "addTest" and "kct_runAllTest" do nothings:
 *	 don't trust him, these function are actually pretty useful!
 *	 Now try to run the program: you should get something like this:
 *
 *	 \image html image01.png
 *
 *	 The tests are stored in a list: though it might be not so fast
 *	 (like Bjame Stroudtrup say in http://www.youtube.com/watch?v=YQs6IC-vgmo),
 *	 the list are quite easy to understand even for programmers beginner, so everyone
 *	 can improve freely the header. Moreover, unlike other test suite, this header
 *	 is totally *free of memory leak*. Sure, you can say:
 *	 "well, that's should be obvious!"; well, of course it is, but you'll never
 *	 know what you download out in the internet.
 *
 *	\section section04 Basic Elements in the header
 *
 *	In the header there are basically  important elements:
 *	 -# assertions;
 *	 -# function kct_addTest();
 *	 -# function kct_addTestImproved();
 *	 -# function kct_runAllTest();
 *	 -# variable currentTest;
 *	 -# variable testList;
 *
 *	 "testList" variable represents the list that will store the functions and some other
 *	 data.
 *
 *	 "currentTest" is the test that is running at the moment; it is mainly
 *	 used to lighten the prototype of the function: in this way the developer
 *	 can *focus* on the test development insted of wasting time on understand the
 *	 testing suite.
 *
 *	 kct_addTest() is the main function used to add a function in the list.
 *
 *	 kct_addTestImproved() has the same aim of kct_addTest but with this function
 *	 you can also specify whether or not you want to skip the function.
 *
 *	 kct_runAllTest() is obviously used to execute the test. Note that with this function
 *	 the list is automatically deallocated from the heap: in this way, if you are
 *	 testing memory leak in your code, Koldar C Tester will not produces memory leaks by himself.
 *
 *	\section section05 Assertion
 *
 *	As we have introduce in the previous section, one of the most important things
 *	in the header are assertions. Assertions are shortcuts to test some conditions:
 *	if the condition is met nothing happens; otherwise the test is terminate and the
 *	header alerts you that in the specific test something went wrong.
 *
 *	For example, let assume you want to determinate if a pointer to a FILE returned
 *	by fopen is NULL or not. Normally you do something like this:
 *
 *	\code
 *		FILE* f=fopen("../test/fileToParse.txt","r");
 *		if (f==NULL){
 *			fprintf(stderr,"Cannot open the file!\n");
 *			return;
 *		}
 *	\endcode
 *
 *	This works, but if you have lots of these small control your code readability can
 *	substantly decrease. Assert helps you in that:
 *
 *	\code
 *		FILE* f=fopen("../test/fileToParse.txt","r");
 *		assertNotNullMsg("cannot open the file!\n",f);
 *	\endcode
 *
 *	Smaller and more readable, right? Koldar C Tester let you use a lot of these assertions:
 *	you can read about all of them in the documentation! Generally speaking, however,
 *	assertion are divided into 2 strains:
 *
 *	- <b>pure assertions</b> check only a particular condition;
 *	- <b>message assertion</b> check a particular condition and if it is not met they warn you
 *		with an error message
 *
 *	\section section06 Skip tests
 *
 *	Let assume you have 100 tests regarding a particular big feature of your program.
 *	It is probable that, to execute all 100 tests, your computer needs some time.
 *	If you are sure that some tests (i.e 53 and 67) return a OK status, you might
 *	want to skip them, for not wasting time. Well, you can easily do that using
 *	kct_addTestImproved(someTest,false):
 *
 *	\code
 *		kct_addTestImproved(test51,true);
 *		kct_addTestImproved(test52,true);
 *		kct_addTestImproved(test53,false);
 *		kct_addTestImproved(test54,true);
 *		kct_addTestImproved(test55,true);
 *	\endcode
 *
 *	While the first parameter is again the test function the second
 *	parameter indicates whether or not the computer should actually run
 *	the test: when you want to reexecute the function you can change "false"
 *	to "true".
 *
 *	Sure, you can just comment out the particular "kct_addTest(test53);" line,
 *	but the header let you also handle the skip in this way.
 *
 *	Some notes about how skipping tests influence the result outcomes:
 *	-# the ratio successful tests/Number of tests does count the skipped tests;
 *	-# the ratio failed tests/Number of tests does count the skipped tests;
 *	-# the percentage of successful tests does not count skipped tests;
 *	-# the percentage of failed tests does not count skipped tests;
 *
 *	This means that the percentage of successful tests <b>must always be 100%</b>: if it
 *	isn't, then some tests aren't working. On the other hand the percentage
 *	of failed tests <b>must always be 0%</b>.
 *
 *
 *	\section section07 "Deeper understanding of KoldarCTester.h"
 *
 *	As we have said before, KoldarCTester basically creates a test list and loop over it.
 *
 *	\dot
 *		digraph list {
 *			rankdir="LR";
 *			TestListHead [label="test list head structure\nmetadata"];
 *			A [label="Test 1\n(head)"];
 *			B [label="Test 2"];
 *			C [label="Test 3"];
 *			D [label="Test 4"];
 *			E [label="Test 5"];
 *			F [label="Test 6\n(tail)"];
 *
 *			TestListHead -> A ->B -> C ->D ->E ->F;
 *		}
 *	\endot
 *
 *	Firstly the list (which in the header has type TestList) is NULL. When someone use the function
 *	kct_addTest() or a similar function, the header automatically initialize the metadata of the list
 *	(stored in a TestList structure). This struct has some metadata like:
 *	 \li head (basilar if you want to loop over the list!);
 *	 \li tail (used to speed up the process of appending test at the tail of the list);
 *	 \li size;
 *
 *	When someone use kct_addTest() (or a similar function) the header automatically initialize a new
 *	element of TestListElement containing a pointer of the test functions (it also contains other small
 *	metadata). The newly created struct is then append at the tail of the list. Some of the metadata stored
 *	in a TestListElement is:
 *	\li the test function;
 *	\li result (the outcome of this test);
 *
 *	The process continue over and over again until the main call kct_runAllTest(): when it happens,
 *	the program	will loop through the TestListElements in the list (starting from the head one);
 *	for each of them the header will run the test function and will update some of the metadata, including
 *	the field result: this result will be important at the end of all tests, when the header has
 *	to generate statistics (like the number of test failures).
 *
 *	To allow such compact APIs, Koldar C Tester uses 2 variables:
 *	\li a default TestList object which is used by every function in the header as default TestList: in this
 *	way there's no need for the developer to manually create and initialize such list;
 *	\li a variable currentTest: while the program iterate through out all the list element, every TestListElement
 *	is stored in a temporary variable. In this way the program can exactly know what TestListElement it is handling;
 *
 *
 * \date Nov 27, 2013
 * \author koldar
 * \version 1.0
 */

#ifndef KOLDARCTESTER_H_
#define KOLDARCTESTER_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* as suggested in the gcc manual http://gcc.gnu.org/onlinedocs/gcc/Function-Names.html
 * I have added this preprocessorn commands to increase portability of the code in order
 * to get a function name.
 */
#if __STDC_VERSION__ < 199901L
#	if __GNUC__ >= 2
#		define __func__ __FUNCTION__
#	else
# 		define __func__ "<unknown>"
#	endif
#endif

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@ CONTROL MACRO DEFINITIONS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/**This macro controls the string separator between 2 test function.
 * The default one is the string "**************"
 * Compile your test program with KCT_CONTROLMACRO_TESTSEPARATOR enabled to change the
 * separator between test functions:
 * \code
 *	#define KCT_CONTROLMACRO_TESTSEPARATOR "%%%%%%%%%%%%%%%%"
 * \endcode
 *
 * \image html image02.png
 *
 *
 * \warning{
 * 	To enable this custom behaviour, define KCT_CONTROLMACRO_TESTSEPARATOR <b>before</b>
 * 	including KoldarCTester header. In alternative, you may compile your test suite with a "-D" flag
 * 	enabled.
 * }
 */
#ifndef KCT_CONTROLMACRO_TESTSEPARATOR
/**\brief Represents the string that separates 2 test function in the output
 *
 */
#	define KCT_TESTSEPARATOR "**************"
#else
#	define KCT_TESTSEPARATOR KCT_CONTROLMACRO_TESTSEPARATOR
#endif


/**This macro allows you to change the default string that is printed out after the developer message
 * in the event a test fails. This string will printed only when macro functions like assertEqualXXXMsg()
 * are called; the macro isn't influenting failMsg() or AssertCondMsg() assertion like!
 *
 * If you want to change the default value, define KCT_CONTROLMACRO_ERRORMESSAGE_EXPECTED
 * as your custom string, like in this example:
 *
 * \code
 * 	#define KCT_CONTROLMACRO_ERRORMESSAGE_EXPECTED "\nEXPECTED VALUE"
 * \endcode
 *
 * The default value is: "\nExpected value: ".
 *
 * \warning{
 * 	To enable this custom behaviour, define KCT_CONTROLMACRO_ERRORMESSAGE_EXPECTED <b>before</b>
 * 	including KoldarCTester header. In alternative, you may compile your test suite with a "-D" flag
 * 	enabled.
 * }
 *
 */
#ifndef KCT_CONTROLMACRO_ERRORMESSAGE_EXPECTED
	/**\brief Represents the string to print before the expected value in the error message
	 *
	 * The syntax of the error message is shown here:
	 *
	 * !!!ERROR!!! <errorMessage> <KCT_ERRORMESSAGE_EXPECTED> <value expected> <KCT_ERRORMESSAGE_ACTUAL> <actual value>
	 *
	 */
#	define KCT_ERRORMESSAGE_EXPECTED "\nExpected value: "
#else
#	define KCT_ERRORMESSAGE_EXPECTED KCT_CONTROLMACRO_ERRORMESSAGE_EXPECTED
#endif

/**This macro allows you to change the default string that is printed out after the developer message
 * in the event a test fails. This string will printed only when macro functions like assertEqualXXXMsg()
 * are called; the macro isn't influenting failMsg() or AssertCondMsg() assertion like!
 *
 * If you want to change the default value, define KCT_CONTROLMACRO_ERRORMESSAGE_ACTUAL
 * as your custom string, like in this example:
 *
 * \code
 * 	#define KCT_CONTROLMACRO_ERRORMESSAGE_ACTUAL "\nACTUAL VALUE"
 * \endcode
 *
 * The default value is: "\nActual value: ".
 *
 * \warning{
 * 	To enable this custom behaviour, define KCT_CONTROLMACRO_ERRORMESSAGE_ACTUAL <b>before</b>
 * 	including KoldarCTester header. In alternative, you may compile your test suite with a "-D" flag
 * 	enabled.
 * }
 *
 */
#ifndef KCT_CONTROLMACRO_ERRORMESSAGE_ACTUAL
	/**\brief Represents the string to print before the actual value in the error message
	 *
	 * The syntax of the error message is shown here:
	 *
	 * !!!ERROR!!! <errorMessage> <KCT_ERRORMESSAGE_EXPECTED> <value expected> <KCT_ERRORMESSAGE_ACTUAL> <actual value>
	 *
	 */
#	define KCT_ERRORMESSAGE_ACTUAL "\nActual value: "
#else
#	define KCT_ERRORMESSAGE_ACTUAL KCT_CONTROLMACRO_ERRORMESSAGE_ACTUAL
#endif

/**This macro allows you to change the default size of the buffer used to convert values into strings.
 * The buffer is useful only in assertions like assertEqualXXXMsg().
 *
 * \note {The default value is 15}
 *
 * For example, let assume you want to convert the number 1234567890123456789. Being this number much
 * longer than 15 the header will stringify will wrongly. By defining this custom value,
 * you can easily increase the buffer size, solving the problem.
 *
 * \warning{
 * 	To enable this custom behaviour, define KCT_CONTROLMACRO_STRINGBUFFER <b>before</b>
 * 	including KoldarCTester header. In alternative, you may compile your test suite with a "-D" flag
 * 	enabled.
 * }
 */
#ifndef KCT_CONTROLMACRO_STRINGBUFFER
	/**Represents the size of the buffer used to convert number (float or integer it doesn't matter)
	 * to strings. The constant is mainly used in the macros like PRIVATE_KCT_ITOA or PRIVATE_KCT_FTOA
	 */
#	define KCT_STRINGBUFFER 15
#else
#	define KCT_STRINGBUFFER KCT_CONTROLMACRO_STRINGBUFFER
#endif

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@ STRUCTURES DEFINITIONS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/**\brief type of a generic test function.
 *
 * Every test function that has to be added as a test function must have the following prototype:
 *  -* no formal parameters;
 *  -* return type of type "void;
 */
typedef void (*TestFunction)(void);

/**\struct TestListElement
 * \brief Defines a single element of the list.
 *
 */
typedef struct TestListElement {
	/**@brief the test case in the particular cell of the list
	 *
	 */
	TestFunction testFunction;
	/**@brief points to a brief description of what the test does
	 *
	 */
	const char* description;
	/**@brief represents the string to be filled with an error message.
	 *
	 * When an error occurs in the test function pointed by "function" field
	 * this variable points to the error message which can help solving
	 * the problem.
	 */
	char* errorMessage;
	/**@brief holds the result of the test.
	 *
	 * The result of the test can be one the values stored in TestResult.
	 * See the KCT_RESULTOUTCOME_XXX macro set to have further informations.
	 *
	 */
	short result;
	/**@brief controls whether or not you want to skip this test
	 *
	 * Set it to true if you want to skip the test, false otherwise
	 */
	bool skip;
	/**@brief points to the next test function
	 *
	 */
	struct TestListElement* next;
} TestListElement;

/**\struct TestList
 * \brief Represents the entire list of test functions
 *
 */
typedef struct TestList {
	/**@brief represents the first element of the list.
	 *
	 * You can cycle through out the list using the the field "next"
	 * of the TestListElement
	 */
	TestListElement* head;
	/**@brief represents the last element of the list.
	 *
	 * Used to speed up the append operation.
	 */
	TestListElement* tail;
	/**@brief the number of the total test functions in the list
	 *
	 */
	int size;
} TestList;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ PRIVATE MACRO DEFINITIONS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/**\brief Represents the outcome of a test function when the test utterly fails
 *
 */
#define PRIVATE_KCT_FAIL 0
/**\brief Represents the outcome of a test function when the test terminates successfully
 *
 */
#define PRIVATE_KCT_SUCCESS 1
/**\brief Represents the outcome of a test function when the test was to be skipped
 *
 */
#define PRIVATE_KCT_SKIPPED 2
/**\brief Represents the outcome of a test function that has not yet been executed
 *
 */
#define PRIVATE_KCT_UNKNOWN 3

/**\brief copy a string in another one
 *
 * It it highly suggested that constantString is a constant String or
 * a simple variable. It is not recommended to use as "constantString" a call to
 * a function because it might decrease performances.
 *
 * \pre
 *  \li destinationpointer has not already been initialized with malloc;
 *  \li destinationpointer has to be of type char*;
 * \post
 *  \li the string in constantString is copied in the heap a the addres destinationpointer
 *
 *  @param [char*]destinationpointer the pointer where the copy of the stirng will be copied
 *  @param [char*]constantString the original string
 *
 */
#define PRIVATE_KCT_COPYSTRING(destinationpointer,constantString) \
	destinationpointer=malloc(strlen(constantString)+1); \
	if (destinationpointer==NULL){ \
		fprintf(stderr,"MaxTester.h:PRIVATE_KCT_COPYSTRING:not enough memory\n"); \
		exit -1; \
	} \
	strcpy((char*)destinationpointer,constantString)

/**\brief convert an integer to string
 *
 * \warning {
 * 	It is highly recommended that you use as "string" parameter only a
 * 	simple variable. Complex formula might cause instabilities
 * }
 *
 * Some example of this function use might be:
 * \code
 * 	//converts 4 in a string
 *	PRIVATE_KCT_INITSTRING(string,4,"%d");
 *	//converts 4.45 in a string
 *	PRIVATE_KCT_INITSTRING(string,4.45,"%2.3f");
 *	//converts 4 in a string
 *	PRIVATE_KCT_INITSTRING(string,4,"%lu");
 * \endcode
 *
 * \pre
 *  \li string must not be initialized
 *  \li string must be of type char*
 *  \li num must be of type number; it might be:
 *   -* int
 *   -* short
 *   -* long
 *   -* float
 *   -* double
 *   -* various unsigned
 *  \li conversion must be a constant string;
 *  \li conversion must be a conversion printf format string;
 * \post
 *  \li string contains a string representation of num;
 *  \li string lead to an allocated space of memory that <b>must</b> be free manually with PRIVATE_KCT_FREESTRING();
 *
 *  @param string the pointer that will lead to the string representation of num
 *  @param num the integer number to convert
 */
#define PRIVATE_KCT_INITSTRING(string,num,conversion) \
	string=malloc(KCT_STRINGBUFFER); \
	sprintf(string,conversion,num);

/**\brief free an already allocated string
 *
 * \warning {
 * 	It is highly recommended that you use as "string" parameter only a
 * 	simple variable. Complex formula might cause instabilities
 * }
 *
 * \pre
 *  \li string was allocated with PRIVATE_KCT_INITSTRING;
 * \post
 *  \li string is freed from the memory;
 *
 * @param string the string to be deallocated;
 *
 */
#define PRIVATE_KCT_FREESTRING(string) \
	free(string);

/**\brief compose a new string by concatenating all the strings in the the give array
 *
 * \pre
 *  \li destination must be a char*
 *  \li destination must not be already mallocated
 *  \li string1-5 must be char* (or constant strings)
 * \post
 *  \li destination is a char* pointing to a string computed by concatenating all the
 *  	strings together.
 *
 * @param destination the pointer linking the strings concatenated
 * @param strings the set of string to be concatenated
 */
#define PRIVATE_KCT_COMPOSESTRING(destination,string1,string2,string3,string4,string5) { \
	destination=malloc(1+strlen(string1)+strlen(string2)+strlen(string3)+strlen(string4)+strlen(string5)); \
	strcat(destination,string1); \
	strcat(destination,string2); \
	strcat(destination,string3); \
	strcat(destination,string4); \
	strcat(destination,string5); \
}



/**\brief constructs an empty test list
 *
 * It is highly recommended that you put in list a single variable, not a function
 * returning a TestList*
 *
 * \pre
 *  \li list *has* to be of type TestList*
 *  \li list has not been initialized with malloc
 * \post
 *  \li an empty testlist is allocated in the memory
 *  \li the head of the list is NULL;
 *  \li the tail of the list is NULL;
 *  \li the size of the list is 0;
 *
 *  @param [TestList*] construct a new TestList and store its pointer in this very parameter
 */
#define PRIVATE_KCT_INITTESTLIST(list) \
	list=malloc(sizeof(TestList)); \
	if (list==NULL){ \
		fprintf(stderr,"MaxTester.c:initTestList:1:Not enough memory\n"); \
		exit -1; \
	} \
	list->head=NULL; \
	list->tail=NULL; \
	list->size=0

/**\brief Create a new TestListElement in the memory
 *
 * \pre
 *  \li element has to be of type TestListElement*;
 *  \li element has not already been initialized with malloc;
 *  \li element must be a simple variable (i.e. a,list);
 *  \li testdescription has to be a constant string, like "hello";
 * \post
 *  \li a new TestListElement is allocated in the heap;
 *  \li the field "result" of the newly created TestListElement is KCT_UNKNWON;
 *  \li the field "errorMessage" of the newly created TestListElement is NULL;
 *
 * @param [TestListElement*]element a uninitialized pointer of TestListElement that will be used to point the new structure
 * @param [TestFunction]_function the pointer name of the function that will be used as a test function
 * @param [char*]_description a string representing the description of the test;
 * @param [bool]toberun TRUE if you want to actually use this test during the test run, FALSE if you want to be added to the
 * 	list but you want to skip it
 *
 */
#define PRIVATE_KCT_INITTESTLISTELEMENT(element,_function,_description,toberun) \
	element=malloc(sizeof(TestListElement)); \
	if (element==NULL){ \
		fprintf(stderr,"KoldarCTester.h:PRIVATE_KCT_INITTESTLISTELEMENT:not enough memory"); \
		exit -1; \
	} \
	element->testFunction=_function; \
	PRIVATE_KCT_COPYSTRING(element->description,_description); \
	element->errorMessage=NULL; \
	element->result=PRIVATE_KCT_UNKNOWN; \
	element->skip=!toberun; \
	element->next=NULL

/**\brief adds a new test function in the test list
 *
 * The variable list is best not to be a complex formula, but a simple variable:
 * in this way the macro function is way quicker
 *
 * \pre
 *  \li _test is already been declared
 *  \li _list of type TestList*
 *  \li _test is of type TestFunction
 *  \li _description is a char*
 *  \li _toberun is a bool
 * \post
 *  \li _test function is stored in the test list
 *  \li _errormessage field is set to NULL
 *
 * @param [TestList*]_list the test list where to add the test function
 * @param [TestFunction]_test the function to be added in the test list
 * @param [char*]_description a brief description of what the test verifies.
 * @param [bool]_toberun
 *  \li TRUE if you want to insert the testcase in the list and execute it;
 *  \li FALSE if you want to insert the testcase in the list but you want it to skip it;
 */
#define PRIVATE_KCT_ADDTESTCASE(_list,_test,_description,_toberun) { \
	TestListElement* newadd; \
	PRIVATE_KCT_INITTESTLISTELEMENT(newadd,_test,_description,_toberun); \
	if (_list->head==NULL){ \
		_list->head=newadd; \
		_list->tail=newadd; \
	}else { \
		_list->tail->next=newadd; \
		_list->tail=newadd; \
	} \
	_list->size++; \
}

/**\brief free the memory occupied by the list.
 *
 * \pre
 *  \li _list not NULL;
 *  \li _list is of type TestList*;
 * \post
 *  \li the memory allocated to every TestListElement pointed by the list is freed.
 *
 * @param [TestList*]list the list to be freed
 */
#define PRIVATE_KCT_FREETESTLIST(_list) { \
	TestListElement* tofree=_list->head; \
	TestListElement* next; \
	while (tofree!=NULL){ \
		next=tofree->next; \
		free((char*)tofree->description); \
		free((char*)tofree->errorMessage); \
		free(tofree); \
		tofree=next; \
	} \
	free(_list); \
}

/**@brief run all the test in the list
 *
 * \pre
 *  \li _f open in write mode;
 *  \li _list not NULL;
 *  \li _f is of type FILE*;
 *  \li _list is of type TestList*;
 *  \li _postDelete is of type bool;
 * \post
 *  \li all the tests which have "skip" flag set to FALSE are run
 *
 * @param [FILE*]_f the file to write the output on
 * @param [TestList*]_list the list holding the various test functions
 * @param [bool]_postDelete
 *  \li TRUE if you want to automatically call clearTestCase() function	at the end of the test functions;
 *  \li FALSE otherwise;
 */
#define PRIVATE_KCT_RUNALLTEST(_f,_list,_postDelete) { \
	TestListElement* testToRun=_list->head; \
	int testnumber; \
	int oktest; \
	int skiptest; \
	int failtest; \
	while (testToRun!=NULL){ \
		if (testToRun->skip==true){ \
			testToRun->result=PRIVATE_KCT_SKIPPED; \
			testToRun=testToRun->next; \
			continue; \
		} \
		currentTest=testToRun; \
		fprintf(_f,"%s TEST \"%s\" %s\n",KCT_TESTSEPARATOR,testToRun->description,KCT_TESTSEPARATOR); \
		fprintf(_f,"\n"); \
		testToRun->testFunction(); \
		if (testToRun->result == PRIVATE_KCT_UNKNOWN){ \
			testToRun->result=PRIVATE_KCT_SUCCESS; \
		}else { \
			fprintf(_f,"!!!ERROR!!! %s\n",testToRun->errorMessage); \
		} \
		fprintf(_f,"\n"); \
		testToRun=testToRun->next; \
	} \
	fprintf(_f,"%s FINAL RESULTS %s\n",KCT_TESTSEPARATOR,KCT_TESTSEPARATOR); \
	fprintf(_f,"\n"); \
	testToRun=_list->head; \
	testnumber=1; \
	oktest=0; \
	failtest=0; \
	skiptest=0; \
	while (testToRun!=NULL){ \
		fprintf(_f,"%i) %s\t",testnumber,testToRun->description); \
		switch (testToRun->result){ \
		case PRIVATE_KCT_SUCCESS: { \
			fprintf(_f,"SUCCESS"); \
			oktest++; \
			break; \
		} \
		case PRIVATE_KCT_FAIL: { \
			fprintf(_f,"FAIL"); \
			failtest++; \
			break; \
		} \
		case PRIVATE_KCT_SKIPPED: { \
			skiptest++; \
			fprintf(_f,"SKIPPED"); \
			break; \
		} \
		case PRIVATE_KCT_UNKNOWN: { \
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
		PRIVATE_KCT_FREETESTLIST(_list); \
		_list=NULL; \
	} \
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ VARIABLE USED @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/**Represents the list holding every test.
 * This variable is used to simplify the MACRO prototype
 */
TestList* testList=NULL;
/*FIXME this variable prevents you to create multiple lists:
 * if you have 2 parallel executing lists, with this variable you can't obvously keep
 * trace on what test the 2 lists are executing!
 */
/**Represents the test which is currently running
 *
 */
static TestListElement* currentTest;

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@ PUBLIC MACRO DEFINITIONS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

/**\brief adds a new function in the test list
 *
 * \pre
 *  \li function has already been defined;
 *  \li function must have no formal parameters;
 *  \li function must return a void type;
 * \post
 *  \li the function is inside the test list
 *
 * @param [TestFunction]function the function to include in the test list;
 * @param [bool]torun TRUE is you want to actually run the test, FALSE if you want to include it in the list
 * 	but you want to skip it
 *
 */
#define kct_addTestImproved(function,torun) \
	if (testList==NULL){ \
		PRIVATE_KCT_INITTESTLIST(testList); \
	} \
	PRIVATE_KCT_ADDTESTCASE(testList,function,#function,torun)

/**\brief adds a new function in the test list
 *
 * \pre
 *  \li function has already been defined;
 *  \li function must have no formal parameters;
 *  \li function must return a void type;
 * \post
 *  \li the function is inside the test list
 *
 * @param [TestFunction]function the function to include in the test list;
 */
#define kct_addTest(function) \
	kct_addTestImproved(function,true)

/**\brief run all the test added to the test list.
 *
 * The function run all the tests which has been added to the test list.
 * You can add test function in the test list with the function kct_addTest().
 * Not all the tests will be run, but onyly those tests which are appended with the flag "torun"
 * set to "TRUE"
 *
 * \pre
 *  \li outputfile open in write mode
 * \post
 *  \li test outcome is appended to the outputfile
 *
 * @param [FILE*]outputfile where to write the test outcome
 */
#define kct_runAllTest(outputfile) \
	PRIVATE_KCT_RUNALLTEST(outputfile,testList,true)

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ ASSERTION MACROS @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


/**\brief Terminate the test and set the result of it as a failure.
 *
 */
#define fail() \
	currentTest->result=PRIVATE_KCT_FAIL; \
	return;

/**\brief Terminate the test and set the result of it as a failure.
 *
 * @param [char*]message represents the message to
 * 	display just before the ending of the test.
 *
 */
#define failMsg(message) \
	PRIVATE_KCT_COPYSTRING(currentTest->errorMessage,message); \
	currentTest->result=PRIVATE_KCT_FAIL; \
	return;

/**\brief terminates successfully the current test
 *
 */
#define ok() \
	return;


/**\brief end the test as a failure if the condition is *not* met
 *
 *  @param condition represents the condition to be tested
 */
#define assertCondition(condition) \
	if ((condition)==false){ \
		fail(); \
	}

/**\brief end the test as a failure if the condition IS *not* met
 *
 * In addition to end the test if condition is *not* met, this
 * function will display an error message.
 *
 * @param [char*]message represents the error message to be display if the condition
 * 	is not met
 * @param condition represents the condition to be test
 */
#define assertConditionMsg(message,condition) \
	if ((condition)==false){ \
		failMsg(message); \
	}

//TODO documentation
#define assertNotCondition(condition) \
	if ((condition)==true){ \
		fail(); \
	}

//TODO documentation
#define assertNotConditionMsg(message,condition) \
	if ((condtion)==true){ \
		failMsg(message); \
	}

/**\brief Checks if 2 simple values are equal. If not, send an error
 *
 * The function checks if 2 data are equal. If they are, nothing will happen.
 * If not, the 2 values are converted in string and a error message is thrown
 * at the user. The error is so composed:
 *  -# message parameter;
 *  -# KCT_ERRORMESSAGE_EXPECTED;
 *  -# expected parameter converted into string;
 *  -# KCT_ERRORMESSAGE_ACTUAL;
 *  -# actual parameter converted into string;
 *
 * The function can manage only simple data: they include:
 *  \li int;
 *  \li short;
 *  \li long;
 *  \li float;
 *  \li double;
 *  \li unsigned variations;
 *  \li pointer variations (check the very pointers, not the structure pointed by them);
 *  \li enumerations;
 *
 * \warning {While stirng and structure can be inserted as values to be checked,
 * the function can't compare them with ease. If you have to compare strings or structures,
 * please use PRIVATE_KCT_ASSERTSTRINGEQUAL or PRIVATE_KCT_ASSERTSTRUCTEQUAL}
 *
 * \pre
 *  \li message is of type char* (or a string);
 *  \li type is a simple type;
 *  \li conversion is a formatter string, like "%d";
 *  \li expected is a value of type type;
 *  \li actual is a value of type type;
 *
 * @param [char*]message represents a custom message to prepend to the error message;
 * @param type represents the type of expected and actual values;
 * @param conversion represents the formatter string used to format the stirng representations of expected and actual;
 * @param expected the value that the developer expects to get;
 * @param actual the very value received
 */
#define assertEqualPrimitiveMsg(message,type,conversion,expected,actual) { \
	type intexpected=(expected); \
	type intactual=(actual); \
	if ((intactual)!=(intexpected)){ \
		char* str_exp; \
		char* str_act; \
		PRIVATE_KCT_INITSTRING(str_exp,intexpected,conversion); \
		PRIVATE_KCT_INITSTRING(str_act,intactual,conversion); \
		PRIVATE_KCT_COMPOSESTRING(currentTest->errorMessage, \
			message, \
			KCT_ERRORMESSAGE_EXPECTED, \
			str_exp, \
			KCT_ERRORMESSAGE_ACTUAL, \
			str_act); \
		PRIVATE_KCT_FREESTRING(str_exp); \
		PRIVATE_KCT_FREESTRING(str_act); \
		fail(); \
	} \
}

/**\brief Checks if 2 simple values are not equal. If not, send an error
 *
 * The function checks if 2 data are different. If they are, nothing will happen.
 * If not, the 2 values are converted in string and a error message is thrown
 * at the user. The error is so composed:
 *  -# message parameter;
 *  -# KCT_ERRORMESSAGE_EXPECTED;
 *  -# expected parameter converted into string;
 *  -# KCT_ERRORMESSAGE_ACTUAL;
 *  -# actual parameter converted into string;
 *
 * The function can manage only simple data: they include:
 *  \li int;
 *  \li short;
 *  \li long;
 *  \li float;
 *  \li double;
 *  \li unsigned variations;
 *  \li pointer variations (check the very pointers, not the structure pointed by them);
 *  \li enumerations;
 *
 * \warning {While stirng and structure can be inserted as values to be checked,
 * the function can't compare them with ease. If you have to compare strings or structures,
 * please use PRIVATE_KCT_ASSERTSTRINGEQUAL or PRIVATE_KCT_ASSERTSTRUCTEQUAL}
 *
 * \pre
 *  \li message is of type char* (or a string);
 *  \li type is a simple type;
 *  \li conversion is a formatter string, like "%d";
 *  \li expected is a value of type type;
 *  \li actual is a value of type type;
 *
 * @param [char*]message represents a custom message to prepend to the error message;
 * @param type represents the type of expected and actual values;
 * @param conversion represents the formatter string used to format the string representations of expected and actual;
 * @param expected the value that the developer expects to get;
 * @param actual the very value received
 */
#define assertNotEqualPrimitiveMsg(message,type,conversion,expected,actual) { \
	type intexpected=(expected); \
	type intactual=(actual); \
	if ((intactual)==(intexpected)){ \
		char* str_exp; \
		char* str_act; \
		PRIVATE_KCT_INITSTRING(str_exp,intexpected,conversion); \
		PRIVATE_KCT_INITSTRING(str_act,intactual,conversion); \
		PRIVATE_KCT_COMPOSESTRING(currentTest->errorMessage, \
			message, \
			KCT_ERRORMESSAGE_EXPECTED, \
			str_exp, \
			KCT_ERRORMESSAGE_ACTUAL, \
			str_act); \
		PRIVATE_KCT_FREESTRING(str_exp); \
		PRIVATE_KCT_FREESTRING(str_act); \
		fail(); \
	} \
}

//TODO documentation
#define assertEqualIntMsg(message,expected,actual) \
	assertEqualPrimitiveMsg(message,int,"%d",expected,actual)

//TODO documentation
#define assertNotEqualIntMsg(message,expected,actual) \
	assertNotEqualPrimitiveMsg(message,int,"%d",expected,actual)

//TODO documentation
#define assertEqualFloatMsg(message,expected,actual) \
	assertEqualPrimitiveMsg(message,float,"%2.3f",expected,actual)

//TODO documentation
#define assertNotEqualFloatMsg(message,expected,actual) \
	assertNotEqualPrimitiveMsg(message,float,"%2.3f",expected,actual)

//TODO documentation
#define assertEqual(expected,actual) \
	if ((expected)!=(actual)) { \
		return false; \
	}

//TODO documentation
#define assertEqualMsg(message,expected,actual) \
	if ((expected)!=(actual)){ \
		failMsg(message); \
	}

//TODO documentation
#define assertNotEqual(expected,actual) \
	if ((expected)==(actual)) { \
		fail(); \
	}

//TODO documentation
#define assertNotEqualMsg(message,expected,actual) \
	if ((expected)==(actual)) { \
		failMsg(message); \
	}

//TODO documentation
#define assertNotNull(pointer) \
	if (pointer==NULL){ \
		fail(); \
	}

//TODO documentation
#define assertNotNullMsg(message,pointer) \
	if (pointer==NULL){ \
		failMsg(message); \
	}

//TODO documentation
#define assertIsNull(pointer) \
	if (pointer!=NULL){ \
		fail(); \
	}

//TODO documentation
#define assertIsNullMsg(message,pointer) \
	if (pointer!=NULL){ \
		failMsg(message); \
	}

//TODO documentation
#define assertExtremis(lowerbound,upperbound,lowin,upin,actual) \
	if (actual<lowerbound){ \
		fail(); \
	} \
	if (actual>upperbound){ \
		fail(); \
	} \
	if ((lowin==false)&&(actual==lowerbound)){ \
		fail(); \
	} \
	if ((upin==false)&&(actual==upperbound)) { \
		fail(); \
	}

//FIXME lowerbound,upperbound,lowin,upin,actual must be stored in variable... if they has random components, the test is ill!
//TODO documentation
#define assertExtremisMsg(message,lowerbound,upperbound,lowin,upin,actual) \
	if (actual<lowerbound){ \
		failMsg(message); \
	} \
	if (actual>upperbound){ \
		failMsg(message); \
	} \
	if ((lowin==false)&&(actual==lowerbound)){ \
		failMsg(message); \
	} \
	if ((upin==false)&&(actual==upperbound)) { \
		failMsg(message); \
	}

#endif /* KOLDARCTESTER_H_ */