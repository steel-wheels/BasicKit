/*
 * @file CNValue.c
 * @description Define CNValue data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNValue.h>
#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNNullValue.h>
#import <BasicKit/CNNumberValue.h>
#import <BasicKit/CNStringValue.h>
#import <BasicKit/CNArrayValue.h>
#import <BasicKit/CNDictionaryValue.h>
#import <BasicKit/CNInterface.h>

const char *
CNValueTypeName(CNValueType vtype)
{
        const char * result = "?" ;
        switch(vtype){
                case CNNullType:        result = "null" ;       break ;
                case CNBooleanType:     result = "bool" ;       break ;
                case CNSignedIntType:   result = "int64" ;      break ;
                case CNUnsignedIntType: result = "uint64" ;     break ;
                case CNFloatType:       result = "double" ;     break ;
                case CNStringType:      result = "string" ;     break ;
                case CNArrayType:       result = "array" ;      break ;
                case CNDictionaryType:  result = "dict" ;       break ;
                case CNCodeType:        result = "code" ;       break ;
        }
        return result ;
}

union CNUnionedValue {
        uint64_t                        baseValue[CNValueSize / sizeof(uint64_t)] ;
        struct CNNullValue              nullValue ;
        struct CNSignedIntValue         signedValue ;
        struct CNUnsignedIntValue       unsignedValue ;
        struct CNFloatValue             floatValue ;
        struct CNStringValue            stringValue ;
        struct CNArrayValue             arrayValue ;
        struct CNDictionaryValue        dictionaryValue ;
} ;

uint32_t
CNSizeOfUnionedValue(void)
{
        return sizeof(union CNUnionedValue) ;
}

struct CNValue *
CNAllocateValue(struct CNValuePool * vpool, CNValueType vtype, struct CNVirtualValueFunctions * vfunc)
{
        struct CNValue * newvalue = CNAllocateScalar(vpool) ;
        struct CNValueAttribute attr = {
                .isFixed        = false,
                .type           = vtype,
                .referenceCount = 1
        } ;
        newvalue->attribute        = CNValueAttributeToInt(&attr) ;
        newvalue->virtualFunctions = vfunc ;
        return newvalue ;
}

void
CNRetainValue(struct CNValue * src)
{
        struct CNValueAttribute attr = CNIntToValueAttribute(src->attribute) ;
        if(attr.isFixed){
                return ;
        }
        attr.referenceCount += 1 ;
        src->attribute = CNValueAttributeToInt(&attr) ;
}

void
CNReleaseValue(struct CNValuePool * pool, struct CNValue * dst)
{
        struct CNValueAttribute attr = CNIntToValueAttribute(dst->attribute) ;
        if(attr.isFixed){
                return ;
        }
        if(attr.referenceCount > 1){
                attr.referenceCount -= 1 ;
                dst->attribute = CNValueAttributeToInt(&attr) ;
        } else if(attr.referenceCount == 1){
                /* release contents of the value */
                (dst->virtualFunctions)->releaseContents(pool, dst) ;
                /* release value frame */
                struct CNValueAttribute newattr = {
                        .isFixed        = false,
                        .type           = CNNullType,
                        .referenceCount = 0
                } ;
                dst->attribute = CNValueAttributeToInt(&newattr) ;
                CNFreeScalar(pool, dst) ;
        } else { // attr.referenceCount == 0
                CNInterface()->error("[Error] Too much release: %s\n", __func__) ;
        }
}

void
CNPrintValueAttribute(struct CNValue * src)
{
        struct CNValueAttribute attr = CNIntToValueAttribute(src->attribute) ;
        CNInterface()->printf("value-attribute {\n") ;
        CNInterface()->printf("  isFixed:        %s\n", attr.isFixed ? "true" : "false") ;
        CNInterface()->printf("  type:           %s\n", CNValueTypeName(attr.type)) ;
        CNInterface()->printf("  referenceCount: %u\n", attr.referenceCount) ;
        CNInterface()->printf("}\n") ;
}


