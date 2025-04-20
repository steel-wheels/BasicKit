/*
 * @file CNAllocators.c
 * @description Define value allocator
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNAllocators.h>

void
CNInitStringAllocator(struct CNValueList * dst, struct CNValuePool * vpool)
{
        CNInitValueList(dst, vpool) ;
}

static bool
putCharIntoString(struct CNValue * val, char c)
{
        struct CNValueAttribute attr = CNIntToValueAttribute(val->attribute) ;
        if(attr.size < CNSTRING_ELEMENT_NUM){
                (val->stringValue).buffer[attr.size] = c ;
                attr.size += 1 ;
                val->attribute = CNValueAttributeToInt(&attr) ;
                return true ;
        } else {
                return false ;
        }
}

void
CNPutCharIntoAllocator(struct CNValueList * dst, char c)
{
        /* append into current string elements */
        struct CNList * last = CNLastInList(CNFirstItemInValueList(dst)) ;
        if(last != NULL){
                if(putCharIntoString(last->data, c)){
                        return ;
                }
        }
        /* append as new element */
        char str[2] ; str[0] = c ; str[1] = '\0' ;
        struct CNValue * newstr = CNAllocateString(str, 1, dst->valuePool) ;
        CNAppendToValueList(dst, newstr) ;
}
