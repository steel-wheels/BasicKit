/*
 * @file CNNullValue.c
 * @description Define null value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNNullValue.h"
#include "CNInterface.h"

static void releaseContents(struct CNValue * val) ;
static void printNullValue(struct CNValue * val) ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForNullValue(void)
{
        static struct CNVirtualValueFunctions s_virt_func ;
        static bool s_initialized = false ;
        if(!s_initialized){
                s_virt_func.releaseContents     = &releaseContents ;
                s_virt_func.print               = &printNullValue ;
                s_initialized = true ;
        }
        return &s_virt_func ;
}

struct CNNullValue *
CNAllocateNullValue(void)
{
        static struct CNNullValue s_null_value ;
        static bool   s_initialized = false ;
        if(!s_initialized){
                struct CNValueAttribute attr = {
                        .isFixed        = true,
                        .type           = CNNullType,
                        .referenceCount = 1
                } ;
                s_null_value.superClass.attribute = CNValueAttributeToInt(&attr) ;
                s_null_value.superClass.virtualFunctions = CNVirtualFunctionsForNullValue() ;
                s_initialized = true ;
        }

        return &s_null_value ;
}

static void
releaseContents(struct CNValue * val)
{
        (void) val ; // nothing have to do
}

static void printNullValue(struct CNValue * src)
{
        struct CNNullValue * nval = CNCastToNullValue(src) ;
        if(nval != NULL){
                CNInterface()->printf("null") ;
        } else {
                CNInterface()->printf("[error at %s]", __func__) ;
        }
}
