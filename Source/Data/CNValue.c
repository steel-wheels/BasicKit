/*
 * @file CNValue.c
 * @description Define CNValue data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNValue.h>
#import <BasicKit/CNInterface.h>
#import <BasicKit/CNUtils.h>
#include <string.h>

void
CNInitValuePool(struct CNValuePool * dst, struct CNListPool * lpool)
{
        CNInitScalarPool(&(dst->scalarPool), sizeof(struct CNValue), 1024, lpool) ;
        CNInitArrayPool(&(dst->arrayPool), sizeof(struct CNValue), lpool) ;
}

void
CNFreeValuePool(struct CNValuePool * dst)
{
        CNFreeScalarPool(&(dst->scalarPool)) ;
        CNFreeArrayPool(&(dst->arrayPool)) ;
}

void
CNDumpValuePool(unsigned int indent, const struct CNValuePool * src)
{
        CNDumpIndent(indent) ; CNInterface()->printf("ValuePool\n") ;
        CNDumpScalarPool(indent + 1, &(src->scalarPool)) ;
        CNDumpArrayPool(indent + 1,  &(src->arrayPool)) ;
}

static struct CNValue *
CNValueAllocate(CNValueType vtype, uint32_t size, struct CNValuePool * vpool)
{
        struct CNValueAttribute attr = {
                 .frameLocked           = false,
                 .valueType             = vtype,
                 .referenceCount        = 1,
                 .size                  = size
        } ;
        struct CNValue * newval = CNAllocateScalar(&(vpool->scalarPool)) ;
        newval->attribute       = CNValueAttributeToInt(&attr) ;
        return newval ;
}

struct CNValue *
CNAllocateVoid(struct CNValuePool * pool)
{
        return CNValueAllocate(CNVoidValueType, 0, pool) ;
}

struct CNValue *
CNAllocateChar(char c, struct CNValuePool * pool)
{
        struct CNValue * val = CNValueAllocate(CNCharValueType, sizeof(char), pool) ;
        val->charValue = c ;
        return val ;
}

struct CNValue *
CNAllocateInt64(int64_t num, struct CNValuePool * pool)
{
        struct CNValue * val = CNValueAllocate(CNIntValueType, sizeof(int64_t), pool) ;
        val->int64Value = num ;
        return val ;
}

struct CNValue *
CNAllocateUUInt64(uint64_t num, struct CNValuePool * pool)
{
        struct CNValue * val = CNValueAllocate(CNIntValueType, sizeof(uint64_t), pool) ;
        val->uint64Value = num ;
        return val ;
}

struct CNValue *
CNAllocateFloat(double num, struct CNValuePool * pool)
{
        struct CNValue * val = CNValueAllocate(CNFloatValueType, sizeof(double), pool) ;
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
                struct CNValue * newval = CNValueAllocate(CNStringValueType, len,  pool) ;
                initString(&(newval->stringValue), NULL, CNSTRING_ELEMENT_NUM, str) ;

                const char * nstr = str + CNSTRING_ELEMENT_NUM ;
                struct CNValue * next = CNAllocateString(nstr, len - CNSTRING_ELEMENT_NUM, pool) ;
                (newval->stringValue).next = next ;

                return newval ;
        } else {
                struct CNValue * newval = CNValueAllocate(CNStringValueType, len, pool) ;
                initString(&(newval->stringValue), NULL, len, str) ;
                return newval ;
        }
}

struct CNValue *
CNAllocateArray(uint32_t count, struct CNValuePool * pool)
{
        struct CNValue * val  = CNValueAllocate(CNArrayValueType, count, pool) ;

        struct CNValue * data = CNAllocateArrayData(&(pool->arrayPool), count) ;
        for(unsigned int i=0 ; i<count ; i++){
                struct CNValue * elm = &(data[i]) ;
                CNSetVoidValue(elm, true) ;
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
        struct CNValue * val  = CNValueAllocate(CNDictionaryValueType, 0, pool) ;

        struct CNDictionary dict ;
        dict.next = NULL ;
        for(unsigned int i=0 ; i<CNDICTIONARY_ELEMENT_NUM ; i++){
                dict.elements[i].key   = NULL ;
                dict.elements[i].value = NULL ;
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
                case CNVoidValueType: {
                        result = 0 ;
                } break ;
                case CNCharValueType: {
                        result = s0->charValue - s1->charValue ;
                } break ;
                case CNIntValueType: {
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
                case CNFloatValueType: {
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
                case CNStringValueType: {
                        uint32_t len0 = CNLengthOfString(s0) ;
                        uint32_t len1 = CNLengthOfString(s1) ;
                        result = CNCompareString(len0, &(s0->stringValue), len1, &(s1->stringValue)) ;
                } break ;
                case CNArrayValueType: {
                        result = CNCompareArray(&(s0->arrayValue), &(s1->arrayValue)) ;
                } break ;
                case CNDictionaryValueType: {
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
                case CNVoidValueType:
                case CNCharValueType:
                case CNIntValueType:
                case CNFloatValueType: {
                } break ;
                case CNStringValueType: {
                        CNRetainString(&(dst->stringValue)) ;
                } break ;
                case CNArrayValueType: {
                        CNRetainArray(&(dst->arrayValue)) ;
                } break ;
                case CNDictionaryValueType: {
                        CNRetainDictionary(&(dst->dictionaryValue)) ;
                } break ;
        }

        /* update reference count */
        if(attr.frameLocked){
                attr.referenceCount += 1 ;
                dst->attribute = CNValueAttributeToInt(&attr) ;
        }
}

