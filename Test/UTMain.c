/*
 * @file UTMain.c
 * @description The main function for UnitTest
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTList.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
        printf("Hello world\n") ;

        bool result0 = UTList() ;

        int ecode ;
        if(result0) {
                printf("SUMMARY: OK\n") ;
                ecode = 0 ;
        } else {
                printf("SUMMARY: Error") ;
                ecode = 1 ;
        }
        exit(ecode) ;
}
