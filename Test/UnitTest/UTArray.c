/*
 * @file UTArray.c
 * @description The unit test for CNArray
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTArray.h"
#include <stdio.h>
#include <string.h>

bool
UTArray(void)
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
        struct CNValue * val0 = CNAllocateSignedInt(123, &vpool) ;
        struct CNValue * val1 = CNAllocateSignedInt(123, &vpool) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        printf("(%s) Release state\n", __func__) ;
        CNReleaseValue(&vpool, val0) ;
        CNReleaseValue(&vpool, val1) ;
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
