/*
 * @file CNByteCode.c
 * @description Define CNByteCode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNByteCode.h>
#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNInterface.h>
#import <BasicKit/CNUtils.h>

static const char *
opCodeName(CNByteCode code)
{
        const char * result = "?" ;
        switch(code){
                case CNMoveByteCode:    result = "move" ;       break ;
                case CNStoreByteCode:   result = "store" ;      break ;
                case CNPrintByteCode:   result = "print" ;      break ;
        }
        return result ;
}

static inline void
printRegister(uint64_t regid)
{
        CNInterface()->printf("$%lu ", regid) ;
}

void
_CNPrintByteCode(const struct CNValue * src)
{
        /* Dump opcode */
        CNByteCode bcode = (src->opCodeValue).code ;
        const char * opstr = opCodeName(bcode) ;
        CNInterface()->printf("%s ", opstr) ;

        /* dump parameters */
        const struct CNOpCode * opcode = &(src->opCodeValue) ;
        switch(bcode){
                case CNMoveByteCode: {
                        const struct CNExecOperands * operands = &(opcode->execOperands) ;
                        printRegister(operands->destinationRegId) ;
                        printRegister(operands->source0RegId) ;
                } break ;
                case CNStoreByteCode: {
                        const struct CNStorageOperands * operands = &(opcode->storageOperands) ;
                        printRegister(operands->destinationRegId) ;
                        CNInterface()->printf("\"") ;
                        CNPrintValue(operands->sourceValue) ;
                        CNInterface()->printf("\"") ;
                } break ;
                case CNPrintByteCode: {
                        const struct CNExecOperands * operands = &(opcode->execOperands) ;
                        printRegister(operands->source0RegId) ;
                } break ;
        }
}
