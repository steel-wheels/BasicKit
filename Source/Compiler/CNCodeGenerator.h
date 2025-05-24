/*
 * @file CNCOddeGenerator.h
 * @description Defint byte code converter
 * @par Copyright
 *   Copyright (C) 2025 Steel Wheels Project
 */


#ifndef CNCodeGenerator_h
#define CNCodeGenerator_h

#import <BasicKit/CNValueList.h>
#import <BasicKit/CNArrayValue.h>
#import <BasicKit/CNByteCode.h>

struct CNArrayValue *
CNGenerateCode(struct CNValueList * codeList) ;

#endif /* CNCodeGenerator_h */
