/*
 * @file CNFile.c
 * @description Define functions to operate file
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNFile.h>
#import <BasicKit/CNStringValue.h>
#import <BasicKit/CNInterface.h>
#include <stdlib.h>
#include <stdio.h>

bool
CNLoadFile(struct CNValueList * dst, const char * filename, struct CNValuePool * vpool)
{
        /* initialize the list */
        CNInitValueList(dst, vpool) ;

        FILE *  file = fopen(filename, "r") ;
        if(file == NULL){
                return false ; // Failed to open
        }

        size_t  bufsize = 2048 ;
        char *  buffer  = malloc(bufsize) ;
        if(buffer == NULL){
                CNInterface()->error("[Error] Failed to alloccate memory at %s\n", __func__) ;
                return false ;
        }

        size_t          cursize = 0 ;
        int             c ;
        while((c = fgetc(file)) != EOF){
                /* expand buffer if it required */
                if(cursize >= bufsize - 1){ // -1 for EOS
                        bufsize += 1024 ;
                        buffer  = realloc(buffer, bufsize) ;
                }
                /* add character to buffer */
                buffer[cursize] = (uint8_t) c ;
                cursize += 1 ;
                /* the newline code means the end of line */
                if(c == '\n'){
                        buffer[cursize] = '\0' ;

                        struct CNStringValue * newstr ;
                        newstr = CNAllocateStringValue(vpool, (unsigned int) cursize, buffer) ;
                        CNAppendValueToValueList(dst, CNSuperClassOfStringValue(newstr)) ;
                        CNReleaseValue(vpool, CNSuperClassOfStringValue(newstr)) ;
                        cursize = 0 ;
                }
        }

        if(cursize > 0){
                buffer[cursize] = '\0' ;
                
                struct CNStringValue * newstr ;
                newstr = CNAllocateStringValue(vpool, (unsigned int) cursize, buffer) ;
                CNAppendValueToValueList(dst, CNSuperClassOfStringValue(newstr)) ;
                CNReleaseValue(vpool, CNSuperClassOfStringValue(newstr)) ;
                cursize = 0 ;
        }

        free(buffer) ;

        return true ;
}

