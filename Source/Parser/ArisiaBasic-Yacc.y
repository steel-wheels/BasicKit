%{
	/* ArisiaBasic-Yacc.y */
#       include "CNInterface.h"
#       include "CNProgram.h"

static int yyerror(char const * str) ;

static char
yylex(void)
{
        return CNGetCharFromProgram() ;
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

