/*
 * @file UTString.c
 * @description The unit test for CNString
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTString.h"
#include <stdio.h>
#include <string.h>

bool UTString(void)
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
        const char * src0 = "Hello, world !!" ;
        uint32_t     len0 = (uint32_t) strlen(src0) ;
        struct CNValue * str0 = CNAllocateString(src0, len0, &vpool) ;
        if(CNLengthOfString(str0) == len0) {
                printf("(%s) len = %u ... OK\n", __func__, len0) ;
        } else {
                printf("(%s) len = %u ... Error\n", __func__, len0) ;
                result = false ;
        }
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        printf("(%s) Free state\n", __func__) ;
        CNReleaseValue(&vpool, str0) ;
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

