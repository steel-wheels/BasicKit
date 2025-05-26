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
                "print 1234\n",
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

        CNInterface()->printf("(%s) Generate code\n", __func__) ;
        struct CNArrayValue * codes ;
        codes = CNGenerateCode(CNCodeListInCompiler(&compiler)) ;
        CNDumpByteCodeInArrayValue(codes) ;

        CNInterface()->printf("(%s) Execute code\n", __func__) ;
        struct CNRegisterFile regfile ;
        CNInitRegisterFile(&regfile, vpool) ;
        CNExecuteByteCode(codes, &regfile, 0) ;
        CNDeinitRegisterFile(&regfile) ;

        CNInterface()->printf("(%s) Free parser\n", __func__) ;
        CNReleaseValue(vpool, CNSuperClassOfArrayValue(codes)) ;
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

