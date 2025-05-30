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
        /*
         CNNullType              = 0,
         CNBooleanType,
         CNSignedIntType,
         CNUnsignedIntType,
         CNFloatType,
         CNStringType,
         CNArrayType,
         CNDictionaryType,
         CNCodeType
         */

        struct CNCodeValue * result = NULL ;


        return result ;
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
                case CNBitOrCode: {
                        opname  = "bit_or" ;
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
