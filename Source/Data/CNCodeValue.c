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
        newval->atttribute  = CNCodeValueAttributeToInt(&attr) ;
        newval->calcOperand = operand ;
        return newval ;
}

struct CNCodeValue *
CNAllocateLoadCodeValue(struct CNValuePool * vpool, uint32_t code,
                        uint64_t dstregid, struct CNValue * srcval)
{
        CNRetainValue(srcval) ;

        struct CNCodeValueAttribute attr = {
                .code           = code,
                .operand        = CNCalcOperandType
        } ;

        struct CNLoadOperand operand = {
                .destinationRegId       = dstregid,
                .sourceValue            = srcval
        } ;

        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForCodeValue() ;
        struct CNCodeValue * newval ;
        newval = (struct CNCodeValue *) CNAllocateValue(vpool, CNCodeType, vfunc) ;
        newval->atttribute  = CNCodeValueAttributeToInt(&attr) ;
        newval->loadOperand = operand ;
        return newval ;
}

static void
releaseContents(struct CNValuePool * vpool, struct CNValue * val)
{
        struct CNCodeValue * codevalue = CNCastToCodeValue(val) ;
        struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(codevalue->atttribute) ;
        switch(attr.operand){
                case CNCalcOperandType: {
                        /* nothing have to do */
                } break ;
                case CNLoadOperandType: {
                        CNReleaseValue(vpool, (codevalue->loadOperand).sourceValue) ;
                } break ;
        }
}

static void
printValues(struct CNValue * val)
{
        struct CNCodeValue * codevalue = CNCastToCodeValue(val) ;
        struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(codevalue->atttribute) ;

        CNInterface()->printf("{code:0x%lx ", attr.code) ;
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
        }
        CNInterface()->printf("}") ;
}
