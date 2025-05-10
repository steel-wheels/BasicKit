/*
 * @file UTArrayValue.c
 * @description The unit test for CNArrayValue
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTArrayValue.h"
#include "UTUtils.h"

bool
UTArrayValue(struct CNValuePool * vpool)
{
        bool result = true ;
        
        CNInterface()->printf("(%s) Array value test\n", __func__) ;

        struct CNArrayValue * arr0 = CNAllocateArrayValue(vpool, 16) ;
        dumpMemoryUsage(vpool) ;

        CNInterface()->printf("(%s) Set elements\n", __func__) ; {
                struct CNSignedIntValue * var0 = CNAllocateSignedIntValue(vpool, +1) ;
                CNInterface()->printf("(%s) Set var to index 0: ", __func__) ;
                CNPrintValue(CNSuperClassOfSignedIntValue(var0)) ;
                CNInterface()->printf("\n") ;
                result &= CNSetValueToArray(vpool, arr0, 0, CNSuperClassOfSignedIntValue(var0)) ;
                //CNPrintValueAttribute(CNSuperClassOfSignedIntValue(var0)) ;
                CNReleaseValue(vpool, CNSuperClassOfSignedIntValue(var0)) ;
        }
        {
                struct CNSignedIntValue * var1 = CNAllocateSignedIntValue(vpool, -1) ;
                CNInterface()->printf("(%s) Set var to index 1: ", __func__) ;
                CNPrintValue(CNSuperClassOfSignedIntValue(var1)) ;
                CNInterface()->printf("\n") ;
                result &= CNSetValueToArray(vpool, arr0, 1, CNSuperClassOfSignedIntValue(var1)) ;
                //CNPrintValueAttribute(CNSuperClassOfSignedIntValue(var1)) ;
                CNReleaseValue(vpool, CNSuperClassOfSignedIntValue(var1)) ;
        }

        CNInterface()->printf("(%s) Dump array\n", __func__) ;
        CNPrintValue(CNSuperClassOfArrayValue(arr0)) ;
        CNInterface()->printf("\n") ;

        CNInterface()->printf("(%s) Released state\n", __func__) ;
        CNReleaseValue(vpool, CNSuperClassOfArrayValue(arr0)) ;
        return checkMemoryUsage(vpool) ;
}
