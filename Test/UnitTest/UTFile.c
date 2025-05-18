/*
 * @file UTFile.c
 * @description The unit test for CNFile
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTFile.h"

bool
UTFile(struct CNValuePool * vpool)
{
        (void) vpool ;

        CNInterface()->printf("(%s) File test\n", __func__) ;

        return true ;
}
