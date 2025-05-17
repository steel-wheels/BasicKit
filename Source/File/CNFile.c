/*
 * @file CNFile.c
 * @description Define functions to operate file
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNFile.h>
#include <stdio.h>

/*
bool
CNLoadFile(struct CNValueList * dst, const char * filename, struct CNValuePool * vpool)
{
        FILE *  file = fopen(filename, "r") ;
        if(file != NULL){
                int     c ;
                CNInitValueList(dst, vpool) ;
                while((c = fgetc(file)) != EOF){
                        CNPutCharIntoStringList(dst, c) ;
                }
                return true ;
        } else {
                return false ;
        }
}
*/
