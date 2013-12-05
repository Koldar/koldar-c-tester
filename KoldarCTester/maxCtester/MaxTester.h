/**
 * \brief header used to implements some test suite for C source files
 * 
 * TODO documentation
 * 
 * \date Nov 27, 2013
 * \author koldar
 */

#ifndef MAXTESTER_H_
#define MAXTESTER_H_

#include <stdbool.h>
#include <stdio.h>

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

/**\def MCT_CONTROLMACRO_TESTSEPARATOR
 *
 * This macro controls the string separator between 2 test function.
 * THe default one is the string "**************"
 *
 */
#ifndef MCT_CONTROLMACRO_TESTSEPARATOR
#	define TESTSEPARATOR "**************"
#else
#	define TESTSEPARATOR MCT_CONTROLMACRO_TESTSEPARATOR
#endif

/**@brief represents the outcome of a test function
 *
 * The outcome of a test function can be:
 *  -* MCT_SUCCESS: the test has ended successfully;
 *  -* MCT_FAIL: something has gone wrong in the test;
 *  -* MCT_SKIPPED: the test was in the list but it was commanded to skip it;
 *  -* MCT_UNKNOWN: The initial value of the field "result" for every test function that has not already been executed
 *
 */
typedef enum MCT_TestResult {
	MCT_FAIL,
	MCT_SUCCESS,
	MCT_SKIPPED,
	MCT_UNKNOWN
} MCT_TestResult;

/**@brief type of the test function.
 *
 */
typedef bool (*TestFunction)(void);

typedef struct TestListElement {
	/**@brief the test case in the particular cell of the list
	 *
	 */
	TestFunction function;
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
	 * Se the datatype TestResult for further details.
	 *
	 */
	MCT_TestResult result;
	/**@brief controls wether or not you want to skip this test
	 *
	 * Set it to true if you want to skip the test, false otherwise
	 */
	bool skip;
	/**@brief points to the next test function
	 *
	 */
	struct TestListElement* next;
} TestListElement;

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

/**@brief adds a new test function in the test list
 *
 * \pre
 *  \li test is already been declared
 * \post
 *  \li test function is stored in the test list
 *  \li errormessage field is set to NULL
 *
 * @param list the test list where to add the test function
 * @param test the function to be added in the test list
 * @param description a brief description of what the test verifies.
 * @param toberun
 *  \li TRUE if you want to insert the testcase in the list and execute it;
 *  \li FALSE if you want to insert the testcase in the list but you want it to skip it;
 */
static void addTestCase(TestList* list,TestFunction test,const char* description, bool toberun);

/**@brief run all the test in the list
 *
 * \pre
 *  \li f open in write mode;
 *  \li list not NULL;
 *
 * @param f the file to write the output on
 * @param list the list holding the various test functions
 * @param postDelete
 *  \li TRUE if you want to automatically call clearTestCase() function	at the end of the test functions;
 *  \li FALSE otherwise;
 */
static void runAllTest(FILE* f,TestList* list, bool postDelete);

/**@brief constructs an empty test list
 *
 * \post
 *  \li an empty testlist is allocated in the memory
 *  \li the head of the list is NULL;
 *  \li the tail of the list is NULL;
 *  \li the size of the list is 0;
 */
static TestList* initTestList();

/**@brief free the memory occupied by the list.
 *
 * \pre
 *  \li list not NULL;
 * \post
 *  \li the memory allocated to every TestListElement pointed by the list is freed.
 *
 * @param list the list to be freed
 */
static void freeTestList(TestList* list);

/**@brief copy a string in another one
 *
 */
static char* copyString(const char* msg);

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

/**@def
 *
 * TODO documentation
 */
#define addTest(function,torun) \
	if (testList==NULL){ \
		testList=initTestList(); \
	} \
	addTestCase(testList,function,#function,torun)

#define FAIL() \
	currentTest->errorMessage=copyString(currentTest->description);\
	return false;

#define FAIL_MSG(message) \
	currentTest->errorMessage=copyString(message);\
	return false

#define OK() \
	return true

#define ASSERT_CONDITION(condition) \
	if (!condition){ \
		return false; \
	}

#define ASSERT_CONDITION_MSG(condition,message) \
	if (!condition){ \
		currentTest->errorMessage=copyString(message); \
		return false; \
	}

#define RUN_TESTS() \
	runAllTest(stdout,testList,true)

#endif /* MAXTESTER_H_ */
