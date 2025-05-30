/*
 * @file CNByteExec.c
 * @description Define CNByteCode executer
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNByteExec.h"
#include "CNBooleanValue.h"
#include "CNNumberValue.h"

void
CNExecuteByteCode(struct CNArrayValue * codes, struct CNRegisterFile * regfile, index_t startidx)
{
        index_t                 curidx = startidx ;
        bool                    dostop = false ;
        while(!dostop){
                struct CNCodeValue * code = CNCastToCodeValue(CNValueInArray(codes, curidx)) ;
                struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(code->atttribute) ;
                struct CNValuePool * vpool = regfile->valuePool ;

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
                        case CNConvU2ICode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t srcid = (index_t) operand->source0RegId ;
                                index_t dstid = (index_t) operand->destinationRegId ;
                                struct CNUnsignedIntValue * srcval = CNCastToUnsignedIntValue(CNValueInRegisterFile(regfile, srcid)) ;
                                struct CNSignedIntValue *   dstval = CNAllocateSignedIntValue(vpool, (int64_t) srcval->value) ;
                                CNSetValueToRegisterFile(regfile, dstid, CNSuperClassOfSignedIntValue(dstval)) ;
                        } break ;
                        case CNConvU2FCode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t srcid = (index_t) operand->source0RegId ;
                                index_t dstid = (index_t) operand->destinationRegId ;
                                struct CNUnsignedIntValue * srcval = CNCastToUnsignedIntValue(CNValueInRegisterFile(regfile, srcid)) ;
                                struct CNFloatValue *   dstval = CNAllocateFloatValue(vpool, (double) srcval->value) ;
                                CNSetValueToRegisterFile(regfile, dstid, CNSuperClassOfFloatValue(dstval)) ;
                        } break ;
                        case CNConvI2UCode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t srcid = (index_t) operand->source0RegId ;
                                index_t dstid = (index_t) operand->destinationRegId ;
                                struct CNSignedIntValue * srcval = CNCastToSignedIntValue(CNValueInRegisterFile(regfile, srcid)) ;
                                struct CNUnsignedIntValue * dstval = CNAllocateUnsignedIntValue(vpool, (uint64_t) srcval->value) ;
                                CNSetValueToRegisterFile(regfile, dstid, CNSuperClassOfUnsignedIntValue(dstval)) ;
                        } break ;
                        case CNConvI2FCode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t srcid = (index_t) operand->source0RegId ;
                                index_t dstid = (index_t) operand->destinationRegId ;
                                struct CNSignedIntValue * srcval = CNCastToSignedIntValue(CNValueInRegisterFile(regfile, srcid)) ;
                                struct CNFloatValue *   dstval = CNAllocateFloatValue(vpool, (double) srcval->value) ;
                                CNSetValueToRegisterFile(regfile, dstid, CNSuperClassOfFloatValue(dstval)) ;
                        } break ;
                        case CNConvF2ICode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t srcid = (index_t) operand->source0RegId ;
                                index_t dstid = (index_t) operand->destinationRegId ;
                                struct CNFloatValue * srcval = CNCastToFloatValue(CNValueInRegisterFile(regfile, srcid)) ;
                                struct CNSignedIntValue * dstval = CNAllocateSignedIntValue(vpool, (int64_t) srcval->value) ;
                                CNSetValueToRegisterFile(regfile, dstid, CNSuperClassOfSignedIntValue(dstval)) ;
                        } break ;
                        case CNConvF2UCode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t srcid = (index_t) operand->source0RegId ;
                                index_t dstid = (index_t) operand->destinationRegId ;
                                struct CNFloatValue * srcval = CNCastToFloatValue(CNValueInRegisterFile(regfile, srcid)) ;
                                struct CNUnsignedIntValue *   dstval = CNAllocateUnsignedIntValue(vpool, (uint64_t) srcval->value) ;
                                CNSetValueToRegisterFile(regfile, dstid, CNSuperClassOfUnsignedIntValue(dstval)) ;
                        } break ;
                        case CNLoadCode: {
                                const struct CNLoadOperand * operand = &(code->loadOperand) ;
                                index_t          dstregid = (index_t) operand->destinationRegId ;
                                struct CNValue * srcval   = operand->sourceValue ;
                                CNSetValueToRegisterFile(regfile, dstregid, srcval) ;
                        } break ;
                        case CNLogicalOrCode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t dstid  = (index_t) operand->destinationRegId ;
                                index_t src0id = (index_t) operand->source0RegId ;
                                index_t src1id = (index_t) operand->source1RegId ;
                                struct CNBooleanValue * src0val = CNCastToBooleanValue(CNValueInRegisterFile(regfile, src0id)) ;
                                struct CNBooleanValue * src1val = CNCastToBooleanValue(CNValueInRegisterFile(regfile, src1id)) ;
                                bool result = src0val->value || src1val->value ;
                                struct CNBooleanValue * dstval  = CNAllocateBooleanValue(vpool, result) ;
                                CNSetValueToRegisterFile(regfile, dstid, CNSuperClassOfBooleanValue(dstval)) ;
                                CNReleaseValue(vpool, CNSuperClassOfBooleanValue(dstval)) ;
                        } break ;
                        case CNLogicalAndCode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t dstid  = (index_t) operand->destinationRegId ;
                                index_t src0id = (index_t) operand->source0RegId ;
                                index_t src1id = (index_t) operand->source1RegId ;
                                struct CNBooleanValue * src0val = CNCastToBooleanValue(CNValueInRegisterFile(regfile, src0id)) ;
                                struct CNBooleanValue * src1val = CNCastToBooleanValue(CNValueInRegisterFile(regfile, src1id)) ;
                                bool result = src0val->value && src1val->value ;
                                struct CNBooleanValue * dstval  = CNAllocateBooleanValue(vpool, result) ;
                                CNSetValueToRegisterFile(regfile, dstid, CNSuperClassOfBooleanValue(dstval)) ;
                                CNReleaseValue(vpool, CNSuperClassOfBooleanValue(dstval)) ;
                        } break ;
                        case CNBitAndCode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t dstid  = (index_t) operand->destinationRegId ;
                                index_t src0id = (index_t) operand->source0RegId ;
                                index_t src1id = (index_t) operand->source1RegId ;
                                struct CNUnsignedIntValue * src0val = CNCastToUnsignedIntValue(CNValueInRegisterFile(regfile, src0id)) ;
                                struct CNUnsignedIntValue * src1val = CNCastToUnsignedIntValue(CNValueInRegisterFile(regfile, src1id)) ;
                                uint64_t result = src0val->value & src1val->value ;
                                struct CNUnsignedIntValue * dstval  = CNAllocateUnsignedIntValue(vpool, result) ;
                                CNSetValueToRegisterFile(regfile, dstid, CNSuperClassOfUnsignedIntValue(dstval)) ;
                                CNReleaseValue(vpool, CNSuperClassOfUnsignedIntValue(dstval)) ;
                        } break ;
                        case CNBitOrCode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t dstid  = (index_t) operand->destinationRegId ;
                                index_t src0id = (index_t) operand->source0RegId ;
                                index_t src1id = (index_t) operand->source1RegId ;
                                struct CNUnsignedIntValue * src0val = CNCastToUnsignedIntValue(CNValueInRegisterFile(regfile, src0id)) ;
                                struct CNUnsignedIntValue * src1val = CNCastToUnsignedIntValue(CNValueInRegisterFile(regfile, src1id)) ;
                                uint64_t result = src0val->value | src1val->value ;
                                struct CNUnsignedIntValue * dstval  = CNAllocateUnsignedIntValue(vpool, result) ;
                                CNSetValueToRegisterFile(regfile, dstid, CNSuperClassOfUnsignedIntValue(dstval)) ;
                                CNReleaseValue(vpool, CNSuperClassOfUnsignedIntValue(dstval)) ;
                        } break ;
                        case CNBitXorCode: {
                                const struct CNCalcOperand * operand = &(code->calcOperand) ;
                                index_t dstid  = (index_t) operand->destinationRegId ;
                                index_t src0id = (index_t) operand->source0RegId ;
                                index_t src1id = (index_t) operand->source1RegId ;
                                struct CNUnsignedIntValue * src0val = CNCastToUnsignedIntValue(CNValueInRegisterFile(regfile, src0id)) ;
                                struct CNUnsignedIntValue * src1val = CNCastToUnsignedIntValue(CNValueInRegisterFile(regfile, src1id)) ;
                                uint64_t result = src0val->value ^ src1val->value ;
                                struct CNUnsignedIntValue * dstval  = CNAllocateUnsignedIntValue(vpool, result) ;
                                CNSetValueToRegisterFile(regfile, dstid, CNSuperClassOfUnsignedIntValue(dstval)) ;
                                CNReleaseValue(vpool, CNSuperClassOfUnsignedIntValue(dstval)) ;
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

