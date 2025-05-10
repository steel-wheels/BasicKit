/*
 * @file CNNullValue.h
 * @description Define null value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNNullValue_h
#define CNNullValue_h

#import <BasicKit/CNValue.h>

struct CNNullValue {
        struct CNValue          superClass ;
} ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForNullValue(void) ;

struct CNNullValue *
CNAllocateNullValue(void) ;

static inline struct CNNullValue *
CNCastToNullValue(struct CNValue * src)
{
        return CNTypeOfValue(src) == CNNullType ? (struct CNNullValue *) src : NULL ;
}

#endif /* CNNullValue_h */
