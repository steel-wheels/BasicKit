/*
 * @file UTIterator.c
 * @description The unit test for iterators
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTIterator.h"
#include <stdio.h>
#include <string.h>

static bool
UTStringIterator(void) ;
static bool
UTStringListIterator(void) ;
static void
appendString(struct CNValueList * dst, struct CNValuePool * vpool, const char * str) ;

bool UTIterator(void)
{
        bool result = true ;
        result &= UTStringIterator() ;
        result &= UTStringListIterator() ;
        return result ;
}

static bool
UTStringIteratorTest(struct CNValuePool * vpool, const char * str) ;

static bool
UTStringIterator(void)
{
        bool result = true ;
        struct CNMemoryUsage usage ;

        printf("(%s) Initial state\n", __func__) ;

        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        struct CNValuePool vpool ;

        CNInitValuePool(&vpool, &lpool) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) Test state\n", __func__) ;

        /* test1 */
        result &= UTStringIteratorTest(&vpool, "Hello, world !!") ;

        /* test2 */
        char src1[256 + 1] ;
        unsigned int i ;
        for(i=0 ; i<256 ; i++){
                src1[i] = '0' + (i % 10) ;
        }
        src1[i] = '\0' ;
        result &= UTStringIteratorTest(&vpool, src1) ;

        printf("(%s) Release state\n", __func__) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

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
UTStringIteratorTest(struct CNValuePool * vpool, const char * str)
{
        uint32_t len = (uint32_t) strlen(str) ;

        printf("(%s) length = %u\n", __func__, len) ;
        struct CNValue * val = CNAllocateString(str, len, vpool) ;
        struct CNStringIterator iterator ;
        CNInitStringIterator(&iterator, val) ;

#       define REV_STR_LEN      1024
        char revstr[REV_STR_LEN + 1] ;

        unsigned int i ;
        for(i=0 ; i<REV_STR_LEN ; i++){
                char c = CNGetCharacterFromStringIterator(&iterator) ;
                if(c != EOF){
                        revstr[i] = c ;
                } else {
                        break ;
                }
        }
        revstr[i] = '\0' ;

        CNDeinitStringIterator(vpool, &iterator) ;

        CNReleaseValue(vpool, val) ;

        bool result ;
        if(strcmp(str, revstr) == 0){
                printf("(%s) Compare ... OK\n", __func__) ;
                result = true ;
        } else {
                printf("(%s) Compare ... NG \"%s\" <=> \"%s\"\n",
                       __func__, str, revstr) ;
                result = false ;
        }
        return result ;
}

static bool
UTStringListIterator(void)
{
        bool result = true ;
        struct CNMemoryUsage usage ;

        printf("(%s) Initial state\n", __func__) ;

        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        struct CNValuePool vpool ;

        CNInitValuePool(&vpool, &lpool) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) Allocate state\n", __func__) ;
        struct CNValueList vlist ;
        CNInitValueList(&vlist, &vpool) ;
        appendString(&vlist, &vpool, "Hello, World !!") ;
        char longstr[128 + 1] ;
        for(unsigned int i=0 ; i<128 ; i++){
                longstr[i] = '0' + (i % 10) ;
        }
        longstr[128] = '\0' ;
        appendString(&vlist, &vpool, longstr) ;
        CNPrintValueList(&vlist) ;
        printf("\n") ;

        struct CNStringListIterator iter ;
        CNInitStringListIterator(&iter, &vpool, &vlist) ;

        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) Operation state\n", __func__) ;
        char resstr[256 + 1] ;
        unsigned int i ;
        char c ;
        for(i=0 ; (c = CNGetCharacterFromStringListIterator(&iter)) != EOF && i<256 ; i++){
                resstr[i] = c ;
        }
        resstr[i] = '\0' ;
        printf("(%s) operation result = %u:%s\n", __func__, i, resstr) ;

        printf("(%s) Release state\n", __func__) ;
        CNDeinitStringListIterator(&iter) ;
        CNDeinitValueList(&vlist) ;

        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

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

static void
appendString(struct CNValueList * dst, struct CNValuePool * vpool, const char * str)
{
        struct CNValue * newval = CNAllocateString(str, (uint32_t) strlen(str), vpool) ;
        CNAppendToValueList(dst, newval) ;
        CNReleaseValue(vpool, newval) ;
}

