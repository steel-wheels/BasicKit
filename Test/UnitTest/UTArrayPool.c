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

        struct CNValuePool vpool ;
        CNInitValuePool(&vpool, &lpool) ;

        printf("(%s) Initial state\n", __func__) ;
        CNDumpValuePool(0, &vpool) ;

        unsigned int elmnum = 256 ;
        struct CNValue * values ;
        values = CNAllocateArray(elmnum, &vpool) ;
        printf("(%s) Allocate state\n", __func__) ;
        CNDumpValuePool(0, &vpool) ;

        CNReleaseValue(&vpool, values) ;
        printf("(%s) Free state\n", __func__) ;
        CNDumpValuePool(0, &vpool) ;

        struct CNValue * rvalues ;
        rvalues = CNAllocateArray(elmnum, &vpool) ;
        printf("(%s) Re-allocate state\n", __func__) ;
        if(values == rvalues){
                printf("(%s) reused ... OK\n", __func__) ;
        } else {
                printf("(%s) notreused ... Error\n", __func__) ;
                result = false ;
        }
        CNDumpValuePool(0, &vpool) ;

        printf("(%s) Final state\n", __func__) ;
        CNFreeValuePool(&vpool) ;
        CNFreeListPool(&lpool) ;

        return result ;
}
