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
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) Allocate state\n", __func__) ;
        struct CNValue * val0 = CNAllocateSignedInt(123, &vpool) ;
        struct CNValue * val1 = CNAllocateSignedInt(456, &vpool) ;
        struct CNValue * arr0 = CNAllocateArray(16, &vpool) ;

        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) Getter/Setter state\n", __func__) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        CNSetElementToArray(&vpool, &(arr0->arrayValue), 0, val0) ;
        CNSetElementToArray(&vpool, &(arr0->arrayValue), 1, val1) ;

        struct CNValue * rev0 = CNElementInArray(&(arr0->arrayValue), 0) ;
        struct CNValue * rev1 = CNElementInArray(&(arr0->arrayValue), 1) ;

        printf("(%s) get %u -> ", __func__, 0) ;
        CNPrintValue(rev0) ;
        printf("\n") ;

        printf("(%s) get %u -> ", __func__, 1) ;
        CNPrintValue(rev1) ;
        printf("\n") ;

        printf("(%s) Release state\n", __func__) ;
        CNReleaseValue(&vpool, val0) ;
        CNReleaseValue(&vpool, val1) ;
        CNReleaseValue(&vpool, arr0) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

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
