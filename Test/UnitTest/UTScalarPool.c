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

        printf("(%s) Initial state\n", __func__) ;
        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        unsigned int lcount_init = CNCountOfFreeItemsInListPool(&lpool) ;

        struct CNScalarPool spool ;
        CNInitScalarPool(&spool, sizeof(struct CNValue), 1024, &lpool) ;
        CNDumpScalarPool(0, &spool) ;
        unsigned int freescaler_init = CNCountOfFreeItemsInScalarPool(&spool) ;

        printf("(%s) Allocate state\n", __func__) ;
#       define VALUE_NUM        64
        struct CNValue * values[VALUE_NUM] ;
        for(unsigned int i=0 ; i<VALUE_NUM ; i++){
                values[i] = CNAllocateScalar(&spool) ;
        }
        CNDumpScalarPool(0, &spool) ;

        printf("(%s) Free state\n", __func__) ;
        for(unsigned int i=0 ; i<VALUE_NUM ; i++){
                CNFreeScalar(&spool, values[i]) ;
        }
        CNDumpScalarPool(0, &spool) ;

        printf("(%s) Final state\n", __func__) ;
        unsigned int freescaler_last = CNCountOfFreeItemsInScalarPool(&spool) ;
        CNDeinitScalarPool(&spool) ;
        unsigned int lcount_last = CNCountOfFreeItemsInListPool(&lpool) ;
        CNDeinitListPool(&lpool) ;

        if(lcount_init != lcount_last) {
                printf("(%s) [Error] Invalid last count %u <=> %u\n",
                       __func__, lcount_init, lcount_last) ;
                result = false ;
        }
        if(freescaler_init != freescaler_last) {
                printf("(%s) [Error] Invalid last count %u <=> %u\n",
                       __func__, freescaler_init, freescaler_last) ;
                result = false ;
        }

        return result ;
}

