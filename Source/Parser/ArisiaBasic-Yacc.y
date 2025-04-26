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

/* reserved words */
%token  LET
%token  PRINT

%token  IDENTIFIER
%token  STRING

%start statement

%%

statement
	: PRINT STRING
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