void
CNReleaseValue(struct CNValuePool * pool, struct CNValue * dst)
{
        struct CNValueAttribute attr = CNIntToValueAttribute(dst->attribute) ;
        if(attr.referenceCount > 1) {
                dst->attribute = CNValueAttributeToInt(&attr) ;
                return ;
        }
        switch(attr.valueType) {
                case CNVoidValueType:
                case CNCharValueType:
                case CNIntValueType:
                case CNFloatValueType: {
                } break ;
                case CNStringValueType: {
                        CNReleaseString(pool, &(dst->stringValue)) ;
                } break ;
                case CNArrayValueType: {
                        CNReleaseArray(pool, &(dst->arrayValue)) ;
                } break ;
                case CNDictionaryValueType: {
                        CNReleaseDictionary(pool, &(dst->dictionaryValue)) ;
                } break ;
        }
        /* release the value itself */
        if(attr.frameLocked){
                CNSetVoidValue(dst, true) ;
        } else {
                CNFreeScalar(&(pool->scalarPool), dst) ;
        }
}

void
CNDumpValue(unsigned int indent, const struct CNValue * src)
{
        struct CNValueAttribute attr = CNIntToValueAttribute(src->attribute) ;
        switch(attr.valueType){
                case CNVoidValueType: {
                        CNDumpIndent(indent) ; CNInterface()->printf("nil\n") ;
                } break ;
                case CNCharValueType: {
                        CNDumpIndent(indent) ;
                        CNInterface()->printf("'%c'\n", (src->charValue)) ;
                } break ;
                case CNIntValueType: {
                        CNDumpIndent(indent) ;
                        CNInterface()->printf("%lld\n", (src->int64Value)) ;
                } break ;
                case CNFloatValueType: {
                        CNDumpIndent(indent) ;
                        CNInterface()->printf("%lf\n", (src->floatValue)) ;
                } break ;
                case CNStringValueType: {
                        CNDumpIndent(indent) ;
                        CNInterface()->printf("\"") ;
                        CNDumpString(attr.size, &(src->stringValue)) ;
                        CNInterface()->printf("\"\n") ;
                } break ;
                case CNArrayValueType: {
                        unsigned int num = attr.size ;
                        CNDumpIndent(indent) ; CNInterface()->printf("%u [\n", num) ;
                        const struct CNValue * values = (src->arrayValue).values ;
                        const struct CNValue * endval = values + num ;
                        for( ; values < endval ; values++){
                                CNDumpValue(indent+1, values) ;
                        }
                        CNDumpIndent(indent) ; CNInterface()->printf("]\n") ;
                } break ;
                case CNDictionaryValueType: {
                        CNDumpIndent(indent) ; CNInterface()->printf("{\n") ;
                        CNDumpDictionary(indent+1, &(src->dictionaryValue)) ;
                        CNDumpIndent(indent) ; CNInterface()->printf("}\n") ;
                } break ;
        }
}

