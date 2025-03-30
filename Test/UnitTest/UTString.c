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

        printf("(%s) Allocate state\n", __func__) ;
        const char * src0 = "Hello, world !!" ;
        size_t       len0 = strlen(src0) ;
        struct CNValue * str0 = CNAllocateString(src0, len0, &vpool) ;
        CNDumpValue(0, str0) ;
        if(CNLengthOfString(str0) == len0) {
                printf("(%s) len = %lu ... OK\n", __func__, len0) ;
        } else {
                printf("(%s) len = %lu ... Error\n", __func__, len0) ;
                result = false ;
        }
        CNDumpValuePool(0, &vpool) ;

        printf("(%s) Free state\n", __func__) ;
        CNFreeValue(&vpool, str0) ;
        CNDumpValuePool(0, &vpool) ;
        
        printf("(%s) Final state\n", __func__) ;
        CNFreeValuePool(&vpool) ;
        CNFreeListPool(&lpool) ;

        return result ;
}

