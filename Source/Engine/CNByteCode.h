/*
 * @file CNByteCode.h
 * @description Define CNByteCode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNByteCode_h
#define CNByteCode_h

#import <BasicKit/CNValue.h>

/* The byte code must be 12 bit */
typedef enum {
        CNMoveByteCode          = 0x001,
        CNStoreByteCode         = 0x002,
        CNPrintByteCode         = 0x003
} CNByteCode ;

static inline struct CNValue *
CNAllocateMoveByteCode(struct CNValuePool * vpool, uint64_t dstregid, uint64_t srcregid)
{
        return CNAllocateOpCodeWithExecOperands(vpool, CNMoveByteCode, dstregid, srcregid, 0) ;
}

static inline struct CNValue *
CNAllocateStoreByteCode(struct CNValuePool * vpool, uint64_t dstregid, struct CNValue * src)
{
        return CNAllocateOpCodeWithStorageOperands(vpool, CNStoreByteCode, dstregid, src) ;
}

static inline struct CNValue *
CNAllocatePrintByteCode(struct CNValuePool * vpool, uint64_t srcregid)
{
        return CNAllocateOpCodeWithExecOperands(vpool, CNPrintByteCode, 0, srcregid, 0) ;
}

void
_CNPrintByteCode(const struct CNValue * src) ;

#endif /* CNByteCode_h */
