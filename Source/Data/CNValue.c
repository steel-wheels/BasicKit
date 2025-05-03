/*
 * @file CNValue.c
 * @description Define CNValue data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNValue.h>
#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNInterface.h>
#import <BasicKit/CNUtils.h>
#include <string.h>

static struct CNValue *
CNValueAllocate(CNValueType vtype, uint32_t size, struct CNValuePool * vpool)
{
        struct CNValueAttribute attr = {
                 .releasable            = true,
                 .valueType             = vtype,
                 .referenceCount        = 1,
                 .size                  = size
        } ;
        struct CNValue * newval = CNAllocateScalar(vpool) ;
        newval->attribute       = CNValueAttributeToInt(&attr) ;
        return newval ;
}

struct CNValue *
CNAllocateNull(void)
{
        static struct CNValue   s_null_value ;
        static bool             s_initialized = false ;
        if(!s_initialized){
                CNSetNullValue(&s_null_value, false) ;
                s_initialized = true ;
        }
        return &s_null_value ;
}

struct CNValue *
CNAllocateChar(char c, struct CNValuePool * pool)
{
        struct CNValue * val = CNValueAllocate(CNCharType, sizeof(char), pool) ;
        val->charValue = c ;
        return val ;
}

struct CNValue *
CNAllocateSignedInt(int64_t num, struct CNValuePool * pool)
{
        struct CNValue * val = CNValueAllocate(CNSignedIntType, sizeof(int64_t), pool) ;
        val->int64Value = num ;
        return val ;
}

struct CNValue *
CNAllocateUnsignedInt(uint64_t num, struct CNValuePool * pool)
{
        struct CNValue * val = CNValueAllocate(CNUnsignedIntType, sizeof(uint64_t), pool) ;
        val->uint64Value = num ;
        return val ;
}

uint64_t
CNUnsignedIntValue(const struct CNValue * src)
{
        uint64_t result ;
        switch(CNTypeOfValue(src)) {
                case CNUnsignedIntType: {
                        result = src->uint64Value ;
                } break ;
                default: {
                        CNInterface()->error("[Error] Not unsigned int value\n") ;
                        result = 0 ;
                } break ;
        }
        return result ;
}

struct CNValue *
CNAllocateFloat(double num, struct CNValuePool * pool)
{
        struct CNValue * val = CNValueAllocate(CNFloatType, sizeof(double), pool) ;
        val->floatValue = num ;
        return val ;
}

static inline void
initString(struct CNString * dst, struct CNValue * next, size_t len, const char * str)
{
        dst->next = next ;
        strncpy(dst->buffer, str, len) ;
}

struct CNValue *
CNAllocateString(const char * str, uint32_t len, struct CNValuePool * pool)
{
        if(len > CNSTRING_ELEMENT_NUM) {
                struct CNValue * newval = CNValueAllocate(CNStringType, len,  pool) ;
                initString(&(newval->stringValue), NULL, CNSTRING_ELEMENT_NUM, str) ;

                const char * nstr = str + CNSTRING_ELEMENT_NUM ;
                struct CNValue * next = CNAllocateString(nstr, len - CNSTRING_ELEMENT_NUM, pool) ;
                (newval->stringValue).next = next ;

                return newval ;
        } else {
                struct CNValue * newval = CNValueAllocate(CNStringType, len, pool) ;
                initString(&(newval->stringValue), NULL, len, str) ;
                return newval ;
        }
}

struct CNValue *
CNAllocateArray(uint32_t count, struct CNValuePool * pool)
{
        struct CNValue * val  = CNValueAllocate(CNArrayType, count, pool) ;

        struct CNValue ** data   = CNAllocateArrayElements(pool, count) ;
        struct CNValue ** ptr    = data ;
        struct CNValue ** endptr = ptr + count ;
        for( ; ptr < endptr ; ptr++){
                *ptr = CNAllocateNull() ;
        }
        struct CNArray array = {
                .count  = count,
                .values = data
        } ;
        val->arrayValue = array ;
        return val ;
}

struct CNValue *
CNAllocateDictionary(struct CNValuePool * pool)
{
        struct CNValue * val   = CNValueAllocate(CNDictionaryType, 0, pool) ;
        struct CNValue * nullp = CNAllocateNull() ;

        struct CNDictionary dict ;
        dict.next = NULL ;
        for(unsigned int i=0 ; i<CNDICTIONARY_ELEMENT_NUM ; i++){
                dict.elements[i].key   = nullp ;
                dict.elements[i].value = nullp ;
        }
        val->dictionaryValue = dict ;
        return val ;
}

struct CNValue *
CNAllocateOpCode(struct CNValuePool * pool, uint64_t attr, struct CNValue * dst,
                 struct CNValue * src0, struct CNValue * src1)
{
        struct CNValue * val   = CNValueAllocate(CNOpCodeType, 0, pool) ;
        struct CNOpCode opcode = {
                .attribute      = attr,
                .destination    = dst,
                .source0        = src0,
                .source1        = src1
        } ;
        val->opCodeValue = opcode ;
        CNRetainValue(opcode.destination) ;
        CNRetainValue(opcode.source0) ;
        CNRetainValue(opcode.source1) ;
        return val ;
}

struct CNValue *
CNAllocateError(CNErrorCode ecode, struct CNValuePool * pool)
{
        struct CNValue * val   = CNValueAllocate(CNErrorType, 0, pool) ;
        val->errorValue.errorCode = ecode ;
        return val ;
}

int
CNCompareValue(const struct CNValue * s0, const struct CNValue * s1)
{
        struct CNValueAttribute a0 = CNIntToValueAttribute(s0->attribute) ;
        struct CNValueAttribute a1 = CNIntToValueAttribute(s1->attribute) ;
        if(a0.valueType != a1.valueType) {
                return a0.valueType - a1.valueType ;
        }
        int result ;
        switch(a0.valueType){
                case CNNullType: {
                        result = 0 ;
                } break ;
                case CNCharType: {
                        result = s0->charValue - s1->charValue ;
                } break ;
                case CNSignedIntType: {
                        int64_t v0 = s0->int64Value ;
                        int64_t v1 = s1->int64Value ;
                        if(v0 > v1){
                                result = 1 ;
                        } else if(v0 == v1){
                                result = 0 ;
                        } else {
                                result = -1 ;
                        }
                } break ;
                case CNUnsignedIntType: {
                        uint64_t v0 = s0->uint64Value ;
                        uint64_t v1 = s1->uint64Value ;
                        if(v0 > v1){
                                result = 1 ;
                        } else if(v0 == v1){
                                result = 0 ;
                        } else {
                                result = -1 ;
                        }
                } break ;
                case CNFloatType: {
                        double v0 = s0->floatValue ;
                        double v1 = s1->floatValue ;
                        if(v0 > v1){
                                result = 1 ;
                        } else if(v0 == v1){
                                result = 0 ;
                        } else {
                                result = -1 ;
                        }
                } break ;
                case CNStringType: {
                        uint32_t len0 = CNLengthOfString(s0) ;
                        uint32_t len1 = CNLengthOfString(s1) ;
                        result = CNCompareString(len0, &(s0->stringValue), len1, &(s1->stringValue)) ;
                } break ;
                case CNArrayType: {
                        result = CNCompareArray(&(s0->arrayValue), &(s1->arrayValue)) ;
                } break ;
                case CNDictionaryType: {
                        result = CNCompareDictionary(&(s0->dictionaryValue), &(s1->dictionaryValue)) ;
                } break ;
                case CNOpCodeType: {
                        result = CNCompareOpCode(&(s0->opCodeValue), &(s1->opCodeValue)) ;
                } break ;
                case CNErrorType: {
                        result = CNCompareError(&(s0->errorValue), &(s1->errorValue)) ;
                } break ;
        }
        return result ;
}

void
CNRetainValue(struct CNValue * dst)
{
        struct CNValueAttribute attr = CNIntToValueAttribute(dst->attribute) ;

        /* update element */
        switch(attr.valueType) {
                case CNNullType:
                case CNCharType:
                case CNSignedIntType:
                case CNUnsignedIntType:
                case CNFloatType: {
                } break ;
                case CNStringType: {
                        CNRetainString(&(dst->stringValue)) ;
                } break ;
                case CNArrayType: {
                        CNRetainArray(&(dst->arrayValue)) ;
                } break ;
                case CNDictionaryType: {
                        CNRetainDictionary(&(dst->dictionaryValue)) ;
                } break ;
                case CNOpCodeType: {
                        CNRetainOpCode(&(dst->opCodeValue)) ;
                } break ;
                case CNErrorType: {
                        CNRetainError(&(dst->errorValue)) ;
                } break ;
        }

        /* update reference count */
        if(attr.releasable){
                attr.referenceCount += 1 ;
                dst->attribute = CNValueAttributeToInt(&attr) ;
        }
}

