/*
 * @file CNNumberValue.h
 * @description Define some number value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNIntValue_h
#define CNIntValue_h

#import <BasicKit/CNValue.h>

struct CNSignedIntValue {
        struct CNValue          superClass ;
        int64_t                 value ;
} ;

struct CNUnsignedIntValue {
        struct CNValue          superClass ;
        uint64_t                value ;
} ;

struct CNFloatValue {
        struct CNValue          superClass ;
        double                  value ;
} ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForSignedIntValue(void) ;

static inline struct CNSignedIntValue *
CNAllocateSignedIntValue(struct CNValuePool * vpool, int64_t value)
{
        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForSignedIntValue() ;
        struct CNSignedIntValue * newval ;
        newval = (struct CNSignedIntValue *) CNAllocateValue(vpool, CNSignedIntType, vfunc) ;
        newval->value = value ;
        return newval ;
}

static inline struct CNSignedIntValue *
CNCasToSignedIntValue(struct CNValue * src)
{
        return CNTypeOfValue(src) == CNSignedIntType ? (struct CNSignedIntValue *) src : NULL ;
}

#endif /* CNIntValue_h */
