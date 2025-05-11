/*
 * @file UTDictionaryValue.c
 * @description The unit test for CNDictionaryValue
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTArrayValue.h"
#include "UTUtils.h"

bool
UTDictionaryValue(struct CNValuePool * vpool)
{
        CNInterface()->printf("(%s) Dictionary value test\n", __func__) ;

        CNInterface()->printf("(%s) allocate dictionary\n", __func__) ;
        struct CNDictionaryValue * dict0 = CNAllocateDictionaryValue(vpool) ;
        CNPrintValue(CNSuperClassOfDictionaryValue(dict0)) ;
        CNInterface()->printf("\n") ;
        dumpMemoryUsage(vpool) ;

        CNInterface()->printf("(%s) set dictionary\n", __func__) ;
        struct CNStringValue * key0 = CNAllocateStringValue(vpool, 4, "key0") ;
        struct CNUnsignedIntValue * val0 = CNAllocateUnsignedIntValue(vpool, 1) ;
        CNSetValueToDictionary(vpool, dict0, key0, CNSuperClassOfUnsignedIntValue(val0)) ;
        CNReleaseValue(vpool, CNSuperClassOfStringValue(key0)) ;
        CNReleaseValue(vpool, CNSuperClassOfUnsignedIntValue(val0)) ;
        CNPrintValue(CNSuperClassOfDictionaryValue(dict0)) ;
        CNInterface()->printf("\n") ;
        dumpMemoryUsage(vpool) ;
        
        CNInterface()->printf("(%s) release dictionary\n", __func__) ;
        //CNPrintValueAttribute(CNSuperClassOfDictionaryValue(dict0)) ;
        CNReleaseValue(vpool, CNSuperClassOfDictionaryValue(dict0)) ;

        //CNDumpValuePool(0, vpool) ;

        return checkMemoryUsage(vpool) ; ;
}

