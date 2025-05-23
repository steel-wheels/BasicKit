/*
 * @file UTCompiler.c
 * @description Defint data structure: CNCompiler
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNCompiler.h"
#include "CNByteCode.h"
#include "CNNumberValue.h"
#include "CNDictionaryValue.h"
#include "CNInterface.h"

void
CNInitCompiler(struct CNCompiler * dst, struct CNValuePool * vpool)
{
        dst->valuePool          = vpool ;
        dst->next_free_regid    = 0 ;
        dst->identifierTable    = CNAllocateDictionaryValue(vpool) ;
        CNInitValueList(&(dst->codeList), vpool) ;
}

void
CNDeinitCompiler(struct CNCompiler * dst)
{
        CNReleaseValue(dst->valuePool, CNSuperClassOfDictionaryValue(dst->identifierTable)) ;
        CNDeinitValueList(&(dst->codeList)) ;
}

bool
CNHasRegisterIdForIdentifier(uint64_t * regid, struct CNCompiler * src, struct CNStringValue * ident)
{
        struct CNValue * value = CNValueForKeyInDictionary(src->identifierTable, ident) ;
        if(value != NULL){
                struct CNUnsignedIntValue * uval = CNCastToUnsignedIntValue(value) ;
                *regid = uval->value ;
                return true ;
        } else {
                return false ;
        }
}

uint64_t
CNAllocateFreeRegisterId(struct CNCompiler * src)
{
        uint64_t regid = src->next_free_regid ;
        src->next_free_regid += 1 ;
        return regid ;
}

void
CNAppendCodeToCompiler(struct CNCompiler * dst, struct CNCodeValue * code)
{
        CNAppendValueToValueList(&(dst->codeList), CNSuperClassOfCodeValue(code)) ;
}

void
CNDumpCodeInCompiler(struct CNCompiler * src)
{
        struct CNList * list = (src->codeList).valueList ;
        for( ; list != NULL ; list = list->next){
                struct CNCodeValue * code = CNCastToCodeValue(list->data) ;
                if(code != NULL){
                        CNPrintByteCode(code) ;
                        CNInterface()->printf("\n") ;
                } else {
                        CNInterface()->printf("[Error] Invalid opcode\n") ;
                }
        }
}
