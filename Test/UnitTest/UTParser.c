/*
 * @file UTParser.c
 * @description The unit test for ArisiaBasic parser
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTParser.h"
#include <string.h>

static bool testParsers(struct CNValuePool * vpool, unsigned int testid) ;

bool
UTParser(void)
{
        struct CNMemoryUsage usage ;
        bool result = true ;

        printf("(%s) Initial state\n", __func__) ;
        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        struct CNValuePool vpool ;
        CNInitValuePool(&vpool, &lpool) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        printf("(%s) Parse state\n", __func__) ;
        result &= testParsers(&vpool, 0) ;

        printf("(%s) Check state\n", __func__) ;
        usage = CNMemoryUsageOfValuePool(&vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        if(usage.allocatedSize == usage.usableSize) {
                printf("(%s) No memory leak\n", __func__) ;
        } else {
                printf("(%s) [Error] some memory leak\n", __func__) ;
                result = false ;
        }

        printf("(%s) Final state\n", __func__) ;
        CNDeinitValuePool(&vpool) ;
        CNDeinitListPool(&lpool) ;

        return result ;
}

const char * program0[] = {
        "print \"a\"",
        NULL
} ;

static bool
testParsers(struct CNValuePool * vpool, unsigned int testid)
{
        bool result = true ;

        struct CNValueList lines ;
        CNInitValueList(&lines, vpool) ;

        /* load program */
        const char ** program ;
        switch(testid) {
                case 0: {
                        program = program0 ;
                } break ;
                default: {
                        printf("(%s) [Error] Unknown test id: %u\n", __func__, testid) ;
                        return false ;
                } break ;
        }
        for(unsigned int i = 0 ; program[i] != NULL ; i++){
                const char * src = program[i] ;
                struct CNValue * line = CNAllocateString(src, (uint32_t) strlen(src), vpool) ;
                CNAppendToValueList(&lines, line) ;
                CNReleaseValue(vpool, line) ;
        }
        CNDumpValueList(0, &lines) ;

        /* exec parser */
        struct CNParserDB pdb ;
        CNInitParserDB(&pdb, vpool) ;
        CNInitParser(&pdb) ;
        CNExecParser(&lines) ;
        CNDeinitParserDB(&pdb) ;

        CNDeinitValueList(&lines) ;

        printf("(%s) Check state\n", __func__) ;
        struct CNMemoryUsage usage = CNMemoryUsageOfValuePool(vpool) ;
        CNDumpMemoryUsage(0, &usage) ;

        if(usage.allocatedSize == usage.usableSize) {
                printf("(%s) No memory leak\n", __func__) ;
        } else {
                printf("(%s) [Error] some memory leak\n", __func__) ;
                result = false ;
        }
        return result ;
}
