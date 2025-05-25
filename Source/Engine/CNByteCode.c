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
