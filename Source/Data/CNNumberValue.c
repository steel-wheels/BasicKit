/*
 * @file CNNumberValue.c
 * @description Define some number value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNNumberValue.h"
#include "CNInterface.h"

static void releaseContents(struct CNValuePool * vpool, struct CNValue * val) ;
static void printSignedIntValue(struct CNValue * val) ;
static void printUnsignedIntValue(struct CNValue * val) ;
static void printFloatValue(struct CNValue * val) ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForSignedIntValue(void)
{
        static struct CNVirtualValueFunctions s_virt_func ;
        static bool s_initialized = false ;
        if(!s_initialized){
                s_virt_func.releaseContents     = &releaseContents ;
                s_virt_func.print               = &printSignedIntValue ;
                s_initialized = true ;
        }
        return &s_virt_func ;
}

struct CNVirtualValueFunctions *
CNVirtualFunctionsForUnignedIntValue(void)
{
        static struct CNVirtualValueFunctions s_virt_func ;
        static bool s_initialized = false ;
        if(!s_initialized){
                s_virt_func.releaseContents    = &releaseContents ;
                s_virt_func.print              = &printUnsignedIntValue ;
                s_initialized = true ;
        }
        return &s_virt_func ;
}

struct CNVirtualValueFunctions *
CNVirtualFunctionsForFloatValue(void)
{
        static struct CNVirtualValueFunctions s_virt_func ;
        static bool s_initialized = false ;
        if(!s_initialized){
                s_virt_func.releaseContents    = &releaseContents ;
                s_virt_func.print              = &printFloatValue ;
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
printSignedIntValue(struct CNValue * src)
{
        struct CNSignedIntValue * ival = CNCastToSignedIntValue(src) ;
        if(ival != NULL){
                CNInterface()->printf("%ld", ival->value) ;
        } else {
                CNInterface()->printf("[error at %s]", __func__) ;
        }
}

static void
printUnsignedIntValue(struct CNValue * src)
{
        struct CNUnsignedIntValue * ival = CNCastToUnsignedIntValue(src) ;
        if(ival != NULL){
                CNInterface()->printf("%lu", ival->value) ;
        } else {
                CNInterface()->printf("[error at %s]", __func__) ;
        }
}

static void
printFloatValue(struct CNValue * src)
{
        struct CNFloatValue * ival = CNCastToFloatValue(src) ;
        if(ival != NULL){
                CNInterface()->printf("%lf", ival->value) ;
        } else {
                CNInterface()->printf("[error at %s]", __func__) ;
        }
}
