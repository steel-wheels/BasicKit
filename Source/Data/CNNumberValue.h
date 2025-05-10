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

struct CNVirtualValueFunctions *
CNVirtualFunctionsForUnignedIntValue(void) ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForFloatValue(void) ;

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
CNCastToSignedIntValue(struct CNValue * src)
{
        return CNTypeOfValue(src) == CNSignedIntType ? (struct CNSignedIntValue *) src : NULL ;
}

static inline struct CNValue *
CNSuperClassOfSignedIntValue(struct CNSignedIntValue * src)
{
        return &(src->superClass) ;
}

static inline struct CNUnsignedIntValue *
CNAllocateUnsignedIntValue(struct CNValuePool * vpool, uint64_t value)
{
        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForUnignedIntValue() ;
        struct CNUnsignedIntValue * newval ;
        newval = (struct CNUnsignedIntValue *) CNAllocateValue(vpool, CNUnsignedIntType, vfunc) ;
        newval->value = value ;
        return newval ;
}

static inline struct CNUnsignedIntValue *
CNCastToUnsignedIntValue(struct CNValue * src)
{
        return CNTypeOfValue(src) == CNUnsignedIntType ? (struct CNUnsignedIntValue *) src : NULL ;
}

static inline struct CNValue *
CNSuperClassOfUnsignedIntValue(struct CNUnsignedIntValue * src)
{
        return &(src->superClass) ;
}

static inline struct CNFloatValue *
CNAllocateFloatValue(struct CNValuePool * vpool, double value)
{
        struct CNVirtualValueFunctions * vfunc = CNVirtualFunctionsForFloatValue() ;
        struct CNFloatValue * newval ;
        newval = (struct CNFloatValue *) CNAllocateValue(vpool, CNFloatType, vfunc) ;
        newval->value = value ;
        return newval ;
}

static inline struct CNFloatValue *
CNCastToFloatValue(struct CNValue * src)
{
        return CNTypeOfValue(src) == CNFloatType ? (struct CNFloatValue *) src : NULL ;
}

static inline struct CNValue *
CNSuperClassOfFloatValue(struct CNFloatValue * src)
{
        return &(src->superClass) ;
}

#endif /* CNIntValue_h */
