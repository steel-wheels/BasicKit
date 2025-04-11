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


        printf("(%s) Initial prologue\n", __func__) ;
        printf("chaValue:      %lu\n", sizeof(char)) ;
        printf("int64_t:       %lu\n", sizeof(int64_t)) ;
        printf("double:        %lu\n", sizeof(double)) ;
        printf("CNString:      %lu (%u)\n", sizeof(struct CNString), CNSTRING_ELEMENT_NUM) ;
        printf("CNDictionary   %lu (%u)\n", sizeof(struct CNDictionary), CNDICTIONARY_ELEMENT_NUM) ;
        printf("sizeOfCNValue: %lu\n", sizeof(struct CNValue)) ;

        printf("(%s) Initial state\n", __func__) ;
        struct CNListPool       listpool ;
        CNInitListPool(&listpool) ;
        struct CNValuePool      valpool ;
        CNInitValuePool(&valpool, &listpool) ;
        CNDumpValuePool(0, &valpool) ;
        unsigned int scalar_init = CNCountOfFreeScalarItemsInValuePool(&valpool) ;

        printf("(%s) Allocate state\n", __func__) ;
        struct CNValue * val0 = CNAllocateSignedInt(-1234, &valpool) ;
        struct CNValue * val1 = CNAllocateUnsignedInt(12345, &valpool) ;
        struct CNValue * val2 = CNAllocateFloat(12.34, &valpool) ;

        printf("val0 = ") ; CNDumpValue(0, val0) ;
        printf("val1 = ") ; CNDumpValue(0, val1) ;
        printf("val2 = ") ; CNDumpValue(0, val2) ;
        CNDumpValuePool(0, &valpool) ;
        unsigned int scalar_mid = CNCountOfFreeScalarItemsInValuePool(&valpool) ;

        printf("(%s) Free state\n", __func__) ;
        CNReleaseValue(&valpool, val0) ;
        CNReleaseValue(&valpool, val1) ;
        CNReleaseValue(&valpool, val2) ;

        unsigned int scalar_last = CNCountOfFreeScalarItemsInValuePool(&valpool) ;
        CNDumpListPool(0, &listpool) ;
        CNDeinitValuePool(&valpool) ;

        if(scalar_init != scalar_last) {
                printf("(%s) [Error] Invalid last list count %u <=> %u\n",
                       __func__, scalar_init, scalar_last) ;
                result = false ;
        }
        if(scalar_init != scalar_mid + 3) {
                printf("(%s) [Error] Invalid mid list count %u <=> %u\n",
                       __func__, scalar_init, scalar_mid) ;
                result = false ;
        }

        return result ;
}


