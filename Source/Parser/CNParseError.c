/*
 * @file CNParseErrpr.c
 * @description Define CNParseError data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNParseError.h"
#include "CNInterface.h"

void
CNDeinitParseError(struct CNValuePool * vpool, struct CNParseError * dst)
{
        switch(dst->type){
                case CNNoParseError: {
                        /* nothing have to release */
                } break ;
                case CNSyntaxError: {
                        CNReleaseValue(vpool,
                                       CNSuperClassOfStringValue((dst->syntaxError).message)) ;
                } break ;
                case CNUndefinedVariableError: {
                        CNReleaseValue(vpool,
                                       CNSuperClassOfStringValue((dst->undefinedVariableError).identifier)) ;
                } break ;
                case CNUnexpectedTypeError:
                case CNIntDataRequiredError:
                case CNFloatDataRequiredError:
                case CNCanNotCastError:
                case CNUnmatchedTypesError: {
                        /* nothing have to release */
                } break ;
        }
}

void
CNPrintParseError(const struct CNParseError * src)
{
        switch(src->type){
                case CNNoParseError: {
                        /* nothing to print */
                } break ;
                case CNSyntaxError: {
                        CNInterface()->printf("[Error] ") ;
                        CNPrintValue(CNSuperClassOfStringValue(src->syntaxError.message)) ;
                        CNInterface()->printf("\" at line %u\n", src->line) ;
                } break ;
                case CNUndefinedVariableError: {
                        CNInterface()->printf("[Error] Undefined variable \"") ;
                        CNPrintValue(CNSuperClassOfStringValue(src->undefinedVariableError.identifier)) ;
                        CNInterface()->printf("\" at line %u\n", src->line) ;
                } break ;
                case CNUnexpectedTypeError: {
                        const char * typename = CNValueTypeName((src->unexpectedTypeError).type) ;
                        CNInterface()->printf("[Error] Data type \"%s\" is unexpected for the expression ", typename) ;
                        CNInterface()->printf("at line %u\n", src->line) ;
                } break ;
                case CNCanNotCastError: {
                        const char * dstname = CNValueTypeName((src->canNotCastError).destinationType) ;
                        const char * srcname = CNValueTypeName((src->canNotCastError).sourceType) ;
                        CNInterface()->printf("[Error] Can not cast the value from %s to %s at libe %u\n",
                                              srcname, dstname, src->line) ;
                } break ;
                case CNUnmatchedTypesError: {
                        const char * leftname  = CNValueTypeName((src->unmatchedTypesError).leftType) ;
                        const char * rightname = CNValueTypeName((src->unmatchedTypesError).rightType) ;
                        CNInterface()->printf("[Error] Unmatch data type for binary operation left:%s, right:%s at libe %u\n",
                                              leftname, rightname, src->line) ;
                } break ;
                case CNIntDataRequiredError: {
                        const char * opstr = CNArithmeticOperationName((src->intDataRequiredError).operation) ;
                        CNInterface()->printf("[Error] Integer data is required for operation %s\n", opstr) ;
                } break ;
                case CNFloatDataRequiredError: {
                        const char * opstr = CNArithmeticOperationName((src->floatDataRequiredError).operation) ;
                        CNInterface()->printf("[Error] Float data is required for operation %s\n", opstr) ;
                } break ;
        }
}

