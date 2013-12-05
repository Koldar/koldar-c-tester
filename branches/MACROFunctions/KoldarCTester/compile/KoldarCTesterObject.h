/**
 *	\file KoldarCTesterObject.h
 *	\brief TODO complete the documentation of KoldarCTesterObject.h
 *
 * 	\date Nov 30, 2013
 * 	\author koldar
 *	\version 1.0
 */
#ifndef KOLDARCTESTEROBJECT_H_
#define KOLDARCTESTEROBJECT_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/** * This macro controls the string separator between 2 test function.
 * THe default one is the string "**************"
 *
 */
#ifndef KCT_CONTROLMACRO_TESTSEPARATOR
	/**\brief Represents the string that separates 2 test function in the output
	 *
	 */
#	define KCT_TESTSEPARATOR "**************"
#else
#	define KCT_TESTSEPARATOR KCT_CONTROLMACRO_TESTSEPARATOR
#endif


//TODO documentaation
#ifndef KCT_CONTROLMACRO_ERRORMESSAGE_EXPECTED
	/**\brief Represents the string to print before the expected value in the error message
	 *
	 * The syntax of the error message is shown here:
	 *
	 * !!!ERROR!!! <errorMessage> <KCT_ERRORMESSAGE_EXPECTED> <value expected>
	 *
	 */
#	define KCT_ERRORMESSAGE_EXPECTED "\nExpected value: "
#else
#	define KCT_ERRORMESSAGE_EXPECTED KCT_CONTROLMACRO_ERRORMESSAGE_EXPECTED
#endif

//TODO documentation
#ifndef KCT_CONTROLMACRO_ERRORMESSAGE_ACTUAL
	//TODO documentation
#	define KCT_ERRORMESSAGE_ACTUAL "\nActual value: "
#else
#	define KCT_ERRORMESSAGE_ACTUAL KCT_CONTROLMACRO_ERRORMESSAGE_ACTUAL
#endif

/**@brief represents the outcome of a test function
 *
 * The outcome of a test function can be:
 *  -* MCT_SUCCESS: the test has ended successfully;
 *  -* MCT_FAIL: something has gone wrong in the test;
 *  -* MCT_SKIPPED: the test was in the list but it was commanded to skip it;
 *  -* MCT_UNKNOWN: The initial value of the field "result" for every test function that has not already been executed
 */

/**\brief Represents the outcome of a test function when the test utterly fails
 */
#define PRIVATE_KCT_FAIL 0
/**\brief Represents the outcome of a test function when the test terminates successfully
 */
#define PRIVATE_KCT_SUCCESS 1
/**\brief Represents the outcome of a test function when the test was to be skipped
 */
#define PRIVATE_KCT_SKIPPED 2
/**\brief Represents the outcome of a test function that has not yet been executed
 */
#define PRIVATE_KCT_UNKNOWN 3

/**\brief type of a generic test function.
 *
 * Every test function that has to be added as a test function must have the following prototype:
 *  -* no formal parameters;
 *  -* return type of type "void;
 */
typedef bool (*TestFunction)(void);

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

/**\brief copy a string in another one
 *
 * \post
 *  \li the string in sourceString is copied in the heap
 *
 *  @param sourceString the original string
 *  @return the pointer where the copy of the stirng will be copied
 */
char* private_kct_copystring(char* sourceString);

/**\brief constructs an empty test list
 *
 * \post
 *  \li an empty testlist is allocated in the memory
 *  \li the head of the list is NULL;
 *  \li the tail of the list is NULL;
 *  \li the size of the list is 0;
 *
 *  @return a pointer to the new TestList structure.
 */
TestList* private_kct_initTestList();


/**\brief Create a new TestListElement in the memory
 *
 * \pre
 *  \li _description must not be NULL
 *  \li _function must not be NULL
 * \post
 *  \li a new TestListElement is allocated in the heap;
 *  \li the field "result" of the newly created TestListElement is KCT_UNKNWON;
 *  \li the field "errorMessage" of the newly created TestListElement is NULL;
 *
 * @param _function the pointer name of the function that will be used as a test function
 * @param _description a string representing the description of the test;
 * @param _toberun TRUE if you want to actually use this test during the test run, FALSE if you want to be added to the
 * 	list but you want to skip it
 * @return a pointer to the newly create element
 */
TestListElement* private_kct_initTestListElement(TestFunction _function,char* _description,bool _toberun);

/**\brief adds a new test function in the test list
 *
 * The variable list is best not to be a complex formula, but a simple variable:
 * in this way the macro function is way quicker
 *
 * \pre
 *  \li _test is already been declared;
 *  \li _function is already been defined;
 *  \li _description is not NULL;
 *  \li _list has been initialized through initTestList();
 * \post
 *  \li _test function is stored in the test list
 *  \li _errormessage field is set to NULL
 *
 * @param _list the test list where to add the test function
 * @param _function the function to be added in the test list
 * @param _description a brief description of what the test verifies.
 * @param _toberun
 *  \li TRUE if you want to insert the testcase in the list and execute it;
 *  \li FALSE if you want to insert the testcase in the list but you want it to skip it;
 */
void private_kct_addTestCase(TestList* _list,TestFunction _function,char* _description,bool _toberun);

/**\brief free the memory occupied by the list.
 *
 * \pre
 *  \li _list not NULL;
 * \post
 *  \li the memory allocated to every TestListElement pointed by the list is freed.
 *
 * @param list the list to be freed
 */
void private_kct_freeTestList(TestList* _list);

/**@brief run all the test in the list
 *
 * \pre
 *  \li _f open in write mode;
 *  \li _list not NULL;
 *  \li _list initialized through initTestList();
 * \post
 *  \li all the tests which have "skip" flag set to FALSE are run
 *
 * @param _f the file to write the output on
 * @param _list the list holding the various test functions
 * @param _postDelete
 *  \li TRUE if you want to automatically call clearTestCase() function	at the end of the test functions;
 *  \li FALSE otherwise;
 */
void private_kct_runAlltests(FILE* _f,TestList** _list,bool _postDelete);

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
TestListElement* currentTest;

/**\brief adds a new function in the test list
 *
 * \pre
 *  \li function has already been defined;
 *  \li function must have no formal parameters;
 *  \li function must return a void type;
 * \post
 *  \li the function is inside the test list
 *
 * @param function the function to include in the test list;
 * @param torun TRUE is you want to actually run the test, FALSE if you want to include it in the list
 * 	but you want to skip it
 *
 */
void kct_addTestImproved(TestFunction function,bool torun);

//TODO documentation
void kct_addTest(TestFunction function);

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
 * @param outputfile where to write the test outcome
 */
void kct_runAllTests(FILE* f);

/**\brief Terminate the test and set the result of it as a failure.
 *
 */
void fail();

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
	currentTest->result=PRIVATE_KCT_SUCCESS; \
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

#endif /* KOLDARCTESTEROBJECT_H_ */
