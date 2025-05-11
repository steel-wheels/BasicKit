/*
 * @file CNArrayValue.c
 * @description Define array value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNArrayValue.h"
#include "CNValuePool.h"
#include "CNValue.h"
#include "CNNullValue.h"
#include "CNInterface.h"

static void releaseContents(struct CNValuePool * vpool, struct CNValue * val) ;
static void printValues(struct CNValue * val) ;

static unsigned int
pageElementNumInPage(unsigned int elmnum)
{
        static const unsigned int UNIT_ELM_NUM = 128 ;
        unsigned int unitnum = (elmnum + UNIT_ELM_NUM - 1) / UNIT_ELM_NUM ;
        return unitnum * UNIT_ELM_NUM ;
}

struct CNVirtualValueFunctions *
CNVirtualFunctionsForArrayValue(void)
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

struct CNArrayValue *
CNAllocateArrayValue(struct CNValuePool * vpool, unsigned int elmnum)
{
        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForArrayValue() ;
        struct CNArrayValue * newval ;
        newval = (struct CNArrayValue *) CNAllocateValue(vpool, CNArrayType, vfunc) ;
        newval->elementNum      = elmnum ;

        unsigned int pagenum = pageElementNumInPage(elmnum) ;
        struct CNValue ** elements ;
        elements = (struct CNValue **) CNAllocateArrayData(&(vpool->arrayPool),
                        sizeof(struct CNValue *) * pagenum) ;

        struct CNValue ** ptr    = elements ;
        struct CNValue ** endptr = ptr + elmnum ;
        for( ; ptr < endptr ; ptr++){
                *ptr = CNSuperClassOfNullValue(CNAllocateNullValue()) ;
        }
        newval->values = elements ;
        return newval ;
}

struct CNValue *
CNValueInArray(struct CNArrayValue * array, unsigned int index)
{
        if(index < array->elementNum) {
                return array->values[index] ;
        } else {
                CNInterface()->printf("[Error] Index overflow at %s\n", __func__) ;
                return CNSuperClassOfNullValue(CNAllocateNullValue()) ;
        }
}

bool
CNSetValueToArray(struct CNValuePool * vpool, struct CNArrayValue * array, unsigned int index, struct CNValue * src)
{
        if(index < array->elementNum) {
                CNRetainValue(src) ;
                CNReleaseValue(vpool, array->values[index]) ;
                array->values[index] = src ;
                return true ;
        } else {
                CNInterface()->printf("[Error] Index overflow at %s\n", __func__) ;
                return false ;
        }
}

static void
releaseContents(struct CNValuePool * vpool, struct CNValue * val)
{
        struct CNArrayValue * arr = CNCastToArrayValue(val) ;
        struct CNValue ** ptr    = arr->values ;
        struct CNValue ** endptr = ptr + arr->elementNum ;
        for( ; ptr < endptr ; ptr++){
                CNReleaseValue(vpool, *ptr) ;
        }
        unsigned int pagenum = pageElementNumInPage((unsigned int) arr->elementNum) ;
        CNFreeArrayData(&(vpool->arrayPool), sizeof(struct CNValue *) * pagenum, arr->values) ;
        arr->elementNum = 0 ;
        arr->values     = NULL ;
}

static void
printValues(struct CNValue * val)
{
        struct CNArrayValue * arr = CNCastToArrayValue(val) ;
        struct CNValue ** ptr    = arr->values ;
        struct CNValue ** endptr = ptr + arr->elementNum ;
        if(ptr < endptr){
                CNInterface()->printf("[") ;
                CNPrintValue(*ptr) ; ptr++ ;
                for( ; ptr < endptr ; ptr++){
                        CNInterface()->printf(", ") ;
                        CNPrintValue(*ptr) ;
                }
                CNInterface()->printf("]") ;
        }
}
