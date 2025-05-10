/*
 * @file UTValue.c
 * @description The unit test for CNValue
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTValue.h"
#include "UTUtils.h"

static bool
testNullValues(struct CNValuePool * vpool) ;
static bool
testIntValues(struct CNValuePool * vpool) ;

bool
UTValue(struct CNValuePool * vpool)
{
        bool res0 = testNullValues(vpool) ;
        bool res1 = testIntValues(vpool) ;
        return res0 && res1 ;
}

static bool
testNullValues(struct CNValuePool * vpool)
{
        CNInterface()->printf("(%s) Null value test\n", __func__) ;
        struct CNNullValue * nval = CNAllocateNullValue() ;
        CNRetainValue(CNSuperClassOfNullValue(nval)) ;
        CNPrintValueAttribute(CNSuperClassOfNullValue(nval)) ;
        CNReleaseValue(vpool, CNSuperClassOfNullValue(nval)) ;
        CNRetainValue(CNSuperClassOfNullValue(nval)) ;
        return checkMemoryUsage(vpool) ;
}

#define TEST_VALUE_NUM          16
static bool
testIntValues(struct CNValuePool * vpool)
{
        CNInterface()->printf("(%s) Int value test\n", __func__) ;
        struct CNValue *values[TEST_VALUE_NUM] ;
        CNInterface()->printf("(%s) allocate values\n", __func__) ;
        for(unsigned int i=0 ; i<TEST_VALUE_NUM ; i++){
                switch(i % 3){
                        case 0: {
                                values[i] = (struct CNValue *) CNAllocateSignedIntValue(vpool, i) ;
                        } break ;
                        case 1: {
                                values[i] = (struct CNValue *) CNAllocateUnsignedIntValue(vpool, i) ;
                        } break ;
                        default: {
                                values[i] = (struct CNValue *) CNAllocateFloatValue(vpool, i) ;
                        } break ;
                }

        }
        for(unsigned int i=0 ; i<TEST_VALUE_NUM ; i++){
                CNPrintValue(values[i]) ;
                CNInterface()->printf(", ") ;
        }
        CNInterface()->printf("\n") ;
        dumpMemoryUsage(vpool) ;

        CNInterface()->printf("(%s) release values\n", __func__) ;
        for(unsigned int i=0 ; i<TEST_VALUE_NUM ; i++){
                CNReleaseValue(vpool, values[i]) ;
        }
        return checkMemoryUsage(vpool) ;
}
