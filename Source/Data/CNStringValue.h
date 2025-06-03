/*
 * @file CNStringValue.h
 * @description Define some string value data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */


#ifndef CNStringValue_h
#define CNStringValue_h

#import <BasicKit/CNValue.h>

struct CNStringValue {
        struct CNValue          superClass ;
        int64_t                 length ;
        char *                  string ;
} ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForStringValue(void) ;

struct CNStringValue *
CNAllocateStringValue(struct CNValuePool * vpool, size_t length, const char * src) ;

static inline size_t
CNLengthOfStringValue(const struct CNStringValue * src)
{
        return src->length ;
}

static inline struct CNStringValue *
CNCastToStringValue(struct CNValue * src)
{
        return CNTypeOfValue(src) == CNStringType ? (struct CNStringValue *) src : NULL ;
}

static inline struct CNValue *
CNSuperClassOfStringValue(struct CNStringValue * src)
{
        return &(src->superClass) ;
}

struct CNStringValue *
CNAddStringValue(struct CNValuePool * vpool, struct CNStringValue * s0, struct CNStringValue * s1) ;

int
CNCompareStringValue(struct CNStringValue * s0, struct CNStringValue * s1) ;

#endif /* CNStringValue_h */
