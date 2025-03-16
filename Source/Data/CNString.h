/*
 * @file CNString.h
 * @description Define CNString data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNSTRING_H
#define CNSTRING_H

#import <BasicKit/CNType.h>

#define CNSTRING_ELEMENT_NUM            16

struct CNString {
        struct CNString *       next ;
        uint8_t                 length ;
        char                    buffer[CNSTRING_ELEMENT_NUM] ;
} ;

#endif /* CNSTRING_H */
