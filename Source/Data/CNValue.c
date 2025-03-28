/*
 * @file CNValue.c
 * @description Define CNValue data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNValue.h>
#import <BasicKit/CNValuePool.h>
#include <string.h>
#include <stdio.h>

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

static void
dumpIndent(unsigned int indent)
{
        unsigned int i ;
        for(i=0 ; i<indent ; i++){
                fputs("  ", stdout) ;
        }
}

void
CNValueDump(unsigned int indent, const struct CNValue * src)
{
        dumpIndent(indent) ;
        switch(CNTypeOfValue(src)){
                case CNVoidValueType: {
                        fputs("nil", stdout) ;
                } break ;
                case CNCharValueType: {
                        printf("'%c'", (src->charValue)) ;
                } break ;
                case CNIntValueType: {
                        printf("%lld", (src->int64Value)) ;
                } break ;
                case CNFloatValueType: {
                        printf("%lf", (src->floatValue)) ;
                } break ;
                case CNStringValueType: {
                        fputs("\"", stdout) ;
                        CNStringDump(CNSizeOfValue(src), &(src->stringValue)) ;
                        fputs("\"", stdout) ;
                } break ;
        }
        fputs("\n", stdout) ;
}

