/*
 * @file CNRegisterFile.c
 * @description Define CNRegisterFile data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNRegisterFile.h"
#include "CNNullValue.h"
#include "CNInterface.h"

void
CNInitRegisterFile(struct CNRegisterFile * dst, struct CNValuePool * vpool)
{
        dst->valuePool          = vpool ;
        dst->currentIndex       = 0 ;
        for(unsigned int i=0 ; i < CNRegisterPageNum ; i++){
                dst->registerPages[i] = CNAllocateArrayValue(vpool, CNRegisterNumInPage) ;
        }
}

void
CNDeinitRegisterFile(struct CNRegisterFile * dst)
{
        for(unsigned int i=0 ; i < CNRegisterPageNum ; i++){
                CNReleaseValue(dst->valuePool, CNSuperClassOfArrayValue(dst->registerPages[i])) ;
        }
}

void
CNSetValueToRegisterFile(struct CNRegisterFile * src, index_t regid, struct CNValue * val)
{
        uint32_t idx     = regid ;
        uint32_t pagenum = idx / CNRegisterNumInPage ;
        uint32_t regoff  = idx % CNRegisterNumInPage ;
        if(pagenum < CNRegisterPageNum){
                CNSetValueToArray(src->valuePool, src->registerPages[pagenum], regoff, val) ;
        } else {
                CNInterface()->error("[Error] Register ID overflow at %s\n", __func__) ;
        }
}

struct CNValue *
CNValueInRegisterFile(struct CNRegisterFile * src, index_t regid)
{
        struct CNValue * result ;
        index_t idx     = regid ;
        index_t pagenum = idx / CNRegisterNumInPage ;
        index_t regoff  = idx % CNRegisterNumInPage ;
        if(pagenum < CNRegisterPageNum){
                result = CNValueInArray(src->registerPages[pagenum], regoff) ;
        } else {
                CNInterface()->error("[Error] Register ID overflow at %s\n", __func__) ;
                result = NULL ;
        }
        return result ;
}

