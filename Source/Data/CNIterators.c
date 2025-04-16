/*
 * @file CNIterators.c
 * @description Define iterators for each data
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNIterators.h>
#import <BasicKit/CNInterface.h>
#import <stdio.h>

void
CNInitStringIterator(struct CNStringIterator * dst, struct CNValue * string)
{
        if(CNTypeOfValue(string) == CNStringType){
                CNRetainValue(string) ;
                dst->sourceString       = string ;
                dst->sourceLength       = CNLengthOfString(string) ;
                dst->currentIndex       = 0 ;
        } else {
                CNInterface()->error("[Error] Invakid parameter type in %s\n", __func__) ;
                dst->sourceString       = NULL ;
                dst->currentIndex       = 0 ;
        }
}

char
CNGetCharacterFromStringIterator(struct CNStringIterator * src)
{
        struct CNValue * curval = src->sourceString ;
        unsigned int curidx = src->currentIndex ;
        unsigned int curlen = CNLengthOfString(curval) ;

        char result ;
        if(curidx < curlen){
                if(curidx < CNSTRING_ELEMENT_NUM){
                        result = (curval->stringValue).buffer[curidx] ;
                        src->currentIndex += 1 ;
                } else {
                        struct CNValue * next = (curval->stringValue).next ;
                        if(next != NULL){
                                src->sourceString  = next ;
                                src->currentIndex -= CNSTRING_ELEMENT_NUM ;
                                result = CNGetCharacterFromStringIterator(src) ;
                        } else {
                                CNInterface()->error("[Error] Not enough string in %s\n", __func__) ;
                                result = EOF ;
                        }
                }
        } else {
                result = EOF ;
        }
        return result ;
}

void
CNInitStringListIterator(struct CNStringListIterator * dst, struct CNList * strings)
{
        struct CNValue * firststr = strings->data ;
        dst->sourceStringList = strings->next ;
        CNInitStringIterator(&(dst->stringIterator), firststr) ;
}

char
CNGetCharacterFromStringListIterator(struct CNStringListIterator * src)
{
        char c = CNGetCharacterFromStringIterator(&(src->stringIterator)) ;
        if(c != EOF){
                return c ;
        } else {
                struct CNList * list = src->sourceStringList ;
                if(list != NULL){
                        struct CNValue * nextstr = list->data ;
                        src->sourceStringList = list->next ;
                        CNInitStringIterator(&(src->stringIterator), nextstr) ;
                        return CNGetCharacterFromStringListIterator(src) ;
                } else {
                        return EOF ;
                }
        }
}
