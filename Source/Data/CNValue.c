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
        }

        if(attr.referenceCount > 1){
                attr.referenceCount -= 1 ;
                dst->attribute = CNValueAttributeToInt(&attr) ;
        } else {
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
                        }
                }
                CNSetNullValue(dst, attr.releasable) ;
                if(attr.releasable){
                        CNFreeScalar(pool, dst) ;
                }
        }
}

void
CNDumpValue(unsigned int indent, const struct CNValue * src)
{
        struct CNValueAttribute attr = CNIntToValueAttribute(src->attribute) ;
        switch(attr.valueType){
                case CNNullType: {
                        CNDumpIndent(indent) ; CNInterface()->printf("nil\n") ;
                } break ;
                case CNCharType: {
                        CNDumpIndent(indent) ;
                        CNInterface()->printf("'%c'\n", (src->charValue)) ;
                } break ;
                case CNSignedIntType: {
                        CNDumpIndent(indent) ;
                        CNInterface()->printf("%lld\n", (src->int64Value)) ;
                } break ;
                case CNUnsignedIntType: {
                        CNDumpIndent(indent) ;
                        CNInterface()->printf("%llu\n", (src->uint64Value)) ;
                } break ;
                case CNFloatType: {
                        CNDumpIndent(indent) ;
                        CNInterface()->printf("%lf\n", (src->floatValue)) ;
                } break ;
                case CNStringType: {
                        CNDumpIndent(indent) ;
                        CNInterface()->printf("\"") ;
                        CNDumpString(attr.size, &(src->stringValue)) ;
                        CNInterface()->printf("\"\n") ;
                } break ;
                case CNArrayType: {
                        unsigned int num = attr.size ;
                        CNDumpIndent(indent) ; CNInterface()->printf("%u [\n", num) ;
                        struct CNValue ** ptr    = (src->arrayValue).values ;
                        struct CNValue ** endptr = ptr + num ;
                        for( ; ptr < endptr ; ptr++){
                                struct CNValue * value = *ptr ;
                                if(value != NULL){
                                        CNDumpValue(indent+1, value) ;
                                }
                        }
                        CNDumpIndent(indent) ; CNInterface()->printf("]\n") ;
                } break ;
                case CNDictionaryType: {
                        CNDumpIndent(indent) ; CNInterface()->printf("{\n") ;
                        CNDumpDictionary(indent+1, &(src->dictionaryValue)) ;
                        CNDumpIndent(indent) ; CNInterface()->printf("}\n") ;
                } break ;
        }
}

