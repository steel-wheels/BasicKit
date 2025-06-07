/*
 * @file CNOperation.h
 * @description Define CNByteCode executer
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNOperation_h
#define CNOperation_h

#import <BasicKit/CNType.h>

typedef enum {
        CNBitAndOperation,
        CNBitOrOperation,
        CNBitXorOperation,
        CNBitShiftLeftOperation,
        CNBitShiftRightOperation
} CNBitBinaryOperation ;

typedef enum {
        CNAddOperation,
        CNSubOperation,
        CNMultOperation,
        CNDivFloatOperation,
        CNDivIntOperation,
        CNModOperation
} CNNumberBinaryOperation ;

typedef enum {
        CNNegateOperation,
} CNNumberUnaryOperation ;

typedef enum {
        CNLogicalAndOperation,
        CNLogicalOrOperation
} CNLogicalBinaryOperation ;

typedef enum {
        CNCompareEqual,
        CNCompareNotEqual,
        CNCompareLessThan,
        CNCompareLessEqual,
        CNCompareGreaterThan,
        CNCompareGreateEqual
} CNCompareOperation ;

const char *
CNArithmeticOperationName(CNNumberBinaryOperation op) ;

#endif /* CNOperation_h */
