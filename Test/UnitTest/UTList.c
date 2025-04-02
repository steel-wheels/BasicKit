/*
 * @file UTList.c
 * @description The unit test for CNList
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTList.h"
#include <stdio.h>
#include <stdlib.h>

bool UTList(void)
{
        printf("UTList: start\n") ;
        bool result = true ;

        struct CNListPool       lpool ;
        CNInitListPool(&lpool) ;
        CNDumpListPool(0, &lpool) ;
        unsigned int fcount_1st = CNCountOfFreeItemsInListPool(&lpool) ;

#       define ITEM_NUM         10
        struct CNList * items[ITEM_NUM] ;
        for(unsigned int i=0 ; i<ITEM_NUM ; i++){
                items[i] = CNAllocateList(&lpool) ;
        }
        CNDumpListPool(0, &lpool) ;
        unsigned int fcount_mid = CNCountOfFreeItemsInListPool(&lpool) ;

        for(unsigned int i=0 ; i<ITEM_NUM ; i++){
                CNFreeList(&lpool, items[i]) ;
        }
        CNDumpListPool(0, &lpool) ;
        unsigned int fcount_last = CNCountOfFreeItemsInListPool(&lpool) ;

        CNFreeListPool(&lpool) ;

        if(fcount_1st != fcount_last){
                printf("(%s) [Error] Invalid last count\n", __func__) ;
                result = false ;
        }
        if(fcount_1st != fcount_mid + ITEM_NUM) {
                printf("(%s) [Error] Invalid middle count\n", __func__) ;
                result = false ;
        }
        printf("UTList: end\n") ;
        return result ;
}
