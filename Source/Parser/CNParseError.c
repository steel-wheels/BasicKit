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
                case CNCanNotCastError: {
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
                        CNInterface()->printf("\" at line %u", src->line) ;
                } break ;
                case CNUndefinedVariableError: {
                        CNInterface()->printf("[Error] Undefined variable \"") ;
                        CNPrintValue(CNSuperClassOfStringValue(src->undefinedVariableError.identifier)) ;
                        CNInterface()->printf("\" at line %u", src->line) ;
                } break ;
                case CNCanNotCastError: {
                        const char * dstname = CNValueTypeName((src->canNotCastError).destinationType) ;
                        const char * srcname = CNValueTypeName((src->canNotCastError).sourceType) ;
                        CNInterface()->printf("[Error] Can not cast the value from %s to %s at libe %u",
                                              srcname, dstname, src->line) ;
                } break ;
        }
}

