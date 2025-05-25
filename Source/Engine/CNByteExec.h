/*
 * @file CNByteExec.h
 * @description Define CNByteCode executer
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */

#ifndef CNByteExec_h
#define CNByteExec_h

#import <BasicKit/CNByteCode.h>
#import <BasicKit/CNArrayValue.h>
#import <BasicKit/CNRegisterFile.h>

void
CNExecuteByteCode(struct CNArrayValue * codes, struct CNRegisterFile * regfile, unsigned int startidx) ;

#endif /* CNByteExec_h */
