%{

#include "ArisiaBasic.h"
#include <stdio.h>

static void yyerror(const char * message) ;

%}

%locations

%token  PRINT

%%



        %start  statement ;

        statement:      PRINT
                ;

%%

static void yyerror(const char * message)
{
        CNInterface()->printf(message) ;
}


