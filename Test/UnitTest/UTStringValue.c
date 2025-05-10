/*
 * @file UTStringValue.c
 * @description The unit test for CNStringValue
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNHeader.h>
#include "UTUtils.h"
#include <string.h>

bool UTStringValue(struct CNValuePool * vpool)
{
        CNInterface()->printf("(%s) String value test\n", __func__) ;

        const char * src0 = "Hello, world !!" ;
        struct CNStringValue * str0 = CNAllocateStringValue(vpool, (unsigned int) strlen(src0), src0) ;
        CNPrintValue(CNSuperClassOfStringValue(str0)) ;
        CNInterface()->printf("\n") ;

        const char * src1 = "Good morning !!" ;
        struct CNStringValue * str1 = CNAllocateStringValue(vpool, (unsigned int) strlen(src1), src1) ;
        CNPrintValue(CNSuperClassOfStringValue(str1)) ;
        CNInterface()->printf("\n") ;

        dumpMemoryUsage(vpool) ;

        CNInterface()->printf("(%s) Release state\n", __func__) ;
        CNReleaseValue(vpool, CNSuperClassOfStringValue(str0)) ;
        CNReleaseValue(vpool, CNSuperClassOfStringValue(str1)) ;

        return checkMemoryUsage(vpool) ;
}

