/*
 * @file CNValue.h
 * @description Define CNValue data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNVALUE_H
#define CNVALUE_H

#import <BasicKit/CNType.h>

/* The value type must be under 0x100 */
#define CNValueTypeMaskBits     8
#define CNValueTypeMask         0xff
typedef enum {
        CNNullType              = 0,
        CNBooleanType,
        CNUnsignedIntType,
        CNSignedIntType,
        CNFloatType,
        CNStringType,
        CNArrayType,
        CNDictionaryType,
        CNCodeType
} CNValueType ;

static inline bool
CNIsNumberValueType(CNValueType vtype)
{
        return (vtype == CNSignedIntType) || (vtype == CNUnsignedIntType) || (vtype == CNFloatType) ;
}

static inline bool
CNIsIntValueType(CNValueType vtype)
{
        return (vtype == CNUnsignedIntType) || (vtype == CNSignedIntType) ;
}

bool
CNUnionValueType(CNValueType * dst, CNValueType src0, CNValueType src1) ;

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
        uint64_t rcnt    = src-> referenceCount ;
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
        void (*releaseContents)(struct CNValuePool * vpool, struct CNValue * src) ;
        void (*print)(struct CNValue * src) ;
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

const char *
CNValueTypeName(CNValueType vtype) ;

uint32_t
CNSizeOfUnionedValue(void) ;

struct CNValue *
CNAllocateValue(struct CNValuePool * vpool, CNValueType vtype, struct CNVirtualValueFunctions * vfuncs) ;

void
CNRetainValue(struct CNValue * src) ;

void
CNReleaseValue(struct CNValuePool * pool, struct CNValue * dst) ;

static inline void
CNPrintValue(struct CNValue * src)
{
        ((src->virtualFunctions)->print)(src) ;
}

void
CNPrintValueAttribute(struct CNValue * src) ;

#endif /* CNVALUE_H */
