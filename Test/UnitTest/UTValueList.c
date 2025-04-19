/*
 * @file UTValueList.c
 * @description The unit test for CNValueList
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTValueList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void
appendString(struct CNValueList * dst, const char * str, struct CNValuePool * vpool) ;

bool
UTValueList(void)
{
        bool result = true ;
        struct CNMemoryUsage usage ;
        
        printf("(%s) Initial state\n", __func__) ;
        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        struct CNValuePool vpool ;
        CNInitValuePool(&vpool, &lpool) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        printf("(%s) Allocate state\n", __func__) ;
        struct CNValueList vlist ;
        CNInitValueList(&vlist, &vpool) ;
        appendString(&vlist, "hello, world!!", &vpool) ;
        char atoz[128+1] ;
        for(int i=0 ; i<128 ; i++){
                atoz[i] = '0' + (i % 10) ;
        }
        atoz[128] = '\0' ;
        appendString(&vlist, atoz, &vpool) ;
        CNDumpValueList(0, &vlist) ;

        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        printf("(%s) Deinit state\n", __func__) ;
        CNDeinitValueList(&vlist) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        if(usage.allocatedSize == usage.usableSize) {
                printf("(%s) No memory leak\n", __func__) ;
        } else {
                printf("(%s) [Error] some memory leak\n", __func__) ;
                result = false ;
        }

        printf("(%s) Final state\n", __func__) ;
        CNDeinitValuePool(&vpool) ;
        CNDeinitListPool(&lpool) ;

        return result ;
}

static void
appendString(struct CNValueList * dst, const char * str, struct CNValuePool * vpool)
{
        struct CNValue * strval = CNAllocateString(str, (uint32_t) strlen(str), vpool) ;
        CNAppendToValueList(dst, strval) ;
        CNReleaseValue(vpool, strval) ;
}
