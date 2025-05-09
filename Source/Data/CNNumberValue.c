/*
 * @file CNNumberValue.c
 * @description Define some number value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNNumberValue.h"
#include "CNInterface.h"

static void releasetSignedIntValue(struct CNValue * val) ;
static void printSignedIntValue(struct CNValue * val) ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForSignedIntValue(void)
{
        static struct CNVirtualValueFunctions s_virt_func ;
        static bool s_initialized = false ;
        if(!s_initialized){
                s_virt_func.releaseContents     = &releasetSignedIntValue ;
                s_virt_func._print              = &printSignedIntValue ;
                s_initialized = true ;
        }
        return &s_virt_func ;
}

static void
releasetSignedIntValue(struct CNValue * val)
{
        (void) val ; // do nothing'
}

static void
printSignedIntValue(struct CNValue * src)
{
        struct CNSignedIntValue * ival = CNCasToSignedIntValue(src) ;
        if(ival != NULL){
                CNInterface()->printf("%ld", ival->value) ;
        } else {
                CNInterface()->printf("[error at %s]", __func__) ;
        }
}

