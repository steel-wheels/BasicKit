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

        printf("(%s) Initial state\n", __func__) ;
        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        struct CNValuePool vpool ;
        CNInitValuePool(&vpool, &lpool) ;
        CNDumpValuePool(0, &vpool) ;
        unsigned int lcount_init = CNCountOfFreeItemsInListPool(&lpool) ;
        unsigned int scalar_init = CNCountOfFreeScalarItemsInValuePool(&vpool) ;

        printf("(%s) Allocate state\n", __func__) ;
        const char * src0 = "Hello, world !!" ;
        uint32_t     len0 = (uint32_t) strlen(src0) ;
        struct CNValue * str0 = CNAllocateString(src0, len0, &vpool) ;
        CNDumpValue(0, str0) ;
        if(CNLengthOfString(str0) == len0) {
                printf("(%s) len = %u ... OK\n", __func__, len0) ;
        } else {
                printf("(%s) len = %u ... Error\n", __func__, len0) ;
                result = false ;
        }
        CNDumpValuePool(0, &vpool) ;
        unsigned int scalar_mid = CNCountOfFreeScalarItemsInValuePool(&vpool) ;

        printf("(%s) Free state\n", __func__) ;
        CNReleaseValue(&vpool, str0) ;
        CNDumpValuePool(0, &vpool) ;
        
        printf("(%s) Final state\n", __func__) ;
        unsigned int lcount_last = CNCountOfFreeItemsInListPool(&lpool) ;
        unsigned int scalar_last = CNCountOfFreeScalarItemsInValuePool(&vpool) ;
        CNFreeValuePool(&vpool) ;
        CNFreeListPool(&lpool) ;

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
        if(scalar_mid != scalar_init - 1) {
                printf("(%s) [Error] Invalid mid scalar count %u <=> %u\n",
                       __func__, scalar_init, scalar_last) ;
                result = false ;
        }

        return result ;
}

