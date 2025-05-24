/*
 * @file CNCodeValue.h
 * @description Define opcode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNCodeValue_h
#define CNCodeValue_h

#import <BasicKit/CNValue.h>

typedef enum {
        CNCalcOperandType,
        CNLoadOperandType
} CNOperandType ;

struct CNCodeValueAttribute {
        uint32_t                label ;         /* 28 bit */
        uint32_t                code ;          /* 28 bit */
        CNOperandType           operand ;       /*  8 bit */
} ;

#define CNNoLabel       0x0

static inline uint64_t
CNCodeValueAttributeToInt(const struct CNCodeValueAttribute * attr)
{
        uint64_t llabel   = attr->label ;
        uint64_t lcode    = attr->code ;
        uint64_t loperand = attr->operand ;
        return    (llabel   & 0x0fffffff) <<  (8+28)
                | (lcode    & 0x0fffffff) <<  8
                | (loperand & 0x000000ff) <<  0 ;
}

static inline struct CNCodeValueAttribute
CNIntToCodeValueAttribute(uint64_t imm)
{
        uint32_t label   = (imm >> (8+28)) & 0x0fffffff ;
        uint32_t code    = (imm >>      8) & 0x0fffffff ;
        uint32_t operand = (imm          ) & 0x000000ff ;
        struct CNCodeValueAttribute result = {
                .label   = label,
                .code    = code,
                .operand = (CNOperandType) operand
        } ;
        return result ;
}

struct CNCalcOperand {
        uint64_t                        destinationRegId ;
        uint64_t                        source0RegId ;
        uint64_t                        source1RegId ;
} ;

struct CNLoadOperand {
        uint64_t                        destinationRegId ;
        struct CNValue *                sourceValue ;
} ;

struct CNCodeValue {
        struct CNValue                  superClass ;
        int64_t                         atttribute ;
        union {
                struct CNCalcOperand    calcOperand ;
                struct CNLoadOperand    loadOperand ;
        } ;
} ;

struct CNVirtualValueFunctions *
CNVirtualFunctionsForCodeValue(void) ;

struct CNCodeValue *
CNAllocateCalcCodeValue(struct CNValuePool * vpool, uint32_t code,
                        uint64_t dstregid, uint64_t src0regid, uint64_t src1regid) ;

struct CNCodeValue *
CNAllocateLoadCodeValue(struct CNValuePool * vpool, uint32_t code,
                        uint64_t dstregid, struct CNValue * srcval) ;

static inline uint32_t
CNLabelOfCodeValue(const struct CNCodeValue * src)
{
        struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(src->atttribute) ;
        return attr.label ;
}

static inline void
CNSetLabelToCodeValue(struct CNCodeValue * dst, uint32_t label)
{
        struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(dst->atttribute) ;
        attr.label = label ;
        dst->atttribute = CNCodeValueAttributeToInt(&attr) ;
}

static inline struct CNCodeValue *
CNCastToCodeValue(struct CNValue * src)
{
        return CNTypeOfValue(src) == CNCodeType ? (struct CNCodeValue *) src : NULL ;
}

static inline struct CNValue *
CNSuperClassOfCodeValue(struct CNCodeValue * src)
{
        return &(src->superClass) ;
}

#endif /* CNCodeValue_h */
