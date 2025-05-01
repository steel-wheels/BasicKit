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
        struct CNMemoryUsage usage ;

        printf("(%s) Initial prologue\n", __func__) ;
        printf("chaValue:      %lu\n", sizeof(char)) ;
        printf("int64_t:       %lu\n", sizeof(int64_t)) ;
        printf("double:        %lu\n", sizeof(double)) ;
        printf("CNString:      %lu (%u)\n", sizeof(struct CNString), CNSTRING_ELEMENT_NUM) ;
        printf("CNDictionary:  %lu (%u)\n", sizeof(struct CNDictionary), CNDICTIONARY_ELEMENT_NUM) ;
        printf("CNOpCode:      %lu\n", sizeof(struct CNOpCode)) ;
        printf("CNError:       %lu\n", sizeof(struct CNError)) ;
        printf("sizeOfCNValue: %lu\n", sizeof(struct CNValue)) ;

        printf("(%s) Initial state\n", __func__) ;
        struct CNListPool       listpool ;
        CNInitListPool(&listpool) ;
        struct CNValuePool      valpool ;
        CNInitValuePool(&valpool, &listpool) ;
        usage = CNMemoryUsageOfValuePool(&valpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        printf("(%s) Allocate state\n", __func__) ;
        struct CNValue * val0 = CNAllocateSignedInt(-1234, &valpool) ;
        struct CNValue * val1 = CNAllocateUnsignedInt(12345, &valpool) ;
        struct CNValue * val2 = CNAllocateFloat(12.34, &valpool) ;

        printf("val0 = ") ; CNDumpValue(0, val0) ;
        printf("val1 = ") ; CNDumpValue(0, val1) ;
        printf("val2 = ") ; CNDumpValue(0, val2) ;
        usage = CNMemoryUsageOfValuePool(&valpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        /* null value test */
        struct CNValue * nullval = CNAllocateNull() ;
        if(CNTypeOfValue(nullval) == CNNullType) {
                CNReleaseValue(&valpool, nullval) ;
        } else {
                printf("(%s) [Error] failed to decode null value\n", __func__) ;
                result = false ;
        }


        printf("(%s) Free state\n", __func__) ;
        CNReleaseValue(&valpool, val0) ;
        CNReleaseValue(&valpool, val1) ;
        CNReleaseValue(&valpool, val2) ;

        usage = CNMemoryUsageOfValuePool(&valpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        if(usage.allocatedSize == usage.usableSize) {
                printf("(%s) No memory leak\n", __func__) ;
        } else {
                printf("(%s) [Error] some memory leak\n", __func__) ;
                result = false ;
        }

        CNDumpListPool(0, &listpool) ;
        CNDeinitValuePool(&valpool) ;

        return result ;
}


