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
#import <BasicKit/CNStringValue.h>
#import <BasicKit/CNVariable.h>
#import <BasicKit/CNCodeValue.h>
#import <BasicKit/CNByteCode.h>
#import <BasicKit/CNParseError.h>

struct CNCompiler
{
        struct CNValuePool *            valuePool ;

        struct CNDictionaryValue *      identifierTable ;
        uint64_t                        next_free_regid ;

        /* List of */
        struct CNValueList              codeList ;

        /* Error */
        unsigned int                    errorCount ;
} ;

void
CNInitCompiler(struct CNCompiler * dst, struct CNValuePool * vpool) ;

void
CNDeinitCompiler(struct CNCompiler * dst) ;

static inline struct CNValueList *
CNCodeListInCompiler(struct CNCompiler * src)
{
        return &(src->codeList) ;
}

bool
CNHasRegisterIdForIdentifier(struct CNVariable * dstvar, struct CNCompiler * src, struct CNStringValue * ident) ;

void
CNSetRegisterIdForIdentifier(struct CNCompiler * dst, const struct CNVariable * srcvar, struct CNStringValue * ident) ;

uint64_t
CNAllocateFreeRegisterId(struct CNCompiler * src) ;

void
CNAppendCodeToCompiler(struct CNCompiler * dst, struct CNCodeValue * code) ;

void
CNPutParseErrorToCompiler(struct CNCompiler * dst, struct CNParseError * src) ;

static inline void
CNDumpCodeInCompiler(struct CNCompiler * src)
{
        CNDumpByteCodeInValueList(&(src->codeList)) ;
}

#endif /* CNCompiler_h */
