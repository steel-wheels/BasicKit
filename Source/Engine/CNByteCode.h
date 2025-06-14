/*
 * @file CNByteCode.h
 * @description Define CNByteCode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNByteCode_h
#define CNByteCode_h

#import <BasicKit/CNCodeValue.h>
#import <BasicKit/CNValueList.h>
#import <BasicKit/CNArrayValue.h>
#import <BasicKit/CNOperation.h>

typedef enum {
        CNNopCode,
        CNStopCode,
        CNMoveCode,
        CNConvI2UCode,
        CNConvF2UCode,
        CNConvU2ICode,
        CNConvF2ICode,
        CNConvU2FCode,
        CNConvI2FCode,
        CNLoadCode,
        CNLogicalAndCode,
        CNLogicalOrCode,
        CNLogicalNotCode,
        CNBitAndCode,
        CNBitOrCode,
        CNBitXorCode,
        CNBitShiftLeftCode,
        CNBitShiftRightCode,
        CNEqualBoolCode,
        CNEqualUnsignedIntCode,
        CNEqualSignedIntCode,
        CNEqualFloatCode,
        CNEqualStringCode,
        CNNotEqualBoolCode,
        CNNotEqualUnsignedIntCode,
        CNNotEqualSignedIntCode,
        CNNotEqualFloatCode,
        CNNotEqualStringCode,
        CNLessThanBoolCode,
        CNLessThanUnsignedIntCode,
        CNLessThanSignedIntCode,
        CNLessThanFloatCode,
        CNLessThanStringCode,
        CNLessEqualBoolCode,
        CNLessEqualUnsignedIntCode,
        CNLessEqualSignedIntCode,
        CNLessEqualFloatCode,
        CNLessEqualStringCode,
        CNGreaterThanBoolCode,
        CNGreaterThanUnsignedIntCode,
        CNGreaterThanSignedIntCode,
        CNGreaterThanFloatCode,
        CNGreaterThanStringCode,
        CNGreateEqualBoolCode,
        CNGreateEqualUnsignedIntCode,
        CNGreateEqualSignedIntCode,
        CNGreateEqualFloatCode,
        CNGreateEqualStringCode,
        CNAddUnsignedIntCode,
        CNAddSignedIntCode,
        CNAddFloatCode,
        CNAddStringCode,
        CNSubUnsignedIntCode,
        CNSubSignedIntCode,
        CNSubFloatCode,
        CNMultUnsignedIntCode,
        CNMultSignedIntCode,
        CNMultFloatCode,
        CNDivUnsignedIntCode,
        CNDivSignedIntCode,
        CNDivFloatCode,
        CNModUnsignedIntCode,
        CNModSignedIntCode,
        CNNegateSignedIntCode,
        CNNegateFloatCode,
        CNBitNotCode,
        CNJumpCode,
        CNBranchThenCode,
        CNBranchElseCode,
        CNPrintCode
} CNOpCode ;

static inline struct CNCodeValue *
CNAllocateNopCode(struct CNValuePool * vpool)
{
        return CNAllocateCalcCodeValue(vpool, CNNopCode, 0, 0, 0) ;
}

static inline struct CNCodeValue *
CNAllocateStopCode(struct CNValuePool * vpool)
{
        return CNAllocateCalcCodeValue(vpool, CNStopCode, 0, 0, 0) ;
}

static inline struct CNCodeValue *
CNAllocateMoveCode(struct CNValuePool * vpool, uint64_t dstreg, uint64_t srcreg)
{
        return CNAllocateCalcCodeValue(vpool, CNMoveCode, dstreg, srcreg, 0) ;
}

static inline struct CNCodeValue *
CNAllocateLoadCode(struct CNValuePool * vpool, uint64_t dstreg, struct CNValue * srcval)
{
        return CNAllocateLoadCodeValue(vpool, CNLoadCode, dstreg, srcval) ;
}

struct CNCodeValue *
CNAllocateConvertCode(struct CNValuePool * vpool, CNValueType dtype, uint64_t dstreg,
                      CNValueType stype, uint64_t srcreg) ;

struct CNCodeValue *
CNAllocateLogicalBinaryCode(struct CNValuePool * vpool, CNLogicalBinaryOperation op, uint64_t dstreg, uint64_t src0reg, uint64_t src1reg) ;

struct CNCodeValue *
CNAllocateBitBinaryCode(struct CNValuePool * vpool, CNBitBinaryOperation op, uint64_t dstreg, uint64_t src0reg, uint64_t src1reg) ;

struct CNCodeValue *
CNAllocateCompareCode(struct CNValuePool * vpool, CNCompareOperation op, uint64_t dstreg, CNValueType srctype, uint64_t src0reg, uint64_t src1reg) ;

struct CNCodeValue *
CNAllocateNumberBinaryCode(struct CNValuePool * vpool, CNNumberBinaryOperation op, uint64_t dstreg, CNValueType srctype, uint64_t src0reg, uint64_t src1reg) ;

struct CNCodeValue *
CNAllocateNumberUnaryCode(struct CNValuePool * vpool, CNNumberUnaryOperation op, uint64_t dstreg, CNValueType srctype, uint64_t srcreg) ;

struct CNCodeValue *
CNAllocateBitUnaryCode(struct CNValuePool * vpool, CNBitUnaryOperation op, uint64_t dstreg, uint64_t srcreg) ;

struct CNCodeValue *
CNAllocateLogicalUnaryCode(struct CNValuePool * vpool, CNLogicalUnaryOperation op, uint64_t dstreg, CNValueType srctype, uint64_t srcreg) ;

struct CNCodeValue *
CNAllocateJumpCode(struct CNValuePool * vpool, uint32_t label) ;

struct CNCodeValue *
CNAllocateBranchThenCode(struct CNValuePool * vpool, uint64_t regid, uint32_t label) ;

struct CNCodeValue *
CNAllocateBranchElseCode(struct CNValuePool * vpool, uint64_t regid, uint32_t label) ;

static inline struct CNCodeValue *
CNAllocatePrintCode(struct CNValuePool * vpool, uint64_t regid)
{
        return CNAllocateCalcCodeValue(vpool, CNPrintCode, 0, regid, 0) ;
}

void
CNPrintByteCode(const struct CNCodeValue * src) ;

void
CNDumpByteCodeInValueList(struct CNValueList * src) ;

void
CNDumpByteCodeInArrayValue(struct CNArrayValue * src) ;

#endif /* CNByteCode_h */
