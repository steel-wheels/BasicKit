/*
 * @file CNValue.h
 * @description Define CNValue data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNVALUE_H
#define CNVALUE_H

#import <BasicKit/CNType.h>

typedef enum {
        CNNullType      = 0,
        CNSignedIntType
} CNValueType ;

struct CNValueAttribute {
        bool            isFixed ;               // [63:63]
        CNValueType     type ;                  // [62:32]
        uint32_t        referenceCount ;        // [31: 0]
} ;

static inline uint64_t
CNValueAttributeToInt(const struct CNValueAttribute * src)
{
        uint64_t isfixed = src->isFixed ? 1 : 0 ;
        uint64_t type    = src->type ;
        uint64_t rcnt    = 1 ;
        return (isfixed << 63) | (type << 32) | rcnt ;
}

static inline struct CNValueAttribute
CNIntToValueAttribute(uint64_t src)
{
        struct CNValueAttribute result = {
                .isFixed        = ((src >> 63) & 0x1) != 0,
                .type           = (CNValueType) ((src >> 32) & 0x7FFFFFFF),
                .referenceCount = (uint32_t) (src & 0xFFFFFFFF)
        } ;
        return result ;
}

#define CNValueSize     64

struct CNVirtualValueFunctions {
        void (*releaseContents)(struct CNValue * src) ;
        void (*_print)(struct CNValue * src) ;
} ;

struct CNValue {
        uint64_t                                attribute ;             // convert to CNValueAttribute
        struct CNVirtualValueFunctions *        virtualFunctions ;
} ;

static inline CNValueType
CNTypeOfValue(const struct CNValue * src)
{
        return (CNValueType) ((src->attribute >> 32) & 0x7FFFFFFF) ;
}

uint32_t
CNSizeOfUnionedValue(void) ;

struct CNValue *
CNAllocateValue(struct CNValuePool * vpool, CNValueType vtype, struct CNVirtualValueFunctions * vfuncs) ;

void
CNRetainValue(struct CNValue * src) ;

static inline void
CNPrintValue(struct CNValue * src)
{
        ((src->virtualFunctions)->_print)(src) ;
}

#if 0

#import <BasicKit/CNError.h>
#import <BasicKit/CNList.h>
#import <BasicKit/CNString.h>
#import <BasicKit/CNArray.h>
#import <BasicKit/CNDictionary.h>
#import <BasicKit/CNOpCode.h>
#import <BasicKit/CNScalarPool.h>
#import <BasicKit/CNArrayPool.h>

/* The value must be smaller than 128
 * Because thie value will be embedded into
 * the "attribute" of CNValue
 */
typedef enum {
        CNNullType,
        CNBoolType,
        CNCharType,
        CNSignedIntType,
        CNUnsignedIntType,
        CNFloatType,
        CNStringType,
        CNArrayType,
        CNDictionaryType,
        CNOpCodeType,
        CNErrorType
} CNValueType ;

struct CNValueAttribute {
        bool            isFixed ;               // [63:63]  1 bit
        CNValueType     valueType ;             // [62:56]  7 bit
        uint32_t        referenceCount ;        // [55:28] 28 bit
        uint32_t        size ;                  // [27: 0] 28 bit
} ;

struct CNValue
{
        uint64_t                        attribute ;
        union {
                bool                    boolValue ;
                char                    charValue ;
                int64_t                 int64Value ;
                uint64_t                uint64Value ;
                double                  floatValue ;
                struct CNString         stringValue ;
                struct CNArray          arrayValue ;
                struct CNDictionary     dictionaryValue ;
                struct CNOpCode         opCodeValue ;
                struct CNError          errorValue ;
        } ; // no name
} ;

static inline uint64_t
CNValueAttributeToInt(const struct CNValueAttribute * attr) {
        uint64_t fix    = attr->isFixed ? 1 : 0 ;
        uint64_t type   = attr->valueType & 0x7f ;
        uint64_t refc   = attr->referenceCount & 0x0fffffff ;
        uint64_t size   = attr->size & 0x0fffffff ;
        return    (fix << 63)
                | (type << 56)
                | (refc << 28)
                | (size <<  0)
                ;
}

static inline CNValueType
CNTypeOfValue(const struct CNValue * src)
{
        unsigned int val = (src->attribute >> 56) & 0x7f ;
        return (CNValueType) val ;
}

static inline uint32_t
CNSizeOfValue(const struct CNValue * src)
{
        return src->attribute & 0x0fffffff ;
}

static inline struct CNValueAttribute
CNIntToValueAttribute(uint64_t attr)
{
        uint64_t fix   = (attr >> 63) ;
        uint64_t type  = (attr >> 56) & 0x7f ;
        uint32_t refc  = (attr >> 28) & 0x0fffffff ;
        uint32_t size  = (attr >>  0) & 0x0fffffff ;
        struct CNValueAttribute result = {
                .isFixed        = (fix != 0),
                .valueType      = (CNValueType) type,
                .referenceCount = refc,
                .size           = size
        } ;
        return result ;
}

struct CNValue *
CNAllocateNull(void) ;

struct CNValue *
CNAllocateBool(bool b, struct CNValuePool * pool) ;

struct CNValue *
CNAllocateChar(char c, struct CNValuePool * pool) ;

struct CNValue *
CNAllocateSignedInt(int64_t val, struct CNValuePool * pool) ;

struct CNValue *
CNAllocateUnsignedInt(uint64_t val, struct CNValuePool * pool) ;

uint64_t
CNUnsignedIntValue(const struct CNValue * src) ;

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

static inline uint32_t
CNNumberOfElementsInArray(const struct CNValue * src)
{
        return CNSizeOfValue(src) ;
}

struct CNValue *
CNAllocateDictionary(struct CNValuePool * pool) ;

struct CNValue *
CNAllocateOpCodeWithExecOperands(struct CNValuePool * pool, uint32_t type, uint64_t dstregid, uint64_t src0regid, uint64_t src1regid) ;

struct CNValue *
CNAllocateOpCodeWithStorageOperands(struct CNValuePool * pool, uint32_t type, uint64_t dstregid, struct CNValue * srcval) ;

struct CNValue *
CNAllocateError(CNErrorCode ecode, struct CNValuePool * pool) ;

int
CNCompareValue(const struct CNValue * s0, const struct CNValue * s1) ;

void
CNRetainValue(struct CNValue * dst) ;

void
CNReleaseValue(struct CNValuePool * pool, struct CNValue * dst) ;

void
CNPrintValue(const struct CNValue * src) ;

void
CNPrintValueInfo(const struct CNValue * src) ;

#endif

#endif /* CNVALUE_H */
