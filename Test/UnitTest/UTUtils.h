/*
 * @file UTUtils.c
 * @description The utility functiosn for test
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef UTUtils_h
#define UTUtils_h

#import <BasicKit/CNHeader.h>

void
dumpMemoryUsage(struct CNValuePool * vpool) ;
bool
checkMemoryUsage(struct CNValuePool * vpool) ;

#endif /* UTUtils_h */
