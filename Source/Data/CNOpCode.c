/*
 * @file CNOpCode.h
 * @description Define CNOpCode data structure
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#import <BasicKit/CNOpCode.h>
#import <BasicKit/CNValue.h>
#import <BasicKit/CNValuePool.h>
#import <BasicKit/CNInterface.h>
#import <BasicKit/CNUtils.h>

void
CNRetainOpCode(struct CNOpCode * dst)
{
        CNRetainValue(dst->destination) ;
        CNRetainValue(dst->source0) ;
        CNRetainValue(dst->source1) ;
}

void
CNReleaseOpCode(struct CNValuePool * vpool, struct CNOpCode * dst)
{
        /* dst->attribute is initialized at de-init */
        CNReleaseValue(vpool, dst->destination) ;
        CNReleaseValue(vpool, dst->source0) ;
        CNReleaseValue(vpool, dst->source1) ;
}

int
CNCompareOpCode(const struct CNOpCode * s0, const struct CNOpCode * s1)
{
        if(s0->destination > s1->destination){
                return  1 ;
        } else if(s0->destination < s1->destination){
                return -1 ;
        }
        int result ;
        if((result = CNCompareValue(s0->destination, s1->destination)) != 0){
                return result ;
        }
        if((result = CNCompareValue(s0->source0, s1->source0)) != 0){
                return result ;
        }
        if((result = CNCompareValue(s0->source1, s1->source1)) != 0){
                return result ;
        }
        return result ;
}

void
CNPrintOpCode(const struct CNOpCode * src)
{
        CNInterface()->printf("{\n") ;
        CNInterface()->printf("  opcode:      0x%x\n", src->attribute) ;
        CNInterface()->printf("  destination: ") ; CNPrintValue(src->destination) ;
        CNInterface()->printf("  source0:     ") ; CNPrintValue(src->source0) ;
        CNInterface()->printf("  source1:     ") ; CNPrintValue(src->source1) ;
        CNInterface()->printf("}\n") ;
}
