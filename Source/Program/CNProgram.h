/*
 * @file CNProgtam.h
 * @description Define CNProgram data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNProgram_h
#define CNProgram_h

#import <BasicKit/CNType.h>
#import <BasicKit/CNList.h>
#import <BasicKit/CNIterators.h>

struct CNProgram {
        /* list item
         *  - next                      Next list item
         *  - attribute                 undefined
         *  - data                      String Value (CNValue with StringType)
         */
        struct CNList *                 lineList ;

        /* Parsing state */
        struct CNStringListIterator     iterator ;
} ;

struct CNProgram *
CNCurrentProgram(void) ;

char
CNGetCharFromProgram(void) ;

#endif /* CNProgram_h */
