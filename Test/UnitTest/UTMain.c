/*
 * @file UTMain.c
 * @description The main function for UnitTest
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNHeader.h>
#include "UTList.h"
#include "UTValue.h"
#include "UTStringValue.h"
#include "UTArrayValue.h"

int main(int argc, char * argv[])
{
        bool result = true ;

        CNInitInterface(NULL) ;
        CNInterface()->printf("Hello world\n") ;

        /* check value size*/
        uint32_t usize = CNSizeOfUnionedValue() ;
        if(usize == CNValueSize){
                CNInterface()->printf("sizeof(Value) = %u\n", usize) ;
        } else {
                CNInterface()->error("[Errror] unexpected sizeof(Value) = %u\n", usize) ;
                result = false ;
        }

        /* allocate pools */
        CNInterface()->printf("(%s) Allocate pools\n", __func__) ;
        struct CNListPool lpool ;
        CNInitListPool(&lpool) ;
        struct CNValuePool vpool ;

        /* execute tests */
        bool result0 = UTList(&lpool) ;

        CNInitValuePool(&vpool, &lpool) ;

        /* execute tests */
        bool result1 = UTValue(&vpool) ;
        bool result2 = UTStringValue(&vpool) ;
        bool result3 = UTArrayValue(&vpool) ;

        /* release pools */
        CNInterface()->printf("(%s) Free pools\n", __func__) ;
        CNDeinitValuePool(&vpool) ;
        CNDeinitListPool(&lpool) ;

        /* summaryze result */
        result &= result0 && result1 && result2 && result3 ;
        if(result){
                CNInterface()->printf("SUMMARY: OK\n") ;
                return 0 ;
        } else {
                CNInterface()->printf("SUMMARY: Error\n") ;
                return -1 ;
        }
}
