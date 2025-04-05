/*
 * @file CNInterface.h
 * @description Define interface of this framework
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNInterface_h
#define CNInterface_h

#import <BasicKit/CNType.h>

struct CNInterfaceProtocol {
        int (*printf)(const char * format, ...) ;
        int (*error)(const char * format, ...) ;
} ;

void
CNInitInterface(struct CNInterfaceProtocol * prot) ;

const struct CNInterfaceProtocol *
CNInterface(void) ;

#endif /* CNInterface_h */
