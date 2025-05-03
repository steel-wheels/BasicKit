/*
 * @file UTRegisters.c
 * @description The unit test for CNRegisters
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import "UTRegisters.h"

static bool
get(struct CNRegisters * src, unsigned int index) ;
static bool
set(struct CNRegisters * dst, unsigned int index, unsigned int data, struct CNValuePool * vpool) ;

bool
UTRegisters(void)
{
        bool result = true ;
        struct CNMemoryUsage usage ;
        
        printf("(%s) Initial state\n", __func__) ;
        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        struct CNValuePool vpool ;
        CNInitValuePool(&vpool, &lpool) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        printf("(%s) Allocate state\n", __func__) ;
        struct CNRegisters regs ;
        CNInitRegisters(&regs, &vpool) ;

        result &= get(&regs, 0) ;
        result &= set(&regs, 0, 123, &vpool) ;
        result &= get(&regs, 0) ;

        result &= get(&regs, 1024) ;
        result &= set(&regs, 102, 123, &vpool) ;
        result &= get(&regs, 0) ;

        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        printf("(%s) Release state\n", __func__) ;
        CNDeinitRegisters(&regs) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;
        if(usage.allocatedSize == usage.usableSize) {
                printf("(%s) No memory leak\n", __func__) ;
        } else {
                printf("(%s) [Error] some memory leak\n", __func__) ;
                result = false ;
        }

        printf("(%s) Final state\n", __func__) ;
        CNDeinitValuePool(&vpool) ;
        CNDeinitListPool(&lpool) ;
        return result ;
}

static bool
get(struct CNRegisters * src, unsigned int index)
{
        printf("(%s) get %u -> ", __func__, index) ;

        struct CNValue * result ;
        result = CNValueInRegisters(src, index) ;
        CNDumpValue(0, result) ;

        return true ;
}

static bool
set(struct CNRegisters * dst, unsigned int index, unsigned int data, struct CNValuePool * vpool)
{
        struct CNValue * newval = CNAllocateUnsignedInt(data, vpool) ;

        printf("(%s) set %u <- ", __func__, index) ;
        CNDumpValue(0, newval) ;

        CNSetValueToRegisters(dst, index, newval) ;
        CNReleaseValue(vpool, newval) ;
        return true ;
}

