/*
 * @file UTValue.c
 * @description The unit test for CNValue
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTValue.h"
#include <stdio.h>
#include <stdlib.h>

bool UTValue(void)
{
        bool result = true ;

        printf("UTValue: end\n") ;

        printf("sizeOfCNValue: %lu\n", sizeof(struct CNValue)) ;

        struct CNListPool       listpool ;
        CNListPoolInit(&listpool) ;
        unsigned int orglistcnt = CNListCountOfFreeItems(&listpool);

        struct CNValuePool      valpool ;
        CNValuePoolInit(&valpool, &listpool) ;
        unsigned int orgvalcnt  = CNValueCountOfFreeItems(&valpool) ;

        /*  */

        struct CNValue * val0 = CNValueAllocate(&valpool) ;
        struct CNValue * val1 = CNValueAllocate(&valpool) ;

        val0->int64Value = 1234 ;
        val1->floatValue = 12.34 ;

        char valname[CNVALUE_DESCRIPTION_MAX_LEN] ;
        CNValueToDescription(valname, CNIntValueType, val0) ;
        printf("val0 = %s\n", valname) ;

        CNValueToDescription(valname, CNFloatValueType, val1) ;
        printf("val1 = %s\n", valname) ;

        /*  */

        CNValueFree(&valpool, val0) ;
        CNValueFree(&valpool, val1) ;

        unsigned int lastvalcnt = CNValueCountOfFreeItems(&valpool) ;
        CNValuePoolFree(&valpool) ;

        if(orgvalcnt != lastvalcnt) {
                printf("[Error] Unexpected free value count: %u <-> %u\n",
                       orgvalcnt, lastvalcnt) ;
                result = false ;
        }

        unsigned int lastlistcnt = CNListCountOfFreeItems(&listpool);
        CNListPoolFree(&listpool) ;

        if(orglistcnt != lastlistcnt) {
                printf("[Error] Unexpected free list count: %u <-> %u\n",
                       orglistcnt, lastlistcnt) ;
                result = false ;
        }

        printf("UTValue: end\n") ;
        return true ;
}


