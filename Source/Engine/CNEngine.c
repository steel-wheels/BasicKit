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
                        case CNStoreByteCode: {
                                uint64_t dstid = CNUnsignedIntValue(curcode->destination) ;
                                struct CNValue * srcval = curcode->source0 ;
                                CNSetValueToRegisters(registers, dstid, srcval) ;
                        } break ;
                        case CNPrintByteCode: {
                                uint64_t regid = CNUnsignedIntValue(curcode->source0) ;
                                struct CNValue * regval = CNValueInRegisters(registers, regid) ;
                                CNPrintValue(regval) ;
                        } break ;
                }
                curlist = next != NULL ? next : curlist->next ;
        }
}
