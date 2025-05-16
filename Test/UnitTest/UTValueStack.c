/*
 * @file UTValueStack.c
 * @description The unit test for CNValueStack
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTValueStack.h"
#include "UTUtils.h"

static void
pushInt(struct CNValueStack * dst, uint64_t src) ;
static bool
popInt(struct CNValueStack * dst, uint64_t exp) ;

bool
UTValueStack(struct CNValuePool * vpool)
{
        bool result = true ;

        CNInterface()->printf("(%s) ValueStack test\n", __func__) ;

        CNInterface()->printf("(%s) allocation\n", __func__) ;
        struct CNValueStack stack ;
        CNInitValueStack(&stack, vpool) ;

        for(uint64_t i=1 ; i<100 ; i++){
                pushInt(&stack, i) ;
        }
        for(uint64_t i=99 ; i>0 ; i--){
                result &= popInt(&stack, i) ;
        }

        CNInterface()->printf("(%s) release\n", __func__) ;
        CNDeinitValueStack(&stack) ;

        return checkMemoryUsage(vpool) ;
}

static void
pushInt(struct CNValueStack * dst, uint64_t src)
{
        struct CNValuePool * vpool = dst->valuePool ;
        struct CNUnsignedIntValue * uval = CNAllocateUnsignedIntValue(vpool, src) ;
        CNPushValueToStack(dst, CNSuperClassOfUnsignedIntValue(uval)) ;
        CNReleaseValue(vpool, CNSuperClassOfUnsignedIntValue(uval)) ;
}

static bool
popInt(struct CNValueStack * src, uint64_t exp)
{
        bool result ;
        struct CNValuePool * vpool = src->valuePool ;
        struct CNValue * retval = CNPopValueFromStack(src) ;
        if(retval != NULL){
                struct CNUnsignedIntValue * uval = CNCastToUnsignedIntValue(retval) ;
                if(uval != NULL){
                        if(uval->value == exp){
                                result = true ;
                        } else {
                                CNInterface()->printf("(%s) [Error] Unexpected value %lu<->%lu\n",
                                                      __func__, uval->value, exp) ;
                                result = false ;
                        }
                        CNReleaseValue(vpool, retval) ;
                } else {
                        CNInterface()->printf("(%s) [Error] Can not happen\n", __func__) ;
                        result = false ;
                }
        } else {
                result = (exp == 0) ;
        }
        return result ;
}
