/*
 * @file UTFile.c
 * @description The unit test for CNFile
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#include "UTFile.h"
#include "UTUtils.h"

bool
UTFile(struct CNValuePool * vpool)
{
        bool result = true ;
        const char * srcfile = "./BasicKit.framework/Resources/Info.plist" ;

        CNInterface()->printf("(%s) File test: %s\n", __func__, srcfile) ;

        struct CNValueList vlist ;
        if(CNLoadFile(&vlist, srcfile, vpool)){
                CNPrintValueList(&vlist) ;
                CNDeinitValueList(&vlist) ;
        } else {
                CNInterface()->error("(%s) [Error] Failed to read %s\n", __func__, srcfile) ;
                result = false ;
        }

        return checkMemoryUsage(vpool) && result ;
}
