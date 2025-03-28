/*
 * @file CNString.h
 * @description Define CNString data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNString.h>
#import <BasicKit/CNValue.h>
#import <stdio.h>

void
CNStringFree(struct CNValuePool * pool, struct CNString * dst)
{
        struct CNValue * next = dst->next ;
        if(next != NULL){
                CNValueFree(pool, next) ;
        }
}

static inline void
CNStringDumpElement(uint32_t count, const struct CNString * src)
{
        unsigned int i ;
        for(i=0 ; i<count ; i++){
                fputc(src->buffer[i], stdout) ;
        }
}

void
CNStringDump(uint32_t count, const struct CNString * src)
{
        if(count > CNSTRING_ELEMENT_NUM) {
                CNStringDumpElement(CNSTRING_ELEMENT_NUM, src) ;
                struct CNValue * next = src->next ;
                if(next != NULL){
                        CNStringDump(count - CNSTRING_ELEMENT_NUM, &(next->stringValue)) ;
                } else {
                        printf("<INTERNAL_ERROR>") ;
                }
        } else {
                CNStringDumpElement(count, src) ;
        }
}
