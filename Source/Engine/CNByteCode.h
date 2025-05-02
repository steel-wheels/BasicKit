/*
 * @file CNByteCode.h
 * @description Define CNByteCode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNByteCode_h
#define CNByteCode_h

#import <BasicKit/CNValue.h>

/* The byte code must be 12 bit */
typedef enum {
        CNStoreStringByteCode   = 0x001,
        CNPrintByteCode         = 0x002
} CNByteCode ;

static inline uint64_t
CNMakeByteCodeAttribute(CNByteCode code)
{
        uint64_t codeval = code ;
        return (codeval & 0xfff) ;
}

static inline CNByteCode
CNByteCodeInAttribute(uint64_t attr)
{
        return (CNByteCode) (attr & 0xfff) ;
}

static inline bool
CNByteCodeInValue(CNByteCode * opcode, const struct CNValue * src)
{
        bool result ;
        switch(CNTypeOfValue(src)){
                case CNOpCodeType: {
                        uint64_t attr = (src->opCodeValue).attribute ;
                        *opcode = CNByteCodeInAttribute(attr) ;
                        result = true ;
                } break ;
                default: {
                        result = false ;
                } break ;
        }
        return result ;
}

static inline struct CNValue *
CNAllocateStoreStringByteCode(struct CNValuePool * vpool, struct CNValue * dstregid, struct CNValue * str0)
{
        uint64_t attr = CNMakeByteCodeAttribute(CNStoreStringByteCode) ;
        struct CNValue * result = CNAllocateOpCode(vpool, attr, dstregid, str0, CNAllocateNull()) ;
        return result ;
}

static inline struct CNValue *
CNAllocatePrintByteCode(struct CNValuePool * vpool, struct CNValue * srcregid)
{
        uint64_t attr = CNMakeByteCodeAttribute(CNPrintByteCode) ;
        return CNAllocateOpCode(vpool, attr, CNAllocateNull(), srcregid, CNAllocateNull()) ;
}

void
CNDumpByteCode(uint32_t count, const struct CNValue * src) ;

#endif /* CNByteCode_h */
