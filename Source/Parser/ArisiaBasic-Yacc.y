%{

#include "ArisiaBasic.h"
#include "CNParser.h"
#include "CNByteCode.h"
#include "CNNumberValue.h"
#include <stdio.h>

static void yyerror(const char * message) ;

static struct CNValuePool *     s_value_pool = NULL ;
static struct CNCompiler *      s_compiler   = NULL ;

static struct CNVariable
allocateStoreStatement(struct CNValue * src) ;

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

%token  IDENTIFIER LET PRINT STRING
%token  INT_VALUE _FALSE _TRUE

%%

%start  statement_list ;

statement_list
        : statement
        | statement_list statement
        ;

statement: PRINT expression
        {
                struct CNVariable src = $2.variable ;
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
        | INT_VALUE
        {
                struct CNUnsignedIntValue * intval ;
                intval = CNAllocateUnsignedIntValue(s_value_pool, $1.unsignedIntValue) ;
                $$.variable = allocateStoreStatement(CNSuperClassOfUnsignedIntValue(intval)) ;
        }
        | STRING
        {
                $$.variable = allocateStoreStatement(CNSuperClassOfStringValue($1.string)) ;
        }
        ;

%%

void CNStartParser(void)
{
        yyparse() ;
}

static struct CNVariable
allocateStoreStatement(struct CNValue * src)
{
        uint64_t dstid = CNAllocateFreeRegisterId(s_compiler) ;
        struct CNCodeValue * code = CNAllocateLoadCode(s_value_pool, dstid, src) ;
        CNAppendCodeToCompiler(s_compiler, code) ;
        CNReleaseValue(s_value_pool, src) ;
        CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        return CNMakeVariable(CNStringType, dstid) ;
}

static void yyerror(const char * message)
{
        unsigned int line = CNGetCurrentParsingLine() ;
        CNInterface()->error("[Error] %s at line %u\n", message, line) ;
}

static void
undefinedVariableReferenceError(struct CNStringValue * ident, int lineno)
{
        CNInterface()->printf("[Error] Undefined identifier \"") ;
        CNPrintValue(CNSuperClassOfStringValue(ident)) ;
        CNInterface()->printf("\" declared at line %d\n", lineno) ;
}
