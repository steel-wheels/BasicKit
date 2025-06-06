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

        const char * lines0[] = {
                "print \"A\"\n",
                "print 1234\n",
                "print 12.034\n",
                "print true\n",
                "let a = 100\n",
                ""
        } ;

        const char * lines1[] = {
                "let a = true\n",
                "let b = false\n",
                "let c = a OR b\n",
                "let d = a AND b\n",
                "let e = 0x100\n",
                "let f = 0x2\n",
                "let j = 0b00100001\n",
                "let g = e | f\n",
                "let h = e ^ f\n",
                "let i = e & f\n",
                "let j = e << f\n",
                "let k = e >> f\n",
                "print c\n",
                "print d\n",
                "print g\n",
                "print h\n",
                "print i\n",
                "print j\n",
                "print k\n",
                ""
        } ;

        const char * lines2[] = {
                "let a = 100\n",
                "let b = 100\n",
                "let c = a == b\n",
                "print c\n",            // true
                "let d = 10.2\n",
                "let e = a == d\n",     // false
                "let f = a != d\n",     // true
                "let g = a < d\n",      // false
                "let h = a <= d\n",     // false
                "let i = a > d\n",      // true
                "let j = a >= d\n",     // true
                "print e\n",
                "print f\n",
                "print g\n",
                "print h\n",
                "print i\n",
                "print j\n",
                ""
        } ;

        const char * lines3[] = {
                "let a = 100\n",
                "let b = 100\n",
                "let c = 1.2\n",
                "let d = a + b\n",
                "let e = a - c\n",
                "let f = a * c\n",
                "let g = a / c\n",
                "print d\n",
                "print e\n",
                "print f\n",
                "print g\n",
                ""
        } ;

        result &= testParser(lines0, vpool) ;
        result &= testParser(lines1, vpool) ;
        result &= testParser(lines2, vpool) ;
        result &= testParser(lines3, vpool) ;

        return checkMemoryUsage(vpool) && result ;
}

static bool
testParser(const char * lines[], struct CNValuePool * vpool)
{
        bool result = true ;

        CNInitLexicalParser(vpool) ;

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

        if(compiler.errorCount == 0){
                CNInterface()->printf("(%s) Generate code\n", __func__) ;
                struct CNArrayValue * codes ;
                codes = CNGenerateCode(CNCodeListInCompiler(&compiler)) ;
                CNDumpByteCodeInArrayValue(codes) ;

                CNInterface()->printf("(%s) Execute code\n", __func__) ;
                struct CNRegisterFile regfile ;
                CNInitRegisterFile(&regfile, vpool) ;
                CNExecuteByteCode(codes, &regfile, 0) ;
                CNDeinitRegisterFile(&regfile) ;

                CNReleaseValue(vpool, CNSuperClassOfArrayValue(codes)) ;
        } else {
                CNInterface()->printf("(%s) [Error] Failed to compile\n", __func__) ;
                result = false ;
        }

        CNInterface()->printf("(%s) Free parser\n", __func__) ;
        CNDeinitCompiler(&compiler) ;
        CNDeinitValueList(&codelist) ;

        CNDeinitLexicalParser() ;

        if(checkMemoryUsage(vpool)) {
                return result ;
        } else {
                CNDumpValuePool(0, vpool) ;
                return false ;
        }
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

