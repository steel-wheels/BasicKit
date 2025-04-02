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
        unsigned int freescalar_init = CNCountOfFreeScalarItemsInValuePool(&vpool) ;
        unsigned int freearray_init  = CNCountOfFreeArrayItemsInValuePool(&vpool) ;
        CNDumpValuePool(0, &vpool) ;

        printf("(%s) Allocate state\n", __func__) ;
        unsigned int elmnum = 256 ;
        struct CNValue * values ;
        values = CNAllocateArray(elmnum, &vpool) ;
        CNDumpValuePool(0, &vpool) ;

        printf("(%s) Release state\n", __func__) ;
        CNReleaseValue(&vpool, values) ;
        CNDumpValuePool(0, &vpool) ;

        printf("(%s) Allocate state (2nd)\n", __func__) ;
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

        printf("(%s) Release state (2nd)\n", __func__) ;
        CNReleaseValue(&vpool, rvalues) ;
        CNDumpValuePool(0, &vpool) ;

        printf("(%s) Final state\n", __func__) ;
        unsigned int freescalar_last = CNCountOfFreeScalarItemsInValuePool(&vpool) ;
        unsigned int freearray_last  = CNCountOfFreeArrayItemsInValuePool(&vpool) ;
        CNFreeValuePool(&vpool) ;
        CNFreeListPool(&lpool) ;

        if(freescalar_init != freescalar_last){
                printf("(%s) [Error] Invalid free scalar count %u <=> %u\n",
                       __func__, freescalar_init, freescalar_last) ;
                result = false ;
        }
        if(freearray_init != 0){
                printf("(%s) [Error] Invalid init free array count: %u\n",
                       __func__, freearray_init) ;
                result = false ;
        }
        if(freearray_last != 1) {
                printf("(%s) [Error] Invalid last free array count: %u\n",
                       __func__, freearray_last) ;
                result = false ;
        }

        return result ;
}
