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
        printf("UTList: start") ;

        struct CNListPool       listpool ;
        CNListPoolInit(&listpool) ;

        unsigned int fcount = CNListCountOfFreeItems(&listpool) ;
        printf("initial free items: %u\n", fcount) ;

        struct CNList * item = CNListAllocate(&listpool) ;
        fcount = CNListCountOfFreeItems(&listpool) ;
        printf("initial next free items: %u\n", fcount) ;

        CNListFree(&listpool, item) ;
        fcount = CNListCountOfFreeItems(&listpool) ;
        printf("initial last free items: %u\n", fcount) ;

        CNListPoolFree(&listpool) ;

        printf("UTList: end") ;
        return true ;
}