void
CNReleaseValue(struct CNValuePool * pool, struct CNValue * dst)
{
        /* release elements */
        struct CNValueAttribute attr = CNIntToValueAttribute(dst->attribute) ;
        switch(attr.valueType){
                case CNNullType:
                case CNCharType:
                case CNSignedIntType:
                case CNUnsignedIntType:
                case CNFloatType: {
                        /* no elements */
                } break ;
                case CNStringType: {
                        CNReleaseNextString(pool, &(dst->stringValue)) ;
                } break ;
                case CNArrayType: {
                        CNReleaseArrayElements(pool, &(dst->arrayValue)) ;
                } break ;
                case CNDictionaryType: {
                        CNReleaseDictionaryElements(pool, &(dst->dictionaryValue)) ;
                } break ;
                case CNOpCodeType: {
                        CNReleaseOpCode(pool, &(dst->opCodeValue)) ;
                } break ;
                case CNErrorType: {
                        CNReleaseError(&(dst->errorValue)) ;
                } break ;
        }

        if(attr.referenceCount == 0){
                CNInterface()->error("[Error] Too much release operation\n") ;
        } else if(attr.referenceCount > 1){
                attr.referenceCount -= 1 ;
                dst->attribute = CNValueAttributeToInt(&attr) ;
        } else { // attr.referenceCount == 0
                /* release context */
                switch(attr.valueType){
                        case CNNullType:
                        case CNCharType:
                        case CNSignedIntType:
                        case CNUnsignedIntType:
                        case CNFloatType: {
                                /* no elements */
                        } break ;
                        case CNStringType: {
                                CNDeinitString(pool, &(dst->stringValue)) ;
                        } break ;
                        case CNArrayType: {
                                CNDeinitArray(pool, &(dst->arrayValue)) ;
                        } break ;
                        case CNDictionaryType: {
                                CNDeinitDictionary(pool, &(dst->dictionaryValue)) ;
                        } break ;
                        case CNOpCodeType: {
                                CNDeinitOpCode(pool, &(dst->opCodeValue)) ;
                        } break ;
                        case CNErrorType: {
                                CNDeinitError(&(dst->errorValue)) ;
                        } break ;
                }
                CNSetNullValue(dst, attr.releasable) ;
                if(attr.releasable){
                        CNFreeScalar(pool, dst) ;
                }
        }
}

