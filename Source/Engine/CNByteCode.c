/*
 * @file CNByteCode.c
 * @description Define CNByteCode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNByteCode.h>
#import <BasicKit/CNInterface.h>

struct CNCodeValue *
CNAllocateConvertCode(struct CNValuePool * vpool, CNValueType dtype, uint64_t dstreg,
                      CNValueType stype, uint64_t srcreg)
{
        CNOpCode opcode = CNNopCode ;
        switch(dtype){
                case CNUnsignedIntType: {
                        switch(stype){
                                case CNSignedIntType: {
                                        opcode = CNConvI2UCode ;
                                } break ;
                                case CNFloatType: {
                                        opcode = CNConvF2UCode ;
                                } break ;
                                case CNUnsignedIntType:
                                case CNNullType:
                                case CNBooleanType:
                                case CNStringType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        CNInterface()->error("[Error] Unexpected source type: %u\n", stype) ;
                                        return NULL ;
                                } break ;
                        }
                } break ;
                case CNSignedIntType: {
                        switch(stype){
                                case CNUnsignedIntType: {
                                        opcode = CNConvU2ICode ;
                                } break ;
                                case CNFloatType: {
                                        opcode = CNConvF2ICode ;
                                } break ;
                                case CNSignedIntType:
                                case CNNullType:
                                case CNBooleanType:
                                case CNStringType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        CNInterface()->error("[Error] Unexpected source type: %u\n", stype) ;
                                        return NULL ;
                                } break ;
                        }
                } break ;
                case CNFloatType: {
                        switch(stype){
                                case CNUnsignedIntType: {
                                        opcode = CNConvU2FCode ;
                                } break ;
                                case CNSignedIntType: {
                                        opcode = CNConvI2FCode ;
                                } break ;
                                case CNFloatType:
                                case CNNullType:
                                case CNBooleanType:
                                case CNStringType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        CNInterface()->error("[Error] Unexpected source type: %u\n", stype) ;
                                        return NULL ;
                                } break ;
                        }
                } break ;
                case CNNullType:
                case CNBooleanType:
                case CNStringType:
                case CNArrayType:
                case CNDictionaryType:
                case CNCodeType: {
                        CNInterface()->error("[Error] Unexpected destination type: %u\n", dtype) ;
                        return NULL ;
                } break ;
        }
        return CNAllocateCalcCodeValue(vpool, opcode, dstreg, srcreg, 0) ;
}

struct CNCodeValue *
CNAllocateLogicalOperationCode(struct CNValuePool * vpool, CNLogicalOperationType op, uint64_t dstreg, uint64_t src0reg, uint64_t src1reg)
{
        CNOpCode opcode = CNNopCode ;
        switch(op){
                case CNLogicalOrOperation:      opcode = CNLogicalOrCode ;      break ;
                case CNLogicalAndOperation:     opcode = CNLogicalAndCode ;     break ;
        }
        return CNAllocateCalcCodeValue(vpool, opcode, dstreg, src0reg, src1reg) ;
}

struct CNCodeValue *
CNAllocateBitOperationCode(struct CNValuePool * vpool, CNBitOperationType op, uint64_t dstreg, uint64_t src0reg, uint64_t src1reg)
{
        CNOpCode opcode = CNNopCode ;
        switch(op){
                case CNBitAndOperation:             opcode = CNBitAndCode ;             break ;
                case CNBitOrOperation:              opcode = CNBitOrCode ;              break ;
                case CNBitXorOperation:             opcode = CNBitXorCode ;             break ;
                case CNBitShiftLeftOperation:       opcode = CNBitShiftLeftCode ;       break ;
                case CNBitShiftRightOperation:      opcode = CNBitShiftRightCode ;      break ;
        }
        return CNAllocateCalcCodeValue(vpool, opcode, dstreg, src0reg, src1reg) ;
}

struct CNCodeValue *
CNAllocateCompareCode(struct CNValuePool * vpool, CNCompareOperation ctype, uint64_t dstreg, CNValueType srctype, uint64_t src0reg, uint64_t src1reg)
{
        CNOpCode opcode = CNNopCode ;
        switch(srctype){
                case CNBooleanType: {
                        switch(ctype){
                                case CNCompareEqual:            opcode = CNEqualBoolCode ;              break ;
                                case CNCompareNotEqual:         opcode = CNNotEqualBoolCode ;           break ;
                                case CNCompareLessThan:         opcode = CNLessThanBoolCode ;           break ;
                                case CNCompareLessEqual:        opcode = CNLessEqualBoolCode ;          break ;
                                case CNCompareGreaterThan:      opcode = CNGreaterThanBoolCode ;        break ;
                                case CNCompareGreateEqual:      opcode = CNGreateEqualBoolCode ;        break ;
                        }
                } break ;
                case CNUnsignedIntType: {
                        switch(ctype){
                                case CNCompareEqual:            opcode = CNEqualUnsignedIntCode ;               break ;
                                case CNCompareNotEqual:         opcode = CNNotEqualUnsignedIntCode ;            break ;
                                case CNCompareLessThan:         opcode = CNLessThanUnsignedIntCode ;           break ;
                                case CNCompareLessEqual:        opcode = CNLessEqualUnsignedIntCode ;          break ;
                                case CNCompareGreaterThan:      opcode = CNGreaterThanUnsignedIntCode ;        break ;
                                case CNCompareGreateEqual:      opcode = CNGreateEqualUnsignedIntCode ;        break ;
                        }
                } break ;
                case CNSignedIntType: {
                        switch(ctype){
                                case CNCompareEqual:            opcode = CNEqualSignedIntCode ;                 break ;
                                case CNCompareNotEqual:         opcode = CNNotEqualSignedIntCode ;              break ;
                                case CNCompareLessThan:         opcode = CNLessThanSignedIntCode ;              break ;
                                case CNCompareLessEqual:        opcode = CNLessEqualSignedIntCode ;             break ;
                                case CNCompareGreaterThan:      opcode = CNGreaterThanSignedIntCode ;           break ;
                                case CNCompareGreateEqual:      opcode = CNGreateEqualSignedIntCode ;           break ;
                        }
                } break ;
                case CNFloatType: {
                        switch(ctype){
                                case CNCompareEqual:            opcode = CNEqualFloatCode ;                     break ;
                                case CNCompareNotEqual:         opcode = CNNotEqualFloatCode ;                  break ;
                                case CNCompareLessThan:         opcode = CNLessThanFloatCode ;                  break ;
                                case CNCompareLessEqual:        opcode = CNLessEqualFloatCode ;                 break ;
                                case CNCompareGreaterThan:      opcode = CNGreaterThanFloatCode ;               break ;
                                case CNCompareGreateEqual:      opcode = CNGreateEqualFloatCode ;               break ;
                        }
                } break ;
                case CNStringType: {
                        switch(ctype){
                                case CNCompareEqual:            opcode = CNEqualStringCode ;                    break ;
                                case CNCompareNotEqual:         opcode = CNNotEqualStringCode ;                 break ;
                                case CNCompareLessThan:         opcode = CNLessThanStringCode ;                 break ;
                                case CNCompareLessEqual:        opcode = CNLessEqualStringCode ;                break ;
                                case CNCompareGreaterThan:      opcode = CNGreaterThanStringCode ;              break ;
                                case CNCompareGreateEqual:      opcode = CNGreateEqualStringCode ;              break ;
                        }
                } break ;
                case CNNullType:
                case CNArrayType:
                case CNDictionaryType:
                case CNCodeType: {
                        return NULL ;
                } break ;
        }
        return CNAllocateCalcCodeValue(vpool, opcode, dstreg, src0reg, src1reg) ;
}

struct CNCodeValue *
CNAllocateArithmeticCode(struct CNValuePool * vpool, CNArithmeticOperation op, uint64_t dstreg, CNValueType srctype, uint64_t src0reg, uint64_t src1reg)
{
        CNOpCode opcode = CNNopCode ;
        switch(srctype){
                case CNUnsignedIntType: {
                        switch(op){
                                case CNAddOperation: {
                                        opcode = CNAddUnsignedIntCode ;
                                } break ;
                                case CNSubOperation: {
                                        opcode = CNSubUnsignedIntCode ;
                                } break ;
                        }
                } break ;
                case CNSignedIntType: {
                        switch(op){
                                case CNAddOperation: {
                                        opcode = CNAddSignedIntCode ;
                                } break ;
                                case CNSubOperation: {
                                        opcode = CNSubSignedIntCode ;
                                } break ;
                        }
                } break ;
                case CNFloatType: {
                        switch(op){
                                case CNAddOperation: {
                                        opcode = CNAddFloatCode ;
                                } break ;
                                case CNSubOperation: {
                                        opcode = CNSubFloatCode ;
                                } break ;
                        }
                } break ;
                case CNNullType:
                case CNBooleanType:
                case CNStringType:
                case CNArrayType:
                case CNDictionaryType:
                case CNCodeType: {
                        return NULL ;
                } break ;
        }
        return CNAllocateCalcCodeValue(vpool, opcode, dstreg, src0reg, src1reg) ;
}

void
CNPrintByteCode(const struct CNCodeValue * src)
{
        struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(src->atttribute) ;

        uint32_t label ;
        if((label = attr.label) != CNNoLabel) {
                CNInterface()->printf("l%x:\t") ;
        } else {
                CNInterface()->printf("\t") ;
        }

        const char * opname = "?" ;
        unsigned int dstnum, srcnum ;
        switch((CNOpCode) attr.code){
                case CNNopCode: {
                        opname  = "nop" ;
                        dstnum  = 0 ;
                        srcnum  = 0 ;
                } break ;
                case CNStopCode: {
                        opname  = "stop" ;
                        dstnum  = 0 ;
                        srcnum  = 0 ;
                } break ;
                case CNMoveCode: {
                        opname  = "move" ;
                        dstnum  = 1 ;
                        srcnum  = 1 ;
                } break ;
                case CNLoadCode: {
                        opname  = "load" ;
                        dstnum  = 1 ;
                        srcnum  = 1 ;
                } break ;
                case CNConvU2ICode: {
                        opname  = "conv_u2i" ;
                        dstnum  = 1 ;
                        srcnum  = 1 ;
                } break ;
                case CNConvU2FCode: {
                        opname  = "conv_u2f" ;
                        dstnum  = 1 ;
                        srcnum  = 1 ;
                } break ;
                case CNConvI2UCode: {
                        opname  = "conv_i2u" ;
                        dstnum  = 1 ;
                        srcnum  = 1 ;
                } break ;
                case CNConvI2FCode: {
                        opname  = "conv_i2f" ;
                        dstnum  = 1 ;
                        srcnum  = 1 ;
                } break ;
                case CNConvF2ICode: {
                        opname  = "conv_f2i" ;
                        dstnum  = 1 ;
                        srcnum  = 1 ;
                } break ;
                case CNConvF2UCode: {
                        opname  = "conv_f2u" ;
                        dstnum  = 1 ;
                        srcnum  = 1 ;
                } break ;
                case CNLogicalOrCode: {
                        opname  = "logical_or" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNLogicalAndCode: {
                        opname  = "logical_and" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNBitAndCode: {
                        opname  = "bit_and" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNBitOrCode: {
                        opname  = "bit_or" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNBitXorCode: {
                        opname  = "bit_xor" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNBitShiftLeftCode: {
                        opname  = "bit_shift_left" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNBitShiftRightCode: {
                        opname  = "bit_shift_right" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNEqualBoolCode: {
                        opname  = "equal_bool" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNEqualSignedIntCode: {
                        opname  = "equal_int" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNEqualUnsignedIntCode: {
                        opname  = "equal_uint" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNEqualFloatCode: {
                        opname  = "equal_float" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNEqualStringCode: {
                        opname  = "equal_str" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNNotEqualBoolCode: {
                        opname  = "not_equal_bool" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNNotEqualSignedIntCode: {
                        opname  = "not_equal_int" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNNotEqualUnsignedIntCode: {
                        opname  = "not_equal_uint" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNNotEqualFloatCode: {
                        opname  = "not_equal_float" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNNotEqualStringCode: {
                        opname  = "not_equal_str" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNLessThanBoolCode: {
                        opname  = "less_than_bool" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNLessThanUnsignedIntCode: {
                        opname  = "less_than_uint" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNLessThanSignedIntCode: {
                        opname  = "less_than_int" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNLessThanFloatCode: {
                        opname  = "less_than_float" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNLessThanStringCode: {
                        opname  = "less_than_str" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNLessEqualBoolCode: {
                        opname  = "less_equal_bool" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNLessEqualUnsignedIntCode: {
                        opname  = "less_equal_uint" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNLessEqualSignedIntCode: {
                        opname  = "less_equal_int" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNLessEqualFloatCode: {
                        opname  = "less_equal_float" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNLessEqualStringCode: {
                        opname  = "less_equal_str" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNGreaterThanBoolCode: {
                        opname  = "greater_than_bool" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNGreaterThanUnsignedIntCode: {
                        opname  = "greater_than_uint" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNGreaterThanSignedIntCode: {
                        opname  = "greater_than_int" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNGreaterThanFloatCode: {
                        opname  = "greater_than_float" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNGreaterThanStringCode: {
                        opname  = "greater_than_str" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNGreateEqualBoolCode: {
                        opname  = "greate_equal_bool" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNGreateEqualUnsignedIntCode: {
                        opname  = "greate_equal_uint" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNGreateEqualSignedIntCode: {
                        opname  = "greate_equal_int" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNGreateEqualFloatCode: {
                        opname  = "greate_equal_float" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNGreateEqualStringCode: {
                        opname  = "greate_equal_str" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNAddUnsignedIntCode: {
                        opname  = "add_uint" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNAddSignedIntCode: {
                        opname  = "add_int" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNAddFloatCode: {
                        opname  = "add_float" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNAddStringCode: {
                        opname  = "add_str" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNSubUnsignedIntCode: {
                        opname  = "sub_uint" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNSubSignedIntCode: {
                        opname  = "sub_int" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNSubFloatCode: {
                        opname  = "sub_float" ;
                        dstnum  = 1 ;
                        srcnum  = 2 ;
                } break ;
                case CNPrintCode: {
                        opname  = "print" ;
                        dstnum  = 0 ;
                        srcnum  = 1 ;
                } break ;
        }
        CNInterface()->printf(opname) ;

        switch(attr.operand){
                case CNCalcOperandType: {
                        const struct CNCalcOperand * operand = &(src->calcOperand) ;
                        char delimiter = ' ' ;
                        if(dstnum > 0){
                                CNInterface()->printf(" r%lu", operand->destinationRegId) ;
                                delimiter = ',' ;
                        }
                        if(srcnum > 0){
                                CNInterface()->printf("%cr%lu", delimiter, operand->source0RegId) ;
                                delimiter = ',' ;
                        }
                        if(srcnum > 1){
                                CNInterface()->printf("%cr%lu", delimiter, operand->source1RegId) ;
                                delimiter = ',' ;
                        }
                } break ;
                case CNLoadOperandType: {
                        const struct CNLoadOperand * operand = &(src->loadOperand) ;
                        char delimiter = ' ' ;
                        if(dstnum > 0){
                                CNInterface()->printf(" r%lu", operand->destinationRegId) ;
                                delimiter = ',' ;
                        }
                        if(srcnum > 0){
                                CNInterface()->printf("%c", delimiter) ;
                                CNPrintValue(operand->sourceValue) ;
                        }
                } break ;
        }
}

void
CNDumpByteCodeInValueList(struct CNValueList * src)
{
        struct CNList * list = src->valueList ;
        for( ; list != NULL ; list = list->next){
                struct CNCodeValue * code = CNCastToCodeValue(list->data) ;
                if(code != NULL){
                        CNPrintByteCode(code) ;
                        CNInterface()->printf("\n") ;
                } else {
                        CNInterface()->printf("[Error] Invalid opcode\n") ;
                }
        }
}

void
CNDumpByteCodeInArrayValue(struct CNArrayValue * src)
{
        unsigned int elmnum = CNNumberOfValuesInArrayValue(src) ;
        for(unsigned int i=0 ; i<elmnum ; i++){
                struct CNCodeValue * code = CNCastToCodeValue(CNValueInArray(src, i)) ;
                if(code != NULL){
                        CNPrintByteCode(code) ;
                        CNInterface()->printf("\n") ;
                } else {
                        CNInterface()->printf("[Error] Invalid opcode\n") ;
                }
        }
}
