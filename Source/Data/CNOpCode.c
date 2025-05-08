/*
 * @file CNOpCode.h
 * @description Define CNOpCode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNOpCode.h>
#import <BasicKit/CNValue.h>
#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNInterface.h>
#import <BasicKit/CNUtils.h>

void
CNRetainOpCode(struct CNOpCode * dst)
{
        switch(dst->operandType){
                case CNExecOperand: {
                        /* do noting */
                } break ;
                case CNStorageOperand: {
                        CNRetainValue((dst->storageOperands).sourceValue) ;
                } break ;
        }
}

void
CNReleaseOpCode(struct CNValuePool * vpool, struct CNOpCode * dst)
{
        switch(dst->operandType){
                case CNExecOperand: {
                        /* do noting */
                } break ;
                case CNStorageOperand: {
                        CNReleaseValue(vpool, (dst->storageOperands).sourceValue) ;
                } break ;
        }
}

static int
compareExecOperands(const struct CNExecOperands * s0, const struct CNExecOperands * s1)
{
        int64_t diff ;
        if((diff = ((int64_t) s0->destinationRegId) - ((int64_t) s1->destinationRegId)) != 0){
                return (int) diff ;
        }
        if((diff = ((int64_t) s0->source0RegId) - ((int64_t) s1->source0RegId)) != 0){
                return (int) diff ;
        }
        if((diff = ((int64_t) s0->source1RegId) - ((int64_t) s1->source1RegId)) != 0){
                return (int) diff ;
        }
        return 0 ;
}

static int
compareStorageOperands(const struct CNStorageOperands * s0, const struct CNStorageOperands * s1)
{
        int64_t diff ;
        if((diff = ((int64_t) s0->destinationRegId) - ((int64_t) s1->destinationRegId)) != 0){
                return (int) diff ;
        }
        return CNCompareValue(s0->sourceValue, s1->sourceValue) ;
}

int
CNCompareOpCode(const struct CNOpCode * s0, const struct CNOpCode * s1)
{
        int diff ;
        if((diff = s0->code - s1->code) != 0){
                return diff ;
        }
        if((diff = s0->operandType - s1->operandType) != 0){
                return diff ;
        }
        switch(s0->operandType){
                case CNExecOperand: {
                        diff = compareExecOperands(&(s0->execOperands), &(s1->execOperands)) ;
                } break ;
                case CNStorageOperand: {
                        diff = compareStorageOperands(&(s0->storageOperands), &(s1->storageOperands)) ;
                } break ;
        }
        return diff ;
}

void
CNPrintOpCode(const struct CNOpCode * src)
{
        CNInterface()->printf("{\n") ;
        CNInterface()->printf("  code:          0x%x\n", src->code) ;
        CNInterface()->printf("  operand-type:  0x%x\n", src->operandType) ;
        switch(src->operandType){
                case CNExecOperand: {
                        const struct CNExecOperands * operands = &(src->execOperands) ;
                        CNInterface()->printf("  destination:  0x%x\n", operands->destinationRegId) ;
                        CNInterface()->printf("  source-0:     0x%x\n", operands->source0RegId) ;
                        CNInterface()->printf("  source-1:     0x%x\n", operands->source1RegId) ;
                } break ;
                case CNStorageOperand: {
                        const struct CNStorageOperands * operands = &(src->storageOperands) ;
                        CNInterface()->printf("  destination:  0x%x\n", operands->destinationRegId) ;
                        CNInterface()->printf("  source:       ") ;
                        CNPrintValue(operands->sourceValue) ;
                        CNInterface()->printf("\n") ;
                } break ;
        }
        CNInterface()->printf("}\n") ;
}
