/*
 * @file UTParser.c
 * @description The unit test for arisia basic parser
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTParser.h"
#include "UTUtils.h"
#include <string.h>

static bool
testParser(const char * lines[], struct CNValuePool * vpool) ;

static void
makeCodeList(struct CNValueList * dst, struct CNValuePool * vpool, const char ** sources) ;

bool
UTParser(struct CNValuePool * vpool)
{
        bool result = true ;
        CNInterface()->printf("(%s) Parser test\n", __func__) ;

        CNInitLexicalParser(vpool) ;

        const char * lines0[] = {
                "print \"A\"\n",
                ""
        } ;

        result &= testParser(lines0, vpool) ;

        CNDeinitLexicalParser() ;

        return checkMemoryUsage(vpool) && result ;
}

static bool
testParser(const char * lines[], struct CNValuePool * vpool)
{
        CNInterface()->printf("(%s) Test parser\n", __func__) ;
        dumpMemoryUsage(vpool) ;
        struct CNValueList codelist ;
        makeCodeList(&codelist, vpool, lines) ;

        CNInterface()->printf("(%s) Dump program\n", __func__) ;
        CNPrintValueList(&codelist) ;
        CNInterface()->printf("\n", __func__) ;

        CNSetSourceCodeToLexicalParser(&codelist) ;

        struct CNCompiler compiler ;
        CNInitCompiler(&compiler, vpool) ;
        CNSetCompilerToSyntaxParser(&compiler, vpool) ;

        CNInterface()->printf("(%s) Start parser\n", __func__) ;
        CNStartParser() ;

        CNInterface()->printf("(%s) Dump program\n", __func__) ;
        CNDumpCodeInCompiler(&compiler) ;

        CNInterface()->printf("(%s) Free parser\n", __func__) ;
        CNDeinitCompiler(&compiler) ;
        CNDeinitValueList(&codelist) ;
        return checkMemoryUsage(vpool) ;
}

static void
makeCodeList(struct CNValueList * dst, struct CNValuePool * vpool, const char ** sources)
{
        CNInitValueList(dst, vpool) ;
        const char ** line ;
        for(line = sources ; strlen(*line) > 0 ; line++){
                struct CNStringValue * str ;
                str = CNAllocateStringValue(vpool, (unsigned int) strlen(*line), *line) ;
                CNAppendValueToValueList(dst, CNSuperClassOfStringValue(str)) ;
                CNReleaseValue(vpool, CNSuperClassOfStringValue(str)) ;
        }
}

