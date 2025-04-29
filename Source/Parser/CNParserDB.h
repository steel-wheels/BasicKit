/*
 * @file CNParserDB.h
 * @description Define CNParserDB data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNParserDB_h
#define CNParserDB_h

#import <BasicKit/CNType.h>
#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNValue.h>

struct CNParserDB {
        struct CNValuePool *    valuePool ;
        /* List of CNDictionary
         *   key:   CNString
         *   value: CNUnsignedInt
         */
        struct CNList *         variableTables ;
} ;

void
CNInitParserDB(struct CNParserDB * pdb, struct CNValuePool * vpool) ;

void
CNDeinitParserDB(struct CNParserDB * pdb) ;

#endif /* CNParserDB_h */
