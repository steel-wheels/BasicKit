/*
 * @file CNString.h
 * @description Define CNString data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNString.h>
#import <BasicKit/CNValue.h>
#import <stdio.h>
#import <string.h>

#define MIN(A, B)       ((A) < (B) ? (A) : (B))

static int
CNCompareStringElement(uint32_t len, const struct CNString * s0, const struct CNString * s1)
{
        return memcmp(s0->buffer, s1->buffer, len) ;
}

int
CNCompareString(uint32_t len0, const struct CNString * s0, uint32_t len1, const struct CNString * s1)
{
        if(len0 > len1){
                return 1 ;
        } else if(len0 < len1) {
                return -1 ;
        }
        while(len0 > 0) {
                int sublen = MIN(len0, CNSTRING_ELEMENT_NUM) ;
                int res = CNCompareStringElement(sublen, s0, s1) ;
                if(res != 0){
                        return res ;
                }
                s0   =  s0->next ? &(s0->next->stringValue) : NULL ;
                s1   =  s1->next ? &(s1->next->stringValue) : NULL ;
                len0 -= sublen ;
                if(len0 > 0 && (s0 == NULL || s1 == NULL)){
                        fprintf(stderr, "[Error] Null pointer reference at %s\n", __func__) ;
                }
        }
        return 0 ;
}

void
CNRetainString(struct CNString * dst)
{
        struct CNValue * next = dst->next ;
        if(next != NULL){
                CNRetainValue(next) ;
        }
}

void
CNReleaseString(struct CNValuePool * pool, struct CNString * dst)
{
        struct CNValue * next = dst->next ;
        if(next != NULL){
                CNReleaseValue(pool, next) ;
        }
}

static inline void
CNDumpStringElement(uint32_t count, const struct CNString * src)
{
        unsigned int i ;
        for(i=0 ; i<count ; i++){
                fputc(src->buffer[i], stdout) ;
        }
}

void
CNDumpString(uint32_t count, const struct CNString * src)
{
        if(count > CNSTRING_ELEMENT_NUM) {
                CNDumpStringElement(CNSTRING_ELEMENT_NUM, src) ;
                struct CNValue * next = src->next ;
                if(next != NULL){
                        CNDumpString(count - CNSTRING_ELEMENT_NUM, &(next->stringValue)) ;
                } else {
                        printf("<INTERNAL_ERROR>") ;
                }
        } else {
                CNDumpStringElement(count, src) ;
        }
}
