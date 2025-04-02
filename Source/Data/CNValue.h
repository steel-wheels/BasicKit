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
#import <BasicKit/CNArray.h>
#import <BasicKit/CNScalarPool.h>
#import <BasicKit/CNArrayPool.h>

/* The value must be smaller than 128
 * Because thie value will be embedded into
 * the "attribute" of CNValue
 */
typedef enum {
        CNVoidValueType,
        CNCharValueType,
        CNIntValueType,
        CNFloatValueType,
        CNStringValueType,
        CNArrayValueType
} CNValueType ;

struct CNValueAttribute {
        bool            frameLocked ;           // [63:63]  1 bit
        CNValueType     valueType ;             // [62:56]  7 bit
        uint32_t        referenceCount ;        // [55:28] 28 bit
        uint32_t        size ;                  // [27: 0] 28 bit
} ;

struct CNValue
{
        uint64_t                        attribute ;
        union {
                char                    charValue ;
                int64_t                 int64Value ;
                uint64_t                uint64Value ;
                double                  floatValue ;
                struct CNString         stringValue ;
                struct CNArray          arrayValue ;
        } ; // no name
} ;

static inline uint64_t
CNValueAttributeToInt(const struct CNValueAttribute * attr) {
        uint64_t lock   = attr->frameLocked ? 1 : 0 ;
        uint64_t type   = attr->valueType & 0x7f ;
        uint64_t refc   = attr->referenceCount & 0x0fffffff ;
        uint64_t size   = attr->size & 0x0fffffff ;
        return    (lock << 63)
                | (type << 56)
                | (refc << 28)
                | (size <<  0)
                ;
}

static inline uint32_t
CNSizeOfValue(const struct CNValue * src)
{
        return src->attribute & 0x0fffffff ;
}

static inline struct CNValueAttribute
CNIntToValueAttribute(uint64_t attr)
{
        uint64_t lock  = (attr >> 63) ;
        uint64_t type  = (attr >> 56) & 0x7f ;
        uint32_t refc  = (attr >> 28) & 0x0fffffff ;
        uint32_t size  = (attr >>  0) & 0x0fffffff ;
        struct CNValueAttribute result = {
                .frameLocked    = (lock != 0),
                .valueType      = (CNValueType) type,
                .referenceCount = refc,
                .size           = size
        } ;
        return result ;
}

static inline void
CNSetVoidValue(struct CNValue * dst, bool framelocked)
{
         struct CNValueAttribute attr = {
                 .frameLocked           = framelocked,
                 .valueType             = CNVoidValueType,
                 .referenceCount        = 1,
                 .size                  = 0
         } ;
        dst->attribute = CNValueAttributeToInt(&attr) ;
}

struct CNValuePool {
        struct CNScalarPool     scalarPool ;
        struct CNArrayPool      arrayPool ;
} ;

void
CNInitValuePool(struct CNValuePool * dst, struct CNListPool * lpool) ;

void
CNFreeValuePool(struct CNValuePool * dst) ;

void
CNDumpValuePool(unsigned int indent, const struct CNValuePool * src) ;

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
CNAllocateString(const char * str, uint32_t len, struct CNValuePool * pool) ;

static inline uint32_t
CNLengthOfString(const struct CNValue * src)
{
        return CNSizeOfValue(src) ;
}

struct CNValue *
CNAllocateArray(uint32_t count, struct CNValuePool * pool) ;

void
CNRetainValue(struct CNValue * dst) ;

void
CNReleaseValue(struct CNValuePool * pool, struct CNValue * dst) ;

void
CNDumpValue(unsigned int indent, const struct CNValue * src) ;

#endif /* CNVALUE_H */
