/*
 * @file CNEngine.c
 * @description Define CNEngine data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNEngine.h>
#import <BasicKit/CNByteCode.h>

void
CNExecuteProgram(struct CNProgram * program)
{
        struct CNValueList * codes     = &(program->program) ;
        struct CNRegisters * registers = &(program->registers) ;

        struct CNList * curlist = codes->firstItem ;
        while(curlist != NULL){
                struct CNValue  * curval  = curlist->data ;
                struct CNOpCode * curcode = &(curval->opCodeValue) ;
                struct CNList   * next    = NULL ;
                switch(CNByteCodeInAttribute(curcode->attribute)){
                        case CNMoveByteCode: {
                                uint64_t  dstregid = CNUnsignedIntValue(curcode->destination) ;
                                uint64_t  srcregid = CNUnsignedIntValue(curcode->source0) ;
                                struct CNValue * srcval   = CNValueInRegisters(registers, srcregid) ;
                                CNSetValueToRegisters(registers, dstregid, srcval) ;
                        } break ;
                        case CNStoreByteCode: {
                                uint64_t dstregid = CNUnsignedIntValue(curcode->destination) ;
                                struct CNValue * srcval = curcode->source0 ;
                                CNSetValueToRegisters(registers, dstregid, srcval) ;
                        } break ;
                        case CNPrintByteCode: {
                                uint64_t srcregid = CNUnsignedIntValue(curcode->source0) ;
                                struct CNValue * srcval = CNValueInRegisters(registers, srcregid) ;
                                CNPrintValue(srcval) ;
                        } break ;
                }
                curlist = next != NULL ? next : curlist->next ;
        }
}
