/*
 * @file CNByteCode.h
 * @description Define CNByteCode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNByteCode_h
#define CNByteCode_h

#import <BasicKit/CNCodeValue.h>

typedef enum {
        CNNopCode,
        CNLoadCode,
        CNPrintCode
} CNOpCode ;

static inline struct CNCodeValue *
CNAllocateNopCode(struct CNValuePool * vpool)
{
        return CNAllocateCalcCodeValue(vpool, CNNopCode, 0, 0, 0) ;
}

static inline struct CNCodeValue *
CNAllocateLoadCode(struct CNValuePool * vpool, uint64_t dstreg, struct CNValue * srcval)
{
        return CNAllocateLoadCodeValue(vpool, CNLoadCode, dstreg, srcval) ;
}

static inline struct CNCodeValue *
CNAllocatePrintCode(struct CNValuePool * vpool, uint64_t regid)
{
        return CNAllocateCalcCodeValue(vpool, CNPrintCode, 0, regid, 0) ;
}

void
CNPrintByteCode(const struct CNCodeValue * src) ;

#endif /* CNByteCode_h */
