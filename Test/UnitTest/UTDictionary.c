/*
 * @file UTString.c
 * @description The unit test for CNString
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTDictionary.h"
#include <stdio.h>
#include <string.h>

static void
setDictionary(struct CNDictionary * dst, const char * key, double value, struct CNValuePool * vpool) ;

static struct CNValue *
searchValueInDictionary(struct CNDictionary * src, const char * key, struct CNValuePool * vpool) ;

bool
UTDictionary(void)
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
        struct CNValue * dict0 = CNAllocateDictionary(&vpool) ;
        setDictionary(&(dict0->dictionaryValue), "A", 12.3, &vpool) ;
        setDictionary(&(dict0->dictionaryValue), "B", 23.4, &vpool) ;
        setDictionary(&(dict0->dictionaryValue), "C", 34.5, &vpool) ;
        setDictionary(&(dict0->dictionaryValue), "D", 45.6, &vpool) ;
        setDictionary(&(dict0->dictionaryValue), "E", 56.7, &vpool) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        struct CNValue * rev0 = searchValueInDictionary(&(dict0->dictionaryValue), "A", &vpool) ;
        if(rev0 != NULL){
                printf("(%s) search result ... ", __func__) ;
                CNDumpValue(0, rev0) ;
        } else {
                printf("(%s) [Error] Failed to search\n", __func__) ;
                result = false ;
        }

        printf("(%s) Release state\n", __func__) ;
        CNReleaseValue(&vpool, dict0) ;

        printf("(%s) Final state\n", __func__) ;
        CNDeinitValuePool(&vpool) ;
        CNDeinitListPool(&lpool) ;

        return result ;
}

static void
setDictionary(struct CNDictionary * dst, const char * key, double data, struct CNValuePool * vpool)
{
        struct CNValue * keyval = CNAllocateString(key, (uint32_t) strlen(key), vpool) ;
        struct CNValue * value  = CNAllocateFloat(data, vpool) ;
        CNSetKeyAndValueToDictionary(dst, keyval, value, vpool) ;

}

static struct CNValue *
searchValueInDictionary(struct CNDictionary * src, const char * key, struct CNValuePool * vpool)
{
        struct CNValue * keyval = CNAllocateString(key, (uint32_t) strlen(key), vpool) ;
        struct CNValue * result = CNSearchValueInDictionary(src, keyval) ;
        CNReleaseValue(vpool, keyval) ;
        return result ;
}
