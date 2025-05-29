/*
 * @file CNBooleanValue.c
 * @description Define boolean value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNBooleanValue.h"
#include "CNInterface.h"

static void releaseContents(struct CNValuePool * vpool, struct CNValue * val) ;
static void printBooleanValue(struct CNValue * val) ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForBooleanValue(void)
{
        static struct CNVirtualValueFunctions s_virt_func ;
        static bool s_initialized = false ;
        if(!s_initialized){
                s_virt_func.releaseContents     = &releaseContents ;
                s_virt_func.print               = &printBooleanValue ;
                s_initialized = true ;
        }
        return &s_virt_func ;
}

static void
releaseContents(struct CNValuePool * vpool, struct CNValue * val)
{
        (void) vpool ; (void) val ; // do nothing'
}

static void
printBooleanValue(struct CNValue * src)
{
        struct CNBooleanValue * ival = CNCastToBooleanValue(src) ;
        if(ival != NULL){
                CNInterface()->printf("%s", ival->value ? "true" : "false") ;
        } else {
                CNInterface()->printf("[error at %s]", __func__) ;
        }
}

