/*
 * @file CNOpCode.h
 * @description Define CNOpCode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNOpCode_h
#define CNOpCode_h

#import <BasicKit/CNType.h>

typedef enum {
        CNExecOperand,
        CNStorageOperand
} CNOperandType ;

struct CNExecOperands {
        uint64_t                destinationRegId ;
        uint64_t                source0RegId ;
        uint64_t                source1RegId ;
} ;

struct CNStorageOperands {
        uint64_t                destinationRegId ;
        struct CNValue *        sourceValue ;
} ;

struct CNOpCode {
        uint32_t                        code ;
        CNOperandType                   operandType ;
        union {
                struct CNExecOperands      execOperands ;
                struct CNStorageOperands   storageOperands ;
        } ; // no name
} ;

void
CNRetainOpCode(struct CNOpCode * dst) ;

void
CNReleaseOpCode(struct CNValuePool * pool, struct CNOpCode * dst) ;

static inline void
CNDeinitOpCode(struct CNValuePool * vpool, struct CNOpCode * dst)
{
        (void) vpool ;
        dst->code = 0 ;
}

int
CNCompareOpCode(const struct CNOpCode * s0, const struct CNOpCode * s1) ;

void
CNPrintOpCode(const struct CNOpCode * src) ;

#endif /* CNOpCode_h */
