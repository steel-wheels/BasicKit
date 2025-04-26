%{
	/* ArisiaBasic-Yacc.y */
#       include "ArisiaBasic.h"

static struct CNValuePool *     s_valuePool = NULL ;

static int yyerror(char const * str) ;

void
CNSetupParser(struct CNValuePool * vpool)
{
        s_valuePool = vpool ;
}

%}

%token  IDENTIFIER
%token	LET
%token  STRING

%start statement

%%

statement
	: LET
	;

%%

void CNStartParser(void)
{
        yyparse() ;
}

static int
yyerror(char const *str)
{
        CNInterface()->error(str) ;
        CNInterface()->error("\n") ;
        return 0 ;
}

