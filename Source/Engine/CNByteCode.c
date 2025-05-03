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
                case CNStoreByteCode:   result = "store" ;      break ;
                case CNPrintByteCode:   result = "print" ;      break ;
        }
        return result ;
}

static void
printRegister(const struct CNValue * regid)
{
        uint64_t regnum = CNUnsignedIntValue(regid) ;
        CNInterface()->printf("$%lu ", regnum) ;
}

void
_CNPrintByteCode(const struct CNValue * src)
{
        CNByteCode bcode ;
        if(!CNByteCodeInValue(&bcode, src)){
                CNInterface()->error("[Error] This is not byte code\n") ;
                return ;
        }

        /* Dump opcode */
        const char * opstr = opCodeName(bcode) ;
        CNInterface()->printf("%s ", opstr) ;

        /* dump parameters */
        const struct CNOpCode * opcode = &(src->opCodeValue) ;
        switch(bcode){
                case CNStoreByteCode: {
                        /* destination register */
                        printRegister(opcode->destination) ;
                        /* dump source string */
                        CNInterface()->printf("\"") ;
                        CNPrintValue(opcode->source0) ;
                        CNInterface()->printf("\"") ;
                } break ;
                case CNPrintByteCode: {
                        /* dump source register */
                        printRegister(opcode->source0) ;
                } break ;
        }
}
