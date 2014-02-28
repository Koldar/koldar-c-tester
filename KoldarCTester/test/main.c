#include "KoldarCTester.h"

typedef struct Struct{
	int a;
	char* b;
} Struct;

int equalStruct(Struct s1,Struct s2){
	return ((s1.a==s2.b) && (strcmp(s1.b,s2.b)==0))?0:1;
}

char* Struct2String(Struct _s){
	char* result=malloc(10);
	strcpy(result,_s.b);
	return result;
}

void test_PRIVATE_KCT_STRCAT(){
	char* destination=0x0;
	char* array[]={"hello"," ","world!"," It is good ","to be here!"};
	//PRIVATE_KCT_STRCAT(destination,5,array);
	PRIVATE_KCT_STRCAT(destination,5,array);
	kct_assertEqualStrMsg("personal message","hello world! Itis good to be here!",destination);
	free(array[0]);
	free(array[1]);
	free(array[2]);
	free(array[3]);
	free(array[4]);
	free(destination);
}

void test_kct_assertEqualIntMsg(){
	int a=5;
	int b=4;

	kct_assertEqualIntMsg("personale message",a,b);
}

void test_kct_assertNotEqualIntMsg(){
	int a=5;
	int b=5;

	kct_assertNotEqualIntMsg("",a,b);
}

void test_kct_assertEqualStructMsg(){
	Struct a;
	a.a=5;
	a.b="ciao";
	Struct b;
	b.a=4;
	b.b="ciao";

	kct_assertEqualStructMsg("",Struct,Struct2String,equalStruct,a,b);
}

void test_kct_assertExtremisMsg(){

	float a=3.5;
	unsigned int b=7;
	long c=3;

	kct_assertExtremisMsg("personal message",float,"%f",a,b,false,false,c);

}

void testOK(){
	kct_ok();
}

int main(){

	kct_addTest(testOK);
	kct_addTest(test_PRIVATE_KCT_STRCAT);
	kct_addTest(test_kct_assertEqualIntMsg);
	kct_addTest(test_kct_assertNotEqualIntMsg);
	kct_addTest(test_kct_assertEqualStructMsg);
	kct_addTest(test_kct_assertExtremisMsg);
	kct_runAllTests(stdout);

	return 0;
}

