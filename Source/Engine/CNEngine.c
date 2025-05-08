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
        struct CNValuePool * vpool     = program->valuePool ;

        struct CNList * curlist = codes->firstItem ;
        while(curlist != NULL){
                struct CNValue  * curval  = curlist->data ;
                struct CNOpCode * curcode = &(curval->opCodeValue) ;
                struct CNList   * next    = NULL ;
                switch((CNByteCode) curcode->code){
                        case CNMoveByteCode: {
                                const struct CNExecOperands * operands = &(curcode->execOperands) ;
                                uint64_t dstreg = operands->destinationRegId ;
                                uint64_t srcreg = operands->source0RegId ;
                                struct CNValue * srcval = CNValueInRegisters(registers, srcreg) ;
                                CNSetValueToRegisters(registers, dstreg, srcval) ;
                                CNReleaseValue(vpool, srcval) ;
                        } break ;
                        case CNStoreByteCode: {
                                const struct CNStorageOperands * operands = &(curcode->storageOperands) ;
                                uint64_t dstreg = operands->destinationRegId ;
                                struct CNValue * srcval = operands->sourceValue ;
                                CNSetValueToRegisters(registers, dstreg, srcval) ;
                                CNReleaseValue(vpool, srcval) ;
                        } break ;
                        case CNPrintByteCode: {
                                const struct CNExecOperands * operands = &(curcode->execOperands) ;
                                uint64_t srcreg = operands->source0RegId ;
                                struct CNValue * srcval = CNValueInRegisters(registers, srcreg) ;
                                CNPrintValue(srcval) ;
                        } break ;
                }
                curlist = next != NULL ? next : curlist->next ;
        }
}
