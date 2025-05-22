/*
 * @file UTCompiler.c
 * @description Defint data structure: CNCompiler
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "CNCompiler.h"
#include "CNNumberValue.h"
#include "CNDictionaryValue.h"

void
CNInitCompiler(struct CNCompiler * dst, struct CNValuePool * vpool)
{
        dst->valuePool          = vpool ;
        dst->next_free_regid    = 0 ;
        dst->identifierTable    = CNAllocateDictionaryValue(vpool) ;
}

bool
CNAllocateRegisterIdForIdentifier(uint64_t * regid, struct CNCompiler * src, struct CNStringValue * ident)
{
        struct CNValue * value = CNValueForKeyInDictionary(src->identifierTable, ident) ;
        if(value != NULL){
                struct CNUnsignedIntValue * uval = CNCastToUnsignedIntValue(value) ;
                *regid = uval->value ;
                return true ;
        } else {
                return false ;
        }
}

