/*
 * @file CNByteExec.c
 * @description Define CNByteCode executer
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNByteExec.h"

void
CNExecuteByteCode(struct CNArrayValue * codes, struct CNRegisterFile * regfile, index_t startidx)
{
        index_t                 curidx = startidx ;
        bool                    dostop = false ;
        while(!dostop){
                struct CNCodeValue * code = CNCastToCodeValue(CNValueInArray(codes, curidx)) ;
                struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(code->atttribute) ;

                index_t nextidx = curidx + 1 ;
                switch((CNOpCode) attr.code){
                        case CNNopCode: {
                                /* do nothing */
                        } break ;
                        case CNStopCode: {
                                /* breakout from this loop */
                                dostop = true ;
                        } break ;
                        case CNMoveCode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t srcid = (index_t) operand->source0RegId ;
                                index_t dstid = (index_t) operand->destinationRegId ;
                                struct CNValue * srcval = CNValueInRegisterFile(regfile, srcid) ;
                                if(srcval != NULL){
                                        CNSetValueToRegisterFile(regfile, dstid, srcval) ;
                                } else {
                                        CNInterface()->printf("[Error] No source value") ;
                                }
                        } break ;
                        case CNLoadCode: {
                                const struct CNLoadOperand * operand = &(code->loadOperand) ;
                                index_t          dstregid = (index_t) operand->destinationRegId ;
                                struct CNValue * srcval   = operand->sourceValue ;
                                CNSetValueToRegisterFile(regfile, dstregid, srcval) ;
                        } break ;
                        case CNPrintCode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t srcregid = (index_t) operand->source0RegId ;
                                struct CNValue * val ;
                                if((val = CNValueInRegisterFile(regfile, srcregid)) != NULL){
                                        CNPrintValue(val) ;
                                        CNInterface()->printf("\n") ;
                                } else {
                                        CNInterface()->printf("[Error] Undefined value") ;
                                }
                        } break ;
                }

                /* update program counter */
                curidx = nextidx ;
        }
}