void
CNPrintValue(const struct CNValue * src)
{
        struct CNValueAttribute attr = CNIntToValueAttribute(src->attribute) ;
        switch(attr.valueType){
                case CNNullType: {
                        CNInterface()->printf("nil") ;
                } break ;
                case CNCharType: {
                        CNInterface()->printf("'%c'", (src->charValue)) ;
                } break ;
                case CNSignedIntType: {
                        CNInterface()->printf("%lld", (src->int64Value)) ;
                } break ;
                case CNUnsignedIntType: {
                        CNInterface()->printf("%llu", (src->uint64Value)) ;
                } break ;
                case CNFloatType: {
                        CNInterface()->printf("%lf", (src->floatValue)) ;
                } break ;
                case CNStringType: {
                        CNInterface()->printf("\"") ;
                        CNPrintString(CNLengthOfString(src), &(src->stringValue)) ;
                        CNInterface()->printf("\"") ;
                } break ;
                case CNArrayType: {
                        CNInterface()->printf("{") ;
                        uint32_t elmnum = CNNumberOfElementsInArray(src) ;
                        CNPrintArray(elmnum, &(src->arrayValue)) ;
                        CNInterface()->printf("}") ;
                } break ;
                case CNDictionaryType: {
                        CNInterface()->printf("{") ;
                        CNPrintDictionary(&(src->dictionaryValue)) ;
                        CNInterface()->printf("}") ;
                } break ;
                case CNOpCodeType: {
                        CNPrintOpCode(&(src->opCodeValue)) ;
                } break ;
                case CNErrorType: {
                        CNPrintError(&(src->errorValue)) ;
                } break ;
        }
}

