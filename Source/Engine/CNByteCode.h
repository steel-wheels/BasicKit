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
        CNStoreStringOpCode     = 0x001,
        CNPrintOpCode           = 0x002
} CNByteOpCode ;

static inline uint64_t
CNMakeOpCodeAttribute(CNByteOpCode code)
{
        uint64_t codeval = code ;
        return (codeval & 0xfff) ;
}

static inline CNByteOpCode
CNByteOpCodeInAttribute(uint64_t attr)
{
        return (CNByteOpCode) (attr & 0xfff) ;
}

static inline struct CNValue *
CNAllocateStoreStringOpCode(struct CNValuePool * vpool, unsigned int dstregid, struct CNValue * str0)
{
        CNRetainValue(str0) ;
        struct CNOpCode code = {
                .attribute      = CNMakeOpCodeAttribute(CNStoreStringOpCode),
                .destination    = CNAllocateUnsignedInt(dstregid, vpool),
                .source0        = str0,
                .source1        = CNAllocateNull()
        } ;
        return CNAllocateOpCode(vpool, &code) ;
}

static inline struct CNValue *
CNAllocatePrintOpCode(struct CNValuePool * vpool, unsigned int srcregid)
{
        struct CNOpCode code = {
                .attribute      = CNMakeOpCodeAttribute(CNPrintOpCode),
                .destination    = CNAllocateUnsignedInt(srcregid, vpool),
                .source0        = CNAllocateNull(),
                .source1        = CNAllocateNull()
        } ;
        return CNAllocateOpCode(vpool, &code) ;
}

#endif /* CNByteCode_h */
