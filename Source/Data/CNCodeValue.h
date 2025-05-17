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
        uint32_t                code ;
        CNValueType             type ;
        CNOperandType           operand ;
} ;

static inline uint64_t
CNCodeValueAttributeToInt(const struct CNCodeValueAttribute * attr)
{
        uint64_t lcode    = attr->code ;
        uint64_t ltype    = attr->type ;
        uint64_t loperand = attr->operand ;
        return    (lcode    & 0xffffffff) << 32
                | (ltype    & 0x0000ffff) << 16
                | (loperand & 0x0000ffff) <<  0 ;
}

static inline struct CNCodeValueAttribute
CNIntToCodeValueAttribute(uint64_t imm)
{
        uint32_t code    = imm >> 32 ;
        uint32_t type    = (imm >> 16) & 0xffff ;
        uint32_t operand = (imm      ) & 0xffff ;
        struct CNCodeValueAttribute result = {
                .code    = code,
                .type    = (CNValueType) type,
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
CNAllocateCalcCodeValue(struct CNValuePool * vpool, uint32_t code, CNValueType vtype,
                        uint64_t dstregid, uint64_t src0regid, uint64_t src1regid) ;

struct CNCodeValue *
CNAllocateLoadCodeValue(struct CNValuePool * vpool, uint32_t code, CNValueType vtype,
                        uint64_t dstregid, struct CNValue * srcval) ;

static inline struct CNCodeValue *
CNCastToCodeValue(struct CNValue * src)
{
        return CNTypeOfValue(src) == CNCodeType ? (struct CNCodeValue *) src : NULL ;
}

#endif /* CNCodeValue_h */
