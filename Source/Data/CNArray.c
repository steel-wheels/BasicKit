/*
 * @file CNArray.c
 * @description Define CNArray data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNArray.h>
#import <BasicKit/CNArrayPool.h>
#import <BasicKit/CNValue.h>
#import <BasicKit/CNUtils.h>

int
CNCompareArray(const struct CNArray * s0, const struct CNArray * s1)
{
        if(s0->count > s1->count){
                return 1 ;
        } else if(s0->count < s1->count) {
                return -1 ;
        }
        // s0->count == s1->count
        uint32_t count = s0->count ;
        for(unsigned int i=0 ; i<count ; i++){
                struct CNValue * v0 = s0->values[i] ;
                struct CNValue * v1 = s1->values[i] ;

                int res = CNCompareValue(v0, v1) ;
                if(res != 0){
                        return res ;
                }
        }
        return 0 ; // every elements are same
}

void
CNRetainArray(struct CNArray * dst)
{
        struct CNValue ** values = dst->values ;
        uint32_t          count  = dst->count ;

        struct CNValue ** ptr    = values ;
        struct CNValue ** endptr = ptr + count ;
        for( ; ptr < endptr ; ptr++){
                struct CNValue * value = *ptr ;
                if(value != NULL){
                        CNRetainValue(value) ;
                }
        }
}

void
CNReleaseArray(struct CNValuePool * pool, struct CNArray * dst)
{
        uint32_t        count  = dst->count ;
        struct CNValue ** values = dst->values ;
        struct CNValue ** ptr    = values ;
        struct CNValue ** endptr = ptr + count ;
        for( ; ptr < endptr ; ptr++){
                struct CNValue * value = *ptr ;
                if(value != NULL){
                        CNReleaseValue(pool, value) ;
                }
        }
        CNFreeArrayData(&(pool->arrayPool), count, values) ;
}

void
CNArrayDump(unsigned int indent, uint32_t count, const struct CNArray * src)
{
        struct CNValue ** values  = src->values ;
        struct CNValue ** ptr     = values ;
        struct CNValue ** endptr  = ptr + count ;
        for( ; ptr < endptr ; ptr++){
                const struct CNValue * value = *ptr ;
                if(value != NULL){
                        CNDumpValue(indent + 1, value) ;
                }
        }
}
