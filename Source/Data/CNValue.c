/*
 * @file CNValue.c
 * @description Define CNValue data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNValue.h>
#import <BasicKit/CNUtils.h>
#include <string.h>
#include <stdio.h>

void
CNInitValuePool(struct CNValuePool * dst, struct CNListPool * lpool)
{
        CNInitElementPool(&(dst->elementPool), sizeof(struct CNValue), 1024, lpool) ;
        CNInitArrayPool(&(dst->arrayPool), sizeof(struct CNValue), lpool) ;
}

void
CNFreeValuePool(struct CNValuePool * dst)
{
        CNFreeElementPool(&(dst->elementPool)) ;
        CNFreeArrayPool(&(dst->arrayPool)) ;
}

static struct CNValue *
CNValueAllocate(CNValueType vtype, uint32_t size, struct CNValuePool * vpool)
{
        struct CNValue * newval = CNAllocateElement(&(vpool->elementPool)) ;
        newval->attribute       = CNMakeValueAttribute(vtype, size) ;
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
        struct CNValue * val = CNValueAllocate(CNIntValueType, sizeof(uint64_t), pool) ;
        val->floatValue = num ;
        return val ;
}

static inline void
initString(struct CNString * dst, struct CNValue * next, uint32_t len, const char * str)
{
        dst->next = next ;
        strncpy(dst->buffer, str, len) ;
}

struct CNValue *
CNAllocateString(const char * str, struct CNValuePool * pool)
{
        uint32_t len = (uint32_t) strlen(str) ;
        if(len > CNSTRING_ELEMENT_NUM) {
                struct CNValue * newval = CNValueAllocate(CNStringValueType, len,  pool) ;
                initString(&(newval->stringValue), NULL, CNSTRING_ELEMENT_NUM, str) ;

                const char * nstr = str + CNSTRING_ELEMENT_NUM ;
                struct CNValue * next = CNAllocateString(nstr, pool) ;
                (newval->stringValue).next = next ;

                return newval ;
        } else {
                struct CNValue * newval = CNValueAllocate(CNStringValueType, len, pool) ;
                initString(&(newval->stringValue), NULL, len, str) ;
                return newval ;
        }
}

void
CNValueFree(struct CNValuePool * pool, struct CNValue * dst)
{
        switch(CNTypeOfValue(dst)) {
                case CNVoidValueType:
                case CNCharValueType:
                case CNIntValueType:
                case CNFloatValueType: {
                } break ;
                case CNStringValueType: {
                        CNFreeString(pool, &(dst->stringValue)) ;
                } break ;
                case CNArrayValueType: {
                        uint32_t count = (dst->arrayValue).count ;
                        struct CNValue * values = (dst->arrayValue).values ;
                        CNFreeArray(&(pool->arrayPool), count, values) ;
                } break ;

        }
}

void
CNValueDump(unsigned int indent, const struct CNValue * src)
{
        switch(CNTypeOfValue(src)){
                case CNVoidValueType: {
                        CNDumpIndent(indent) ;
                        fputs("value: nil\n", stdout) ;
                } break ;
                case CNCharValueType: {
                        CNDumpIndent(indent) ;
                        printf("value: '%c'\n", (src->charValue)) ;
                } break ;
                case CNIntValueType: {
                        CNDumpIndent(indent) ;
                        printf("value: %lld\n", (src->int64Value)) ;
                } break ;
                case CNFloatValueType: {
                        CNDumpIndent(indent) ;
                        printf("value: %lf\n", (src->floatValue)) ;
                } break ;
                case CNStringValueType: {
                        CNDumpIndent(indent) ;
                        printf("value: \"") ;
                        CNDumpString(CNSizeOfValue(src), &(src->stringValue)) ;
                        fputs("\"\n", stdout) ;
                } break ;
                case CNArrayValueType: {
                        unsigned int num = CNSizeOfValue(src) ;
                        CNDumpIndent(indent) ;
                        printf("value: %u [\n", num) ;
                        struct CNValue * values = (src->arrayValue).values ;
                        for(unsigned int i=0 ; i<num ; i++) {
                                CNValueDump(indent+1, &(values[i])) ;
                        }
                        CNDumpIndent(indent) ;
                        printf("]\n") ;
                } break ;
        }
}

