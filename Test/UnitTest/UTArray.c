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

        printf("(%s) Initial state\n", __func__) ;
        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        struct CNValuePool vpool ;
        CNInitValuePool(&vpool, &lpool) ;
        CNDumpValuePool(0, &vpool) ;
        unsigned int lcount_init = CNCountOfFreeItemsInListPool(&lpool) ;
        unsigned int scalar_init = CNCountOfFreeScalarItemsInValuePool(&vpool) ;

        printf("(%s) Allocate state\n", __func__) ;
        //struct CNValue * val0 = CNAllocateArray(24, &vpool) ;
        //struct CNValue * val1 = CNAllocateArray(16, &vpool) ;

        //CNDumpValue(0, val0) ;
        //CNDumpValue(0, val1) ;

        printf("(%s) Release state\n", __func__) ;
        //CNReleaseValue(&vpool, val0) ;
        //CNReleaseValue(&vpool, val1) ;

        printf("(%s) Final state\n", __func__) ;
        unsigned int lcount_last = CNCountOfFreeItemsInListPool(&lpool) ;
        unsigned int scalar_last = CNCountOfFreeScalarItemsInValuePool(&vpool) ;
        CNDeinitValuePool(&vpool) ;
        CNDeinitListPool(&lpool) ;

        if(lcount_init != lcount_last) {
                printf("(%s) [Error] Invalid last list count %u <=> %u\n",
                       __func__, lcount_init, lcount_last) ;
                result = false ;
        }
        if(scalar_init != scalar_last) {
                printf("(%s) [Error] Invalid last scalar count %u <=> %u\n",
                       __func__, scalar_init, scalar_last) ;
                result = false ;
        }

        return result ;
}
