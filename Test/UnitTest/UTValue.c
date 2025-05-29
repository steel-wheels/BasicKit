/*
 * @file UTValue.c
 * @description The unit test for CNValue
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTValue.h"
#include "UTUtils.h"

static void
dumpInfo(void) ;
static bool
testNullValues(struct CNValuePool * vpool) ;
static bool
testBooleanValues(struct CNValuePool * vpool) ;
static bool
testIntValues(struct CNValuePool * vpool) ;

bool
UTValue(struct CNValuePool * vpool)
{
        dumpInfo() ;
        bool res0 = testNullValues(vpool) ;
        bool res1 = testBooleanValues(vpool) ;
        bool res2 = testIntValues(vpool) ;
        return res0 && res1 && res2 ;
}

static void
dumpInfo(void)
{
        CNInterface()->printf("null value          : %u\n", sizeof(struct CNNullValue)) ;
        CNInterface()->printf("boolean value       : %u\n", sizeof(struct CNBooleanValue)) ;
        CNInterface()->printf("signed int value    : %u\n", sizeof(struct CNSignedIntValue)) ;
        CNInterface()->printf("unsigned int value  : %u\n", sizeof(struct CNUnsignedIntValue)) ;
        CNInterface()->printf("float value         : %u\n", sizeof(struct CNFloatValue)) ;
        CNInterface()->printf("string value        : %u\n", sizeof(struct CNStringValue)) ;
        CNInterface()->printf("array value         : %u\n", sizeof(struct CNArrayValue)) ;
        CNInterface()->printf("dictionary value    : %u\n", sizeof(struct CNDictionaryValue)) ;
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

static bool
testBooleanValues(struct CNValuePool * vpool)
{
        CNInterface()->printf("(%s) Boolean value test\n", __func__) ;
        struct CNBooleanValue * bval = CNAllocateBooleanValue(vpool, true) ;
        CNPrintValueAttribute(CNSuperClassOfBooleanValue(bval)) ;
        CNReleaseValue(vpool, CNSuperClassOfBooleanValue(bval)) ;
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
