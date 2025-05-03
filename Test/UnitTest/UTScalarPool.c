/*
 * @file UTScalarPool.c
 * @description The unit test for CNScalarPool
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTScalarPool.h"
#include <stdio.h>

bool UTScalarPool(void)
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
#       define VALUE_NUM        64
        struct CNValue * values[VALUE_NUM] ;
        for(unsigned int i=0 ; i<VALUE_NUM ; i++){
                values[i] = CNAllocateUnsignedInt(i, &vpool) ;
        }
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) Release state\n", __func__) ;
        for(unsigned int i=0 ; i<VALUE_NUM ; i++){
                CNReleaseValue(&vpool, values[i]) ;
        }
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        if(usage.allocatedSize == usage.usableSize) {
                printf("(%s) No memory leak\n", __func__) ;
        } else {
                printf("(%s) [Error] some memory leak\n", __func__) ;
                result = false ;
        }

        CNDeinitValuePool(&vpool) ;
        CNDeinitListPool(&lpool) ;

        return result ;
}

