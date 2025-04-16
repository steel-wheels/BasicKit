%{
	/* ArisiaBasic-Yacc.y */
#       include "CNInterface.h"

static int yyerror(char const * str) ;

static char
yylex(void)
{
        return '\0' ;
}

%}

%token	LET

%%

statement
	: LET
	;

%%

static int
yyerror(char const *str)
{
        CNInterface()->error(str) ;
        return 0 ;
}

