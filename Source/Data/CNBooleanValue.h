/*
 * @file CNBooleanValue.h
 * @description Define boolean value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNBooleanValue_h
#define CNBooleanValue_h

#import <BasicKit/CNValue.h>

struct CNBooleanValue {
        struct CNValue          superClass ;
        bool                    value ;
} ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForBooleanValue(void) ;

static inline struct CNBooleanValue *
CNAllocateBooleanValue(struct CNValuePool * vpool, bool value)
{
        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForBooleanValue() ;
        struct CNBooleanValue * newval ;
        newval = (struct CNBooleanValue *) CNAllocateValue(vpool, CNBooleanType, vfunc) ;
        newval->value = value ;
        return newval ;
}

static inline struct CNBooleanValue *
CNCastToBooleanValue(struct CNValue * src)
{
        return CNTypeOfValue(src) == CNBooleanType ? (struct CNBooleanValue *) src : NULL ;
}

static inline struct CNValue *
CNSuperClassOfBooleanValue(struct CNBooleanValue * src)
{
        return &(src->superClass) ;
}

#endif /* CNBooleanValue_h */
