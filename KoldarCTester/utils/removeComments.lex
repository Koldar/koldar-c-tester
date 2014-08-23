	/*declaration of the state IN_COMMENT*/
%x IN_COMMENT
	/*avoid the restart of the program at EOF*/
%option noyywrap

%%

<INITIAL>"/*"		BEGIN(IN_COMMENT);
<INITIAL>\s*"//".+	;
<INITIAL>.		ECHO;
<INITIAL>\n\n+		printf("\n");

<IN_COMMENT>"*/"	BEGIN(INITIAL);
<IN_COMMENT>[^*\n]+	;
<IN_COMMENT>"*"		;
<IN_COMMENT>\n		;

%%

#ifndef __REMOVE_COMMENTS_MAIN__
#define __REMOVE_COMMENTS_MAIN__
int main(){
	yylex();
}
#endif
