%{

#include "ArisiaBasic.h"
#include "CNParser.h"
#include "CNByteCode.h"
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

%token  IDENTIFIER LET PRINT STRING _FALSE _TRUE

%%

%start  statement ;

statement: PRINT expression
        {
                struct CNVariable src = $1.variable ;
                struct CNCodeValue * code = CNAllocatePrintCode(s_value_pool, src.registerId) ;
                CNAppendCodeToCompiler(s_compiler, code) ;
                CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        }
        ;

expression: IDENTIFIER
        {
                struct CNStringValue *  ident = $1.identifier ;
                uint64_t                regid ;
                if(CNHasRegisterIdForIdentifier(&regid, s_compiler, ident)){
                        $$.variable = CNMakeVariable(CNStringType, regid) ;
                } else {
                        undefinedVariableReferenceError(ident, yyloc.first_line) ;
                }
                CNReleaseValue(s_value_pool, CNSuperClassOfStringValue(ident)) ;
        }
        | STRING
        {
                struct CNValue * srcval = CNSuperClassOfStringValue($1.string) ;
                uint64_t dstid = CNAllocateFreeRegisterId(s_compiler) ;
                struct CNCodeValue * code = CNAllocateStoreCode(s_value_pool, dstid, srcval) ;
                CNAppendCodeToCompiler(s_compiler, code) ;
                CNReleaseValue(s_value_pool, srcval) ;
                CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        }
        ;

%%

void CNStartParser(void)
{
        yyparse() ;
}
        
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
