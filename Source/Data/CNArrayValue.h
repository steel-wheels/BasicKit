/*
 * @file CNArrayValue.h
 * @description Define array value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNArrayValue_h
#define CNArrayValue_h

#import <BasicKit/CNValue.h>

struct CNArrayValue {
        struct CNValue          superClass ;
        int64_t                 elementNum ;
        struct CNValue **       values ;
} ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForArrayValue(void) ;

struct CNArrayValue *
CNAllocateArrayValue(struct CNValuePool * vpool, unsigned int elmnum) ;

static inline struct CNArrayValue *
CNCastToArrayValue(struct CNValue * src)
{
        return CNTypeOfValue(src) == CNArrayType ? (struct CNArrayValue *) src : NULL ;
}

static inline struct CNValue *
CNSuperClassOfArrayValue(struct CNArrayValue * src)
{
        return &(src->superClass) ;
}

struct CNValue *
CNValueInArray(struct CNArrayValue * array, unsigned int index) ;

bool
CNSetValueToArray(struct CNValuePool * vpool, struct CNArrayValue * array, unsigned int index, struct CNValue * src) ;

#endif /* CNArrayValue_h */
