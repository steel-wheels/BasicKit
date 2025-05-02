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
                case CNStoreStringByteCode:     result = "store.s" ;    break ;
                case CNPrintByteCode:           result = "print" ;      break ;
        }
        return result ;
}

static void
dumpRegister(const struct CNValue * regid)
{
        uint64_t regnum = CNUnsignedIntValue(regid) ;
        CNInterface()->printf("$%lu ", regnum) ;
}

void
CNDumpByteCode(uint32_t indent, const struct CNValue * src)
{
        CNByteCode bcode ;
        if(!CNByteCodeInValue(&bcode, src)){
                CNInterface()->error("[Error] This is not byte code\n") ;
                return ;
        }

        /* Dump opcode */
        const char * opstr = opCodeName(bcode) ;
        CNDumpIndent(indent) ;
        CNInterface()->printf("%s ", opstr) ;

        /* dump parameters */
        const struct CNOpCode * opcode = &(src->opCodeValue) ;
        switch(bcode){
                case CNStoreStringByteCode: {
                        /* destination register */
                        dumpRegister(opcode->destination) ;
                        /* dump source string */
                        CNDumpValue(0, opcode->source0) ;
                } break ;
                case CNPrintByteCode: {
                        /* dump source register */
                        dumpRegister(opcode->source0) ;
                        CNInterface()->printf("\n") ;
                } break ;
        }
}
