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
        struct CNMemoryUsage usage ;

        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;

        struct CNValuePool vpool ;
        CNInitValuePool(&vpool, &lpool) ;

        printf("(%s) Initial state\n", __func__) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) Allocate state\n", __func__) ;
        unsigned int elmnum = 256 ;
        struct CNValue * values ;
        values = CNAllocateArray(elmnum, &vpool) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) Release state\n", __func__) ;
        CNReleaseValue(&vpool, values) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        if(usage.allocatedSize == usage.usableSize) {
                printf("(%s) No memory leak\n", __func__) ;
        } else {
                printf("(%s) [Error] some memory leak\n", __func__) ;
                result = false ;
        }

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

        printf("(%s) Release state (2nd)\n", __func__) ;
        CNReleaseValue(&vpool, rvalues) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;

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
