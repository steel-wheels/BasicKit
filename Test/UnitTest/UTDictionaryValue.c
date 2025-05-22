/*
 * @file UTDictionaryValue.c
 * @description The unit test for CNDictionaryValue
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTArrayValue.h"
#include "UTUtils.h"
#include <stdio.h>
#include <string.h>

static void
setIntValue(struct CNDictionaryValue * dst, struct CNValuePool * vpool, uint64_t value) ;
static uint64_t
getIntValue(struct CNDictionaryValue * dst, struct CNValuePool * vpool, uint64_t value) ;

bool
UTDictionaryValue(struct CNValuePool * vpool)
{
        bool result = true ;

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

        CNInterface()->printf("(%s) set dictionary (2)\n", __func__) ;
        for(uint64_t i=0 ; i<1024 ; i++){
                setIntValue(dict0, vpool, i) ;
        }
        for(uint64_t i=0 ; i<1024 ; i++){
                if(getIntValue(dict0, vpool, i) != i){
                        CNInterface()->printf("(%s) [Error] Unexpected value\n", __func__) ;
                        result = false ;
                }
        }

        CNInterface()->printf("(%s) release dictionary\n", __func__) ;
        //CNPrintValueAttribute(CNSuperClassOfDictionaryValue(dict0)) ;
        CNReleaseValue(vpool, CNSuperClassOfDictionaryValue(dict0)) ;

        //CNDumpValuePool(0, vpool) ;

        return checkMemoryUsage(vpool) && result ;
}

static void
setIntValue(struct CNDictionaryValue * dst, struct CNValuePool * vpool, uint64_t src)
{
        char buffer[64] ;
        snprintf(buffer, 64, "%llu", src) ;
        struct CNStringValue * key = CNAllocateStringValue(vpool, (unsigned int) strlen(buffer), buffer) ;
        struct CNUnsignedIntValue * val = CNAllocateUnsignedIntValue(vpool, src) ;
        CNSetValueToDictionary(vpool, dst, key, CNSuperClassOfUnsignedIntValue(val)) ;
        CNReleaseValue(vpool, CNSuperClassOfStringValue(key)) ;
        CNReleaseValue(vpool, CNSuperClassOfUnsignedIntValue(val)) ;
}

static uint64_t
getIntValue(struct CNDictionaryValue * dst, struct CNValuePool * vpool, uint64_t keyval)
{
        char buffer[64] ;
        snprintf(buffer, 64, "%llu", keyval) ;
        struct CNStringValue * key = CNAllocateStringValue(vpool, (unsigned int) strlen(buffer), buffer) ;
        struct CNValue * ret = CNValueForKeyInDictionary(dst, key) ;
        CNReleaseValue(vpool, CNSuperClassOfStringValue(key)) ;

        if(ret != NULL){
                struct CNUnsignedIntValue * uval = CNCastToUnsignedIntValue(ret) ;
                return uval->value ;
        } else {
                return (uint64_t) -1 ;
        }
}
