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

        printf("chaValue:      %lu\n", sizeof(char)) ;
        printf("int64_t:       %lu\n", sizeof(int64_t)) ;
        printf("double:        %lu\n", sizeof(double)) ;
        printf("CNString:      %lu (%u)\n", sizeof(struct CNString), CNSTRING_ELEMENT_NUM) ;
        printf("sizeOfCNValue: %lu\n", sizeof(struct CNValue)) ;

        struct CNListPool       listpool ;
        CNInitListPool(&listpool) ;
        CNDumpListPool(0, &listpool) ;

        struct CNValuePool      valpool ;
        CNInitValuePool(&valpool, &listpool) ;
        CNDumpListPool(0, &listpool) ;

        char str[101] ;
        for(unsigned int i=0 ; i<100 ; i++) {
                str[i] = '0' + (i % 10) ;
        }
        str[100] = '\0' ;

        struct CNValue * val0 = CNAllocateInt64(1234, &valpool) ;
        struct CNValue * val1 = CNAllocateInt64(12.34, &valpool) ;
        struct CNValue * val2 = CNAllocateString("hello", &valpool) ;
        struct CNValue * val3 = CNAllocateString(str, &valpool) ;

        printf("val0 = ") ; CNValueDump(0, val0) ;
        printf("val1 = ") ; CNValueDump(0, val1) ;
        printf("val2 = ") ; CNValueDump(0, val2) ;
        printf("val3 = ") ; CNValueDump(0, val3) ;

        /*  */

        CNValueFree(&valpool, val0) ;
        CNValueFree(&valpool, val1) ;
        CNValueFree(&valpool, val2) ;

        CNDumpListPool(0, &listpool) ;
        CNFreeValuePool(&valpool) ;

        printf("UTValue: end\n") ;
        return result ;
}


