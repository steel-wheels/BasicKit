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
} ;

void
CNInitRegisters(struct CNRegisters * dst, struct CNValuePool * vpool) ;

void
CNDeinitRegisters(struct CNRegisters * dst) ;

struct CNValue *
CNValueInRegisters(struct CNRegisters * src, unsigned int index) ;

void
CNSetValueToRegisters(struct CNRegisters * dst, unsigned int index, struct CNValue * src) ;

#endif /* CNRegisters_h */
