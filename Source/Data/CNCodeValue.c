/*
 * @file CNCodeValue.c
 * @description Define opcode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNCodeValue.h"
#include "CNInterface.h"

static void releaseContents(struct CNValuePool * vpool, struct CNValue * val) ;
static void printValues(struct CNValue * val) ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForCodeValue(void)
{
        static struct CNVirtualValueFunctions s_virt_func ;
        static bool s_initialized = false ;
        if(!s_initialized){
                s_virt_func.releaseContents     = &releaseContents ;
                s_virt_func.print               = &printValues ;
                s_initialized = true ;
        }
        return &s_virt_func ;
}

struct CNCodeValue *
CNAllocateCalcCodeValue(struct CNValuePool * vpool, uint32_t code,
                        uint64_t dstregid, uint64_t src0regid, uint64_t src1regid)
{
        struct CNCodeValueAttribute attr = {
                .label          = CNNoLabel,
                .code           = code,
                .operand        = CNCalcOperandType
        } ;

        struct CNCalcOperand operand = {
                .destinationRegId       = dstregid,
                .source0RegId           = src0regid,
                .source1RegId           = src1regid
        } ;

        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForCodeValue() ;
        struct CNCodeValue * newval ;
        newval = (struct CNCodeValue *) CNAllocateValue(vpool, CNCodeType, vfunc) ;
        newval->codeAttribute  = CNCodeValueAttributeToInt(&attr) ;
        newval->calcOperand = operand ;
        return newval ;
}

struct CNCodeValue *
CNAllocateLoadCodeValue(struct CNValuePool * vpool, uint32_t code,
                        uint64_t dstregid, struct CNValue * srcval)
{
        CNRetainValue(srcval) ;

        struct CNCodeValueAttribute attr = {
                .label          = CNNoLabel,
                .code           = code,
                .operand        = CNLoadOperandType
        } ;

        struct CNLoadOperand operand = {
                .destinationRegId       = dstregid,
                .sourceValue            = srcval
        } ;

        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForCodeValue() ;
        struct CNCodeValue * newval ;
        newval = (struct CNCodeValue *) CNAllocateValue(vpool, CNCodeType, vfunc) ;
        newval->codeAttribute  = CNCodeValueAttributeToInt(&attr) ;
        newval->loadOperand = operand ;
        return newval ;
}

struct CNCodeValue *
CNAllocateBranchCodeValue(struct CNValuePool * vpool, uint32_t code,
                          uint64_t condregid, uint32_t label)
{
        struct CNCodeValueAttribute attr = {
                .label          = CNNoLabel,
                .code           = code,
                .operand        = CNBranchOperandType
        } ;

        struct CNBranchOperand operand = {
                .conditionRegId = condregid,
                .targetLabel    = label,
                .targetOffset   = 0
        } ;

        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForCodeValue() ;
        struct CNCodeValue * newval ;
        newval = (struct CNCodeValue *) CNAllocateValue(vpool, CNCodeType, vfunc) ;
        newval->codeAttribute     = CNCodeValueAttributeToInt(&attr) ;
        newval->branchOperand = operand ;
        return newval ;
}

static void
releaseContents(struct CNValuePool * vpool, struct CNValue * val)
{
        struct CNCodeValue * codevalue = CNCastToCodeValue(val) ;
        struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(codevalue->codeAttribute) ;
        switch(attr.operand){
                case CNCalcOperandType:
                case CNBranchOperandType: {
                        /* nothing have to do */
                } break ;
                case CNLoadOperandType: {
                        CNReleaseValue(vpool, (codevalue->loadOperand).sourceValue) ;
                } break ;
        }
}

void
CNSetTargetLabelToBrachCodeValue(struct CNCodeValue * dst, uint32_t label)
{
        struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(dst->codeAttribute) ;
        switch(attr.operand){
                case CNCalcOperandType:
                case CNLoadOperandType: {
                        CNInterface()->error("[Error] Unexpected code type at %s\n", __func__) ;
                } break ;
                case CNBranchOperandType: {
                        struct CNBranchOperand * bop = &(dst->branchOperand) ;
                        bop->targetLabel = label ;
                } break ;
        }
}

static void
printValues(struct CNValue * val)
{
        struct CNCodeValue * codevalue = CNCastToCodeValue(val) ;
        struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(codevalue->codeAttribute) ;

        CNInterface()->printf("{code:0x%lx label:%u ", attr.code, attr.label) ;
        switch(attr.operand){
                case CNCalcOperandType: {
                        struct CNCalcOperand * cop = &(codevalue->calcOperand) ;
                        CNInterface()->printf("dst:%lx ", cop->destinationRegId) ;
                        CNInterface()->printf("src0:%lx ", cop->source0RegId) ;
                        CNInterface()->printf("src1:%lx", cop->source0RegId) ;
                } break ;
                case CNLoadOperandType: {
                        struct CNLoadOperand * lop = &(codevalue->loadOperand) ;
                        CNInterface()->printf("dst:%lx ", lop->destinationRegId) ;
                        CNInterface()->printf("src:") ;
                        CNPrintValue(lop->sourceValue) ;
                } break ;
                case CNBranchOperandType: {
                        struct CNBranchOperand * bop = &(codevalue->branchOperand) ;
                        CNInterface()->printf("cond:%lx ", bop->conditionRegId) ;
                        CNInterface()->printf("target-label:%lu ", bop->targetLabel) ;
                        CNInterface()->printf("target-offset:%ld", bop->targetOffset) ;
                } break ;
        }
        CNInterface()->printf("}") ;
}
