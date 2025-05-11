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

static unsigned int
pageElementNumInPage(unsigned int length)
{
        static const unsigned int UNIT_SIZE = sizeof(char) * 8 ;
        return (length + UNIT_SIZE - 1) / UNIT_SIZE ;
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
CNAllocateStringValue(struct CNValuePool * vpool, unsigned int length, const char * src)
{
        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForStringValue() ;
        struct CNStringValue * newval ;
        newval = (struct CNStringValue *) CNAllocateValue(vpool, CNStringType, vfunc) ;
        newval->length = length ;

        unsigned int elmnum = pageElementNumInPage(length + 1) ;
        char * buffer = (char *) CNAllocateArrayData(&(vpool->arrayPool), sizeof(char) * elmnum) ;
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
        unsigned int elmnum = pageElementNumInPage((unsigned int) (str->length + 1)) ;
        CNFreeArrayData(&(vpool->arrayPool), sizeof(char) * elmnum, str->string) ;
}

static void
printValues(struct CNValue * val)
{
        struct CNStringValue * str = CNCastToStringValue(val) ;
        CNInterface()->printf("\"") ;
        CNInterface()->printf(str->string) ;
        CNInterface()->printf("\"") ;
}

