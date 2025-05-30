/*
 * @file UTRegisterFile.c
 * @description The unit test for CNRegisterFile
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTRegisterFile.h"
#include "UTUtils.h"
#include <stdio.h>
#include <stdlib.h>

static void
setValue(struct CNRegisterFile * dst, index_t idx, uint64_t val) ;

static uint64_t
getValue(struct CNRegisterFile * src, index_t idx) ;

bool UTRegisterFile(struct CNValuePool * vpool)
{
        bool result = true ;
        struct CNRegisterFile regfile ;

        CNInterface()->printf("(%s) RegisterFile test\n", __func__) ;

        CNInterface()->printf("(%s) Allocation state\n", __func__) ;
        CNInitRegisterFile(&regfile, vpool) ;

        setValue(&regfile, 0, 123) ;
        if(getValue(&regfile, 0) != 123){
                CNInterface()->error("(%s) Failed to get value\n", __func__) ;
                result = false ;
        }

        setValue(&regfile, 1024, 456) ;
        if(getValue(&regfile, 1024) != 456){
                CNInterface()->error("(%s) Failed to get value\n", __func__) ;
                result = false ;
        }

        setValue(&regfile, 4096-1, 789) ;
        if(getValue(&regfile, 4096-1) != 789){
                CNInterface()->error("(%s) Failed to get value\n", __func__) ;
                result = false ;
        }

        CNInterface()->printf("(%s) Release state\n", __func__) ;
        CNDeinitRegisterFile(&regfile) ;
        
        return checkMemoryUsage(vpool) && result ;
}

static void
setValue(struct CNRegisterFile * dst, index_t idx, uint64_t src)
{
        struct CNUnsignedIntValue * val = CNAllocateUnsignedIntValue(dst->valuePool, src) ;
        CNSetValueToRegisterFile(dst, idx, CNSuperClassOfUnsignedIntValue(val)) ;
        CNReleaseValue(dst->valuePool, CNSuperClassOfUnsignedIntValue(val)) ;
}

static uint64_t
getValue(struct CNRegisterFile * src, index_t idx)
{
        struct CNValue * val = CNValueInRegisterFile(src, idx) ;
        if(val != NULL){
                struct CNUnsignedIntValue * uval ;
                uval = CNCastToUnsignedIntValue(val) ;
                if(uval != NULL){
                        return uval->value ;
                }
        }
        CNInterface()->printf("(%s) [Error] Unexpected index: %u\n", __func__, idx) ;
        return -1 ;
}
