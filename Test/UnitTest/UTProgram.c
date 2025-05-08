/*
 * @file UTProgram.c
 * @description The unit test for CNProgram data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTProgram.h"
#include <string.h>

bool
UTProgram(void)
{
        bool result = true ;
        struct CNMemoryUsage usage ;

        printf("(%s) Initial state\n", __func__) ;
        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        struct CNValuePool vpool ;
        CNInitValuePool(&vpool, &lpool) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

        printf("(%s) Allocate state\n", __func__) ;
        struct CNProgram prg ;
        CNInitProgram(&prg, &vpool) ;

        uint64_t regid0 = CNAllocateRegisterInProgram(&prg) ;
        printf("(%s) allocate reg id0: %llu\n", __func__, regid0) ;

        uint64_t regid1 = CNAllocateRegisterInProgram(&prg) ;
        printf("(%s) allocate reg id1: %llu\n", __func__, regid1) ;

        const char * str0 = "var0" ;
        struct CNValue * ident0 = CNAllocateString(str0, (uint32_t) strlen(str0), &vpool) ;
        if(CNRegisterIdForIdentifier(&regid0, &prg, ident0)){
                printf("[Error] Can not happen (1)\n") ;
                result = false ;
        }
        regid1 = CNAllocateRegisterIdForIdentifier(&prg, ident0) ;
        printf("(%s) allocate reg id1: %llu\n", __func__, regid1) ;

        uint64_t regid2 ;
        if(CNRegisterIdForIdentifier(&regid2, &prg, ident0)){
                if(regid1 != regid2){
                        printf("(%s) Unexpected reg id: 0x%llu <-> 0x%llu\n", __func__, regid1, regid2) ;
                        result = false ;
                }
        } else {
                printf("(%s) [Error] Can not happen (2)\n", __func__) ;
                result = false ;
        }

        struct CNValue * opcode0 = CNAllocateMoveByteCode(&vpool, 0, 1) ;
        struct CNValue * opcode1 = CNAllocateStoreByteCode(&vpool, 2, ident0) ;
        CNAppendCodeToProgram(&prg, opcode0) ;
        CNAppendCodeToProgram(&prg, opcode1) ;
        CNReleaseValue(&vpool, opcode0) ;
        CNReleaseValue(&vpool, opcode1) ;
        CNDumpProgram(0, &prg) ;

        /*
         static inline void
         CNAppendCodeToProgram(struct CNProgram * dst, struct CNValue * opcode)
         {
                 CNAppendToValueList(&(dst->program), opcode) ;
         }

         void
         CNDumpProgram(unsigned int indent, struct CNProgram * src) ;

         */

        printf("(%s) Release state\n", __func__) ;
        CNReleaseValue(&vpool, ident0) ;
        CNDeinitProgram(&prg) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNPrintMemoryUsage(&usage) ;

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
