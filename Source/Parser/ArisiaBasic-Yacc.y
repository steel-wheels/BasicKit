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
static struct CNVariable
allocateConvertStatement(CNValueType dsttype, const struct CNVariable * src) ;

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
                struct CNStringValue *  ident = $2.identifier ;
                struct CNVariable       src   = $4.variable ;
                struct CNVariable       dstvar ;
                if(!CNHasRegisterIdForIdentifier(&dstvar, s_compiler, ident)){
                        /* the identifier is not in register table */
                        uint64_t dstid = CNAllocateFreeRegisterId(s_compiler) ;
                        dstvar = CNMakeVariable(src.valueType, dstid) ;
                        /* register the identifier */
                        CNSetRegisterIdForIdentifier(s_compiler, &dstvar, ident) ;
                }
                struct CNCodeValue * code = CNAllocateMoveCode(s_value_pool, dstvar.registerId, src.registerId) ;
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
                uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ;
                struct CNCodeValue * code = CNAllocateLogicalOrCode(s_value_pool, dstid, lvar.registerId, rvar.registerId) ;
                CNAppendCodeToCompiler(s_compiler, code) ;
                CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
                $$.variable = CNMakeVariable(CNBooleanType, dstid) ;
        }
        ;

logical_and_expression
        : IDENTIFIER
        {
                struct CNStringValue *  ident = $1.identifier ;
                struct CNVariable       srcvar ;
                if(CNHasRegisterIdForIdentifier(&srcvar, s_compiler, ident)){
                        $$.variable = srcvar ;
                } else {
                        unsigned int line = CNGetCurrentParsingLine() ;
                        struct CNParseError error = CNMakeUndefinedVariableError(ident, line) ;
                        CNPutParseErrorToCompiler(s_compiler, &error) ;

                        uint64_t regid = CNAllocateFreeRegisterId(s_compiler) ;
                        $$.variable = CNMakeVariable(CNNullType, regid) ;
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
        if(src->valueType == dsttype){
                return *src ;
        }
        bool              succeeded = false ;
        struct CNVariable result    = *src ;
        struct CNVariable dummyres  = CNMakeVariable(CNBooleanType, src->registerId) ;
        switch(dsttype){
                case CNNullType: {
                        switch(src->valueType){
                                case CNNullType: {
                                        succeeded = true ;
                                        result    = *src ;      // needless to convert
                                } break ;
                                case CNStringType: {
                                        result = allocateConvertStatement(dsttype, src) ;
                                        succeeded = true ;
                                } break ;
                                case CNBooleanType:
                                case CNSignedIntType:
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNBooleanType: {
                        switch(src->valueType){
                                case CNBooleanType: {
                                        succeeded = true ;
                                        result    = *src ;      // needless to convert
                                } break ;
                                case CNStringType: {
                                        result = allocateConvertStatement(dsttype, src) ;
                                        succeeded = true ;
                                } break ;
                                case CNNullType:
                                case CNSignedIntType:
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNSignedIntType: {
                        switch(src->valueType){
                                case CNSignedIntType: {
                                        result    = *src ;      // needless to convert
                                        succeeded = true ;
                                } break ;
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNStringType: {
                                        result = allocateConvertStatement(dsttype, src) ;
                                        succeeded = true ;
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNUnsignedIntType: {
                        switch(src->valueType){
                                case CNUnsignedIntType: {
                                        result    = *src ;      // needless to convert
                                        succeeded = true ;
                                } break ;
                                case CNSignedIntType:
                                case CNFloatType:
                                case CNStringType: {
                                        result = allocateConvertStatement(dsttype, src) ;
                                        succeeded = true ;
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNFloatType: {
                        switch(src->valueType){
                                case CNFloatType: {
                                        result    = *src ;      // needless to convert
                                        succeeded = true ;
                                } break ;
                                case CNUnsignedIntType:
                                case CNSignedIntType:
                                case CNStringType: {
                                        result = allocateConvertStatement(dsttype, src) ;
                                        succeeded = true ;
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }

                } break ;
                case CNStringType: {
                        switch(src->valueType){
                                case CNStringType: {
                                        succeeded = true ;
                                        result    = *src ;      // needless to convert
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNSignedIntType:
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNArrayType: {
                        switch(src->valueType){
                                case CNArrayType: {
                                        succeeded = true ;
                                        result    = *src ;      // needless to convert
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNSignedIntType:
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNStringType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNDictionaryType: {
                        switch(src->valueType){
                                case CNDictionaryType: {
                                        succeeded = true ;
                                        result    = *src ;      // needless to convert
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNSignedIntType:
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNStringType:
                                case CNArrayType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNCodeType: {
                        /* always fail */
                        succeeded = false ;
                        result    = dummyres ;
                } break ;
        }
        if(!succeeded){
                unsigned int line = CNGetCurrentParsingLine() ;
                struct CNParseError error = CNMakeCanNotCastError(dsttype, src->valueType, line) ;
                CNPutParseErrorToCompiler(s_compiler, &error) ;
        }
        return result ;
}

static struct CNVariable
allocateConvertStatement(CNValueType dsttype, const struct CNVariable * src)
{
        uint64_t dstreg = CNAllocateFreeRegisterId(s_compiler) ;
        struct CNCodeValue * code = CNAllocateConvertCode(s_value_pool, dsttype, dstreg,
                              src->valueType, src->registerId) ;
        CNAppendCodeToCompiler(s_compiler, code) ;
        CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        return CNMakeVariable(dsttype, dstreg) ;
}

static void yyerror(const char * message)
{
        struct CNValuePool * vpool = s_compiler->valuePool ;
        struct CNStringValue * str = CNAllocateStringValue(vpool, strlen(message), message) ;

        unsigned int line = CNGetCurrentParsingLine() ;
        struct CNParseError error = CNMakeSyntaxError(str, line) ;
        CNPutParseErrorToCompiler(s_compiler, &error) ;
}
