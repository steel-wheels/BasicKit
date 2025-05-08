/*
 * @file CNRegisters.h
 * @description Define CNRegisters data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNRegisters_h
#define CNRegisters_h

#import <BasicKit/CNType.h>

#define CNRegisterPageNum               256
#define CNRegisterElementsInPage        0x1000

struct CNRegisters {
        struct CNValuePool *    valuePool ;
        struct CNValue *        registerArray[CNRegisterPageNum] ;
        uint64_t                base_index ;
        uint64_t                current_index ;
} ;

#define CNLastSpecialValueRegister      CNFalseValueRegister
typedef enum {
        CNTrueValueRegister     = 0x0,  // register id
        CNFalseValueRegister    = 0x1   // register id
} CNSpecialValueRegister ;

void
CNInitRegisters(struct CNRegisters * dst, struct CNValuePool * vpool) ;

void
CNDeinitRegisters(struct CNRegisters * dst) ;

static inline uint64_t
CNAllocateRegisterInRegisters(struct CNRegisters * dst)
{
        uint64_t result = dst->current_index ;
        dst->current_index += 1 ;
        return result ;
}

struct CNValue *
CNValueInRegisters(struct CNRegisters * src, uint64_t index) ;

void
CNSetValueToRegisters(struct CNRegisters * dst, uint64_t index, struct CNValue * src) ;

static inline struct CNValue *
CNSpecialValueInRegisters(struct CNRegisters * src, CNSpecialValueRegister spec)
{
        return CNValueInRegisters(src, (uint64_t) spec) ;
}

#endif /* CNRegisters_h */
