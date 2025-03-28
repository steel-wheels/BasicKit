/*
 * @file CNValue.h
 * @description Define CNValue data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNVALUE_H
#define CNVALUE_H

#import <BasicKit/CNType.h>
#import <BasicKit/CNList.h>
#import <BasicKit/CNString.h>

typedef enum {
        CNVoidValueType,
        CNCharValueType,
        CNIntValueType,
        CNFloatValueType,
        CNStringValueType
} CNValueType ;

struct CNValue
{
        uint64_t                        attribute ;
        union {
                char                    charValue ;
                int64_t                 int64Value ;
                uint64_t                uint64Value ;
                double                  floatValue ;
                struct CNString         stringValue ;
        } ; // no name
} ;

static inline uint64_t
CNMakeValueAttribute(CNValueType type, uint32_t size)
{
        uint64_t tval = type ;
        uint64_t sval = size ;
        return (sval << 32) | tval ;
}

static inline CNValueType
CNTypeOfValue(const struct CNValue * src)
{
        return (CNValueType) (src->attribute & 0xffffffff) ;
}

static inline uint32_t
CNSizeOfValue(const struct CNValue * src)
{
        return (src->attribute >> 32) ;
}

struct CNValue *
CNAllocateVoid(struct CNValuePool * pool) ;

struct CNValue *
CNAllocateChar(char c, struct CNValuePool * pool) ;

struct CNValue *
CNAllocateInt64(int64_t val, struct CNValuePool * pool) ;

struct CNValue *
CNAllocateUInt64(uint64_t val, struct CNValuePool * pool) ;

struct CNValue *
CNAllocateFloat(double val, struct CNValuePool * pool) ;

struct CNValue *
CNAllocateString(const char * str, struct CNValuePool * pool) ;

void
CNValueFree(struct CNValuePool * pool, struct CNValue * dst) ;

unsigned int
CNValueCountOfFreeItems(const struct CNValuePool * pool) ;

void
CNValueDump(unsigned int indent, const struct CNValue * src) ;

#endif /* CNVALUE_H */
