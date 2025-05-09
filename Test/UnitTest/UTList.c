/*
 * @file UTList.c
 * @description The unit test for CNList
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTList.h"
#include <stdio.h>
#include <stdlib.h>

bool UTList(struct CNListPool * lpool)
{
        printf("(%s) start\n", __func__) ;
        bool result = true ;

        struct CNMemoryUsage usage = CNMemoryUsageOfListPool(lpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) allocate state\n", __func__) ;
#       define ITEM_NUM         10
        struct CNList * items[ITEM_NUM] ;
        for(unsigned int i=0 ; i<ITEM_NUM ; i++){
                items[i] = CNAllocateList(lpool) ;
        }
        usage = CNMemoryUsageOfListPool(lpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) release state\n", __func__) ;
        for(unsigned int i=0 ; i<ITEM_NUM ; i++){
                CNFreeList(lpool, items[i]) ;
        }

        printf("(%s) check memory management\n", __func__) ;
        usage = CNMemoryUsageOfListPool(lpool) ;
        CNPrintMemoryUsage(&usage) ;
        if(usage.allocatedSize == usage.usableSize) {
                printf("(%s) No memory leak\n", __func__) ;
        } else {
                printf("(%s) [Error] some memory leak\n", __func__) ;
                result = false ;
        }

        printf("(%s) end\n", __func__) ;
        return result ;
}
