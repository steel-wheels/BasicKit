/*
 * @file CNRegisters.c
 * @description Define CNRegisters data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNRegisters.h>
#import <BasicKit/CNValue.h>
#import <BasicKit/CNInterface.h>

void
CNInitRegisters(struct CNRegisters * dst, struct CNValuePool * vpool)
{
        dst->valuePool = vpool ;
        for(unsigned int i = 0 ; i < CNRegisterPageNum ; i++){
                dst->registerArray[i] = CNAllocateNull() ;
        }
}

void
CNDeinitRegisters(struct CNRegisters * dst)
{
        struct CNValuePool * vpool = dst->valuePool ;
        for(unsigned int i = 0 ; i < CNRegisterPageNum ; i++){
                CNReleaseValue(vpool, dst->registerArray[i]) ;
        }
}

static inline bool
checkIndex(uint64_t * page, uint64_t * offset, uint64_t index)
{
        *page   = index / CNRegisterElementsInPage ;
        *offset = index % CNRegisterElementsInPage ;
        if(*page < CNRegisterPageNum){
                return true ;
        } else {
                CNInterface()->error("[Error] index out of range: %u\n", *page) ;
                return false ;
        }
}

struct CNValue *
CNValueInRegisters(struct CNRegisters * src, uint64_t index)
{
        uint64_t page, offset ;
        if(!checkIndex(&page, &offset, index)) {
                return CNAllocateNull() ;
        }
        struct CNValue * array = src->registerArray[page] ;
        if(CNTypeOfValue(array) != CNArrayType) {
                array = CNAllocateArray(CNRegisterElementsInPage, src->valuePool) ;
                src->registerArray[page] = array ;
        }
        return CNElementInArray(&(array->arrayValue), offset) ;
}

void
CNSetValueToRegisters(struct CNRegisters * dst, uint64_t index, struct CNValue * src)
{
        uint64_t page, offset ;
        if(!checkIndex(&page, &offset, index)) {
                return ;
        }
        struct CNValue * array = dst->registerArray[page] ;
        if(CNTypeOfValue(array) != CNArrayType) {
                array = CNAllocateArray(CNRegisterElementsInPage, dst->valuePool) ;
                dst->registerArray[page] = array ;
        }
        CNSetElementToArray(dst->valuePool, &(array->arrayValue), offset, src) ;
}
