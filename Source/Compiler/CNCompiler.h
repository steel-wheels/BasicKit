/*
 * @file UTCompiler.h
 * @description Defint data structure: CNCompiler
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNCompiler_h
#define CNCompiler_h

#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNValueList.h>
#import <BasicKit/CNCodeValue.h>
#import <BasicKit/CNStringIterator.h>

struct CNCompiler
{
        struct CNValuePool *            valuePool ;

        struct CNDictionaryValue *      identifierTable ;
        uint64_t                        next_free_regid ;

        struct CNValueList              codeList ;
} ;

void
CNInitCompiler(struct CNCompiler * dst, struct CNValuePool * vpool) ;

void
CNDeinitCompiler(struct CNCompiler * dst) ;

bool
CNHasRegisterIdForIdentifier(uint64_t * regid, struct CNCompiler * src, struct CNStringValue * ident) ;

uint64_t
CNAllocateFreeRegisterId(struct CNCompiler * src) ;

void
CNAppendCodeToCompiler(struct CNCompiler * dst, struct CNCodeValue * code) ;

#endif /* CNCompiler_h */
