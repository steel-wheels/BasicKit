//
//  UTArrayPool.c
//  UnitTest
//
//  Created by Tomoo Hamada on 2025/03/30.
//

#include "UTArrayPool.h"
#include <stdio.h>

bool UTArrayPool(void)
{
        bool result = true ;

        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;

        struct CNArrayPool apool ;
        CNInitArrayPool(&apool, sizeof(struct CNValue), &lpool) ;
        printf("(%s) Initial state\n", __func__) ;
        CNDumpArrayPool(0, &apool) ;

        unsigned int elmnum = 256 ;
        struct CNValue * values ;
        values = CNAllocateArray(&apool, elmnum) ;
        printf("(%s) Allocate state\n", __func__) ;
        CNDumpArrayPool(0, &apool) ;

        CNFreeArray(&apool, elmnum, values) ;
        printf("(%s) Free state\n", __func__) ;
        CNDumpArrayPool(0, &apool) ;

        struct CNValue * rvalues ;
        rvalues = CNAllocateArray(&apool, elmnum) ;
        printf("(%s) Re-allocate state\n", __func__) ;
        if(values == rvalues){
                printf("(%s) reused ... OK\n", __func__) ;
        } else {
                printf("(%s) notreused ... Error\n", __func__) ;
                result = false ;
        }
        CNDumpArrayPool(0, &apool) ;

        printf("(%s) Final state\n", __func__) ;
        CNFreeArrayPool(&apool) ;

        return result ;
}
