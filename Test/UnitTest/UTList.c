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

        struct CNListPool       listpool ;
        CNInitListPool(&listpool) ;
        CNDumpListPool(0, &listpool) ;

        struct CNList * item = CNAllocateList(&listpool) ;
        CNDumpListPool(0, &listpool) ;

        CNFreeList(&listpool, item) ;
        CNDumpListPool(0, &listpool) ;

        CNFreeListPool(&listpool) ;

        printf("UTList: end\n") ;
        return true ;
}
