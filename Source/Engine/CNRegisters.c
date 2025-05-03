/*
 * @file CNRegisters.c
 * @description Define CNRegisters data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNRegisters.h>
#import <BasicKit/CNValue.h>

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
