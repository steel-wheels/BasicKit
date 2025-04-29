%{

/* ArisiaBasic-Yacc.y */

#include "ArisiaBasic.h"
#include "CNList.h"

static struct CNParserDB * s_parser_db = NULL ;

void
CNSetupParser(struct CNParserDB * pdb)
{
        s_parser_db = pdb ;
}

static int yyerror(char const * str) ;

%}

/* reserved words */
%token  LET
%token  PRINT

%token  IDENTIFIER
%token  STRING

%start statement

%%

statement
        : PRINT expression
        {
                CNReleaseValue(s_parser_db->valuePool, $2.value) ;
        }
        ;

expression
        : STRING
        {
                CNInterface()->printf("STRING = ") ;
                CNDumpValue(0, $1.value) ;
                $0 = $1 ;
        }
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

