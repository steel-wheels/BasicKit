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
        CNDumpIndent(indent) ;
        printf("ValuePool\n") ;
        CNDumpScalarPool(indent + 1, &(src->scalarPool)) ;
        CNDumpArrayPool(indent + 1,  &(src->arrayPool)) ;
}

static struct CNValue *
CNValueAllocate(CNValueType vtype, size_t size, struct CNValuePool * vpool)
{
        struct CNValue * newval = CNAllocateScalar(&(vpool->scalarPool)) ;
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
initString(struct CNString * dst, struct CNValue * next, size_t len, const char * str)
{
        dst->next = next ;
        strncpy(dst->buffer, str, len) ;
}

struct CNValue *
CNAllocateString(const char * str, size_t len, struct CNValuePool * pool)
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

void
CNFreeValue(struct CNValuePool * pool, struct CNValue * dst)
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
CNDumpValue(unsigned int indent, const struct CNValue * src)
{
        switch(CNTypeOfValue(src)){
                case CNVoidValueType: {
                        CNDumpIndent(indent) ;
                        fputs("nil\n", stdout) ;
                } break ;
                case CNCharValueType: {
                        CNDumpIndent(indent) ;
                        printf("'%c'\n", (src->charValue)) ;
                } break ;
                case CNIntValueType: {
                        CNDumpIndent(indent) ;
                        printf("%lld\n", (src->int64Value)) ;
                } break ;
                case CNFloatValueType: {
                        CNDumpIndent(indent) ;
                        printf("%lf\n", (src->floatValue)) ;
                } break ;
                case CNStringValueType: {
                        CNDumpIndent(indent) ;
                        printf("\"") ;
                        CNDumpString(CNSizeOfValue(src), &(src->stringValue)) ;
                        fputs("\"\n", stdout) ;
                } break ;
                case CNArrayValueType: {
                        unsigned int num = CNSizeOfValue(src) ;
                        CNDumpIndent(indent) ;
                        printf("%u [\n", num) ;
                        struct CNValue * values = (src->arrayValue).values ;
                        for(unsigned int i=0 ; i<num ; i++) {
                                CNDumpValue(indent+1, &(values[i])) ;
                        }
                        CNDumpIndent(indent) ;
                        printf("]\n") ;
                } break ;
        }
}

