/*
 * @file CNByteCode.c
 * @description Define CNByteCode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNByteCode.h>
#import <BasicKit/CNInterface.h>

void
CNPrintByteCode(const struct CNCodeValue * src)
{
        struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(src->atttribute) ;

        const char * opname = "?" ;
        unsigned int dstnum, srcnum ;
        switch((CNOpCode) attr.code){
                case CNNopCode: {
                        opname  = "nop" ;
                        dstnum  = 0 ;
                        srcnum  = 0 ;
                } break ;
                case CNLoadCode: {
                        opname  = "load" ;
                        dstnum  = 1 ;
                        srcnum  = 1 ;
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
