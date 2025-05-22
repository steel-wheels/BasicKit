%{

#include "ArisiaBasic.h"
#include <stdio.h>

static void yyerror(const char * message) ;

static struct CNValuePool *     s_value_pool = NULL ;
static struct CNCompiler *      s_compiler   = NULL ;

static void
undefinedVariableReferenceError(struct CNStringValue * ident, int lineno) ;

void
CNSetCompilerToSyntaxParser(struct CNCompiler * compiler, struct CNValuePool * vpool)
{
        s_value_pool    = vpool ;
        s_compiler      = compiler ;
}

%}

%locations

%token  IDENTIFIER LET PRINT _FALSE _TRUE

%%

%start  statement ;

statement: PRINT expression
        ;

expression: IDENTIFIER
        {
                uint64_t regid ;
                if(CNAllocateRegisterIdForIdentifier(&regid, s_compiler, $1.identifier)){
                        $$.variable = CNMakeVariable(CNStringType, regid) ;
                } else {
                        undefinedVariableReferenceError($1.identifier, yyloc.first_line) ;
                }
        }
        ;

%%

static void yyerror(const char * message)
{
        CNInterface()->error("[Error] %s\n", message) ;
}

static void
undefinedVariableReferenceError(struct CNStringValue * ident, int lineno)
{
        CNInterface()->printf("[Error] Undefined identifier \"") ;
        CNPrintValue(CNSuperClassOfStringValue(ident)) ;
        CNInterface()->printf("\" declared at line %d\n", lineno) ;
}
