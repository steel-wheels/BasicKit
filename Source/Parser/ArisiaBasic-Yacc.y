%{

/*
 * Reference: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html
 */

#include "ArisiaBasic.h"
#include "CNParser.h"
#include "CNByteCode.h"
#include "CNBooleanValue.h"
#include "CNNumberValue.h"
#include "CNStringValue.h"
#include <stdio.h>
#include <string.h>

static void yyerror(const char * message) ;

static struct CNValuePool *     s_value_pool = NULL ;
static struct CNCompiler *      s_compiler   = NULL ;

static struct CNVariable
allocateStoreStatement(CNValueType restype, struct CNValue * src) ;
static struct CNVariable
allocateCastExpression(CNValueType dsttype, const struct CNVariable * src) ;

void
CNSetCompilerToSyntaxParser(struct CNCompiler * compiler, struct CNValuePool * vpool)
{
        s_value_pool    = vpool ;
        s_compiler      = compiler ;
}

%}

%locations

%token  IDENTIFIER
%token  LET PRINT STRING
%token  OP_AND OP_OR
%token  INT_VALUE FLOAT_VALUE FALSE_VALUE TRUE_VALUE

%%

%start  statement_list ;

statement_list
        : statement
        | statement_list statement
        ;

statement: LET IDENTIFIER '=' expression
        {
                struct CNStringValue * ident = $2.identifier ;
                struct CNVariable      src   = $4.variable ;
                uint64_t                dstid ;
                if(!CNHasRegisterIdForIdentifier(&dstid, s_compiler, ident)){
                        /* the identifier is not in register table */
                        dstid = CNAllocateFreeRegisterId(s_compiler) ;
                }
                struct CNCodeValue * code = CNAllocateMoveCode(s_value_pool, dstid, src.registerId) ;
                CNAppendCodeToCompiler(s_compiler, code) ;
                CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
                CNReleaseValue(s_value_pool, CNSuperClassOfStringValue(ident)) ;
        }
        | PRINT expression
        {
                struct CNVariable src = $2.variable ;
                struct CNCodeValue * code = CNAllocatePrintCode(s_value_pool, src.registerId) ;
                CNAppendCodeToCompiler(s_compiler, code) ;
                CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        }
        ;

expression
        : logical_and_expression
        {
                $$ = $1 ;
        }
        | expression OP_OR logical_and_expression
        {
                struct CNVariable lvar = allocateCastExpression(CNBooleanType, &($1.variable)) ;
                struct CNVariable rvar = allocateCastExpression(CNBooleanType, &($3.variable)) ;
        }
        ;

logical_and_expression
        : IDENTIFIER
        {
                struct CNStringValue *  ident = $1.identifier ;
                uint64_t                regid ;
                if(CNHasRegisterIdForIdentifier(&regid, s_compiler, ident)){
                        $$.variable = CNMakeVariable(CNStringType, regid) ;
                } else {
                        unsigned int line = CNGetCurrentParsingLine() ;
                        struct CNParseError error = CNMakeUndefinedVariableError(ident, line) ;
                        CNPutParseErrorToCompiler(s_compiler, &error) ;

                        regid = CNAllocateFreeRegisterId(s_compiler) ;
                        $$.variable = CNMakeVariable(CNUnsignedIntType, regid) ;
                }
                CNReleaseValue(s_value_pool, CNSuperClassOfStringValue(ident)) ;
        }
        | TRUE_VALUE
        {
                struct CNBooleanValue * bval ;
                bval = CNAllocateBooleanValue(s_value_pool, true) ;
                $$.variable = allocateStoreStatement(CNBooleanType, CNSuperClassOfBooleanValue(bval)) ;
        }
        | FALSE_VALUE
        {
                struct CNBooleanValue * bval ;
                bval = CNAllocateBooleanValue(s_value_pool, false) ;
                $$.variable = allocateStoreStatement(CNBooleanType, CNSuperClassOfBooleanValue(bval)) ;
        }
        | INT_VALUE
        {
                struct CNUnsignedIntValue * intval ;
                intval = CNAllocateUnsignedIntValue(s_value_pool, $1.unsignedIntValue) ;
                $$.variable = allocateStoreStatement(CNUnsignedIntType, CNSuperClassOfUnsignedIntValue(intval)) ;
        }
        | FLOAT_VALUE
        {
                struct CNFloatValue * fltval ;
                fltval = CNAllocateFloatValue(s_value_pool, $1.floatValue) ;
                $$.variable = allocateStoreStatement(CNFloatType, CNSuperClassOfFloatValue(fltval)) ;
        }
        | STRING
        {
                $$.variable = allocateStoreStatement(CNStringType, CNSuperClassOfStringValue($1.string)) ;
        }

%%

void CNStartParser(void)
{
        yyparse() ;
}

static struct CNVariable
allocateStoreStatement(CNValueType vtype, struct CNValue * src)
{
        uint64_t dstid = CNAllocateFreeRegisterId(s_compiler) ;
        struct CNCodeValue * code = CNAllocateLoadCode(s_value_pool, dstid, src) ;
        CNAppendCodeToCompiler(s_compiler, code) ;
        CNReleaseValue(s_value_pool, src) ;
        CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        return CNMakeVariable(vtype, dstid) ;
}

static struct CNVariable
allocateCastExpression(CNValueType dsttype, const struct CNVariable * src)
{
        CNValueType srctype = src->valueType ;
        if(dsttype == srctype) {
                return *src ;
        } else {
                /* Unexpected variable type */
                unsigned int line = CNGetCurrentParsingLine() ;
                struct CNParseError error = CNMakeCanNotCastError(dsttype, srctype, line) ;
                CNPutParseErrorToCompiler(s_compiler, &error) ;
                return *src ;
        }
}

static void yyerror(const char * message)
{
        struct CNValuePool * vpool = s_compiler->valuePool ;
        struct CNStringValue * str = CNAllocateStringValue(vpool, strlen(message), message) ;

        unsigned int line = CNGetCurrentParsingLine() ;
        struct CNParseError error = CNMakeSyntaxError(str, line) ;
        CNPutParseErrorToCompiler(s_compiler, &error) ;
}
