/*
 * @file UTValue.c
 * @description The unit test for CNValue
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTValue.h"
#include "UTUtils.h"

static bool
testIntValues(struct CNValuePool * vpool) ;

bool
UTValue(struct CNValuePool * vpool)
{
        return testIntValues(vpool) ;
}

#define TEST_VALUE_NUM          16
static bool
testIntValues(struct CNValuePool * vpool)
{
        CNInterface()->printf("(%s) Int value test\n", __func__) ;
        struct CNSignedIntValue *values[TEST_VALUE_NUM] ;
        CNInterface()->printf("(%s) allocate values\n", __func__) ;
        for(unsigned int i=0 ; i<TEST_VALUE_NUM ; i++){
                values[i] = CNAllocateSignedIntValue(vpool, i) ;
        }
        for(unsigned int i=0 ; i<TEST_VALUE_NUM ; i++){
                CNPrintValue(&(values[i]->superClass)) ;
                CNInterface()->printf(", ") ;
        }
        CNInterface()->printf("\n") ;
        dumpMemoryUsage(vpool) ;

        CNInterface()->printf("(%s) release values\n", __func__) ;
        for(unsigned int i=0 ; i<TEST_VALUE_NUM ; i++){
                CNReleaseValue(vpool, &(values[i]->superClass)) ;
        }
        return checkMemoryUsage(vpool) ;
}
