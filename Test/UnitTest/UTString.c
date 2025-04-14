/*
 * @file UTString.c
 * @description The unit test for CNString
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTString.h"
#include <stdio.h>
#include <string.h>

static bool
allocateString(struct CNValue ** newstr, struct CNValuePool * vpool, const char * str) ;

bool UTString(void)
{
        bool result = true ;
        struct CNMemoryUsage usage ;

        printf("(%s) Initial state\n", __func__) ;
        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        struct CNValuePool vpool ;
        CNInitValuePool(&vpool, &lpool) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        printf("(%s) Allocate state\n", __func__) ;
        struct CNValue * str0 ;
        result &= allocateString(&str0, &vpool, "Hello, world !!") ;

        char buf1[256 + 1] ;
        for(unsigned int i=0 ; i<256 ; i++){
                buf1[i] = '0' + (i / 10) ;
        }
        buf1[256] = '\0' ;
        struct CNValue * str1 ;
        result &= allocateString(&str1, &vpool, buf1) ;

        printf("(%s) Free state\n", __func__) ;
        CNReleaseValue(&vpool, str0) ;
        CNReleaseValue(&vpool, str1) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

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

static bool
allocateString(struct CNValue ** newstr, struct CNValuePool * vpool, const char * src)
{
        bool result  = true ;
        uint32_t len = (uint32_t) strlen(src) ;
        struct CNValue * str = CNAllocateString(src, len, vpool) ;

        /* count element num */
        unsigned int elmnum = 0 ;
        struct CNValue * next = str ;
        while(next != NULL){
                printf("(%s) %u elm len=%u\n", __func__, elmnum, CNLengthOfString(next)) ;
                elmnum += 1 ;
                next = (next->stringValue).next ;
        }
        unsigned int expnum = (len + CNSTRING_ELEMENT_NUM - 1) / CNSTRING_ELEMENT_NUM ;
        if(elmnum == expnum){
                printf("(%s) element-num = %u ... OK\n", __func__, elmnum) ;
        } else {
                printf("(%s) element-num = %u <-> %u ... NG\n", __func__, elmnum, expnum) ;
                result = false ;
        }

        if(CNLengthOfString(str) == len) {
                printf("(%s) len = %u ... OK\n", __func__, len) ;
        } else {
                printf("(%s) len = %u ... Error\n", __func__, len) ;
                result = false ;
        }

        struct CNMemoryUsage usage = CNMemoryUsageOfValuePool(vpool) ;
        CNDumpMemoryUsage(0, &usage) ;
        *newstr = str ;
        return result ;
}
