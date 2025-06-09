/*
 * @file CNVariable.h
 * @description Defint data structure: CNVariable
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNVariable_h
#define CNVariable_h

#import <BasicKit/CNValue.h>

struct CNVariable {
        CNValueType     valueType ;
        uint64_t        registerId ;
} ;

static inline struct CNVariable
CNMakeVariable(CNValueType vtype, uint64_t regid)
{
        struct CNVariable result = {
                .valueType      = vtype,
                .registerId     = regid
        } ;
        return result ;
}

#endif /* CNVariable_h */
