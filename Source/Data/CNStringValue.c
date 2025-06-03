/*
 * @file CNStringValue.c
 * @description Define some string value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNStringValue.h"
#include "CNValuePool.h"
#include "CNInterface.h"
#include <string.h>

static void releaseContents(struct CNValuePool * vpool, struct CNValue * val) ;
static void printValues(struct CNValue * val) ;

static size_t
lengthToSize(size_t length)
{
        size_t tmp = (length + CNValueSize - 1) / CNValueSize ;
        return tmp * CNValueSize ;
}

struct CNVirtualValueFunctions *
CNVirtualFunctionsForStringValue(void)
{
        static struct CNVirtualValueFunctions s_virt_func ;
        static bool s_initialized = false ;
        if(!s_initialized){
                s_virt_func.releaseContents     = &releaseContents ;
                s_virt_func.print               = &printValues ;
                s_initialized = true ;
        }
        return &s_virt_func ;
}

struct CNStringValue *
CNAllocateStringValue(struct CNValuePool * vpool, size_t length, const char * src)
{
        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForStringValue() ;
        struct CNStringValue * newval ;
        newval = (struct CNStringValue *) CNAllocateValue(vpool, CNStringType, vfunc) ;
        newval->length = length ;

        size_t srcsize = lengthToSize(length + 1) ;
        char * buffer = (char *) CNAllocateArrayData(&(vpool->arrayPool), srcsize) ;
        memcpy(buffer, src, length + 1) ;
        newval->string = buffer ;
        return newval ;
}

int
CNCompareStringValue(struct CNStringValue * s0, struct CNStringValue * s1)
{
        int64_t diff ;
        if((diff = s0->length - s1->length) != 0){
                return (int) diff ;
        } else {
                return strcmp(s0->string, s1->string) ;
        }
}

static void
releaseContents(struct CNValuePool * vpool, struct CNValue * val)
{
        struct CNStringValue * str = CNCastToStringValue(val) ;
        size_t srcsize = lengthToSize(str->length + 1) ;
        CNFreeArrayData(&(vpool->arrayPool), srcsize, str->string) ;
}

struct CNStringValue *
CNAddStringValue(struct CNValuePool * vpool, struct CNStringValue * s0, struct CNStringValue * s1)
{
        uint64_t len0   = CNLengthOfStringValue(s0) ;
        uint64_t len1   = CNLengthOfStringValue(s1) ;
        uint64_t length = len0 + len1 ;

        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForStringValue() ;
        struct CNStringValue * newval ;
        newval = (struct CNStringValue *) CNAllocateValue(vpool, CNStringType, vfunc) ;
        newval->length = length ;

        size_t srcsize = lengthToSize(length + 1) ;
        char * buffer = (char *) CNAllocateArrayData(&(vpool->arrayPool), srcsize) ;

        memcpy(buffer,        s0->string, len0) ;
        memcpy(buffer + len1, s1->string, len1) ;

        newval->string = buffer ;
        return newval ;
}

static void
printValues(struct CNValue * val)
{
        struct CNStringValue * str = CNCastToStringValue(val) ;
        CNInterface()->printf("\"") ;
        CNInterface()->printf(str->string) ;
        CNInterface()->printf("\"") ;
}

