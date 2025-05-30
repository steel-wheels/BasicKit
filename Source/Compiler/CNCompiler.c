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

static inline struct CNVariable
intToVariable(uint64_t value)
{
        uint64_t vtype = value & CNValueTypeMask ;
        uint64_t regid = value >> CNValueTypeMaskBits ;
        return CNMakeVariable((CNValueType) vtype, regid) ;
}

static inline uint64_t
variableToInt(const struct CNVariable * src)
{
        uint64_t vtype = src->valueType & CNValueTypeMask ;
        uint64_t regid = ((uint64_t) src->registerId) << CNValueTypeMaskBits ;
        return regid | vtype ;
}

bool
CNHasRegisterIdForIdentifier(struct CNVariable * dstvar, struct CNCompiler * src, struct CNStringValue * ident)
{
        struct CNValue * value = CNValueForKeyInDictionary(src->identifierTable, ident) ;
        if(value != NULL){
                struct CNUnsignedIntValue * uval = CNCastToUnsignedIntValue(value) ;
                *dstvar = intToVariable(uval->value) ;
                return true ;
        } else {
                return false ;
        }
}

void
CNSetRegisterIdForIdentifier(struct CNCompiler * dst, const struct CNVariable *  srcvar, struct CNStringValue * ident)
{
        struct CNValuePool * vpool = dst->valuePool ;
        struct CNUnsignedIntValue * val = CNAllocateUnsignedIntValue(vpool, variableToInt(srcvar)) ;
        CNSetValueToDictionary(vpool, dst->identifierTable, ident, CNSuperClassOfUnsignedIntValue(val)) ;
        CNReleaseValue(vpool, CNSuperClassOfUnsignedIntValue(val)) ;
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
CNPutParseErrorToCompiler(struct CNCompiler * dst, struct CNParseError * src)
{
        CNPrintParseError(src) ;
        CNDeinitParseError(dst->valuePool, src) ;
}

