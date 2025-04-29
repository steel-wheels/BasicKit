/*
 * @file CNRegisterFile.h
 * @description Define CNRegisterFile data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNRegisterFile_h
#define CNRegisterFile_h

#import <BasicKit/CNType.h>
#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNValue.h>
#import <BasicKit/CNInterface.h>

#define CNNumberOfRegistersInPage       512
#define CNNumberOfPagesInRegisterFile   32
#define CNMaxRegisterNumInRegisterFile  (CNNumberOfRegistersInPage * CNNumberOfPagesInRegisterFile)

struct CNRegisterFile {
        struct CNValuePool *    valuePool ;
        struct CNList *         currentIndexes ;
        struct CNValue *        arrays[CNNumberOfPagesInRegisterFile] ;         // Type: CNArrayValue
} ;

void
CNInitRegisterFile(struct CNRegisterFile * dst, struct CNValuePool * vpool) ;

void
CNDeinitRegisterFile(struct CNRegisterFile * dst) ;

struct CNValue *
CNRegisterInFile(struct CNRegisterFile * file, unsigned int index) ;

#endif /* CNRegisterFile_h */
