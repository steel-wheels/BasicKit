/*
 * @file UTMain.c
 * @description The main function for UnitTest
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTList.h"
#include "UTArrayPool.h"
#include "UTScalarPool.h"
#include "UTArray.h"
#include "UTString.h"
#include "UTDictionary.h"
#include "UTValue.h"
#include "UTValueList.h"
#include "UTIterator.h"
#include "UTAllocator.h"
#include "UTRegisters.h"
#include "UTParser.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
        printf("Hello world\n") ;

        CNInitInterface(NULL) ;
        CNInterface()->printf("Message from interface\n") ;

        bool result0  = UTList() ;
        bool result1  = UTArrayPool() ;
        bool result2  = UTScalarPool() ;
        bool result3  = UTString() ;
        bool result4  = UTArray() ;
        bool result5  = UTDictionary() ;
        bool result6  = UTValue() ;
        bool result7  = UTValueList() ;
        bool result8  = UTIterator() ;
        bool result9  = UTAllocator() ;
        bool result10 = UTRegisters() ;
        bool result11 = UTParser() ;

        int ecode ;
        bool result = result0 && result1 && result2  && result3 &&
                      result4 && result5 && result6  && result7 &&
                      result8 && result9 && result10 && result11 ;
        if(result) {
                printf("SUMMARY: OK\n") ;
                ecode = 0 ;
        } else {
                printf("SUMMARY: Error\n") ;
                ecode = 1 ;
        }
        exit(ecode) ;
}
