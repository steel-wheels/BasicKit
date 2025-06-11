%{

/*
 * Reference: https://www.lysator.liu.se/c/ANSI-C-grammar-y.html
 */

#include "ArisiaBasic.h"
#include "CNParser.h"
#include "CNByteCode.h"
#include "CNBooleanValue.h"
#include "CNNumberValue.h"
#include "CNStringValue.h"
#include "CNCodeValue.h"
#include <stdio.h>
#include <string.h>

static void yyerror(const char * message) ;

static struct CNValuePool *     s_value_pool = NULL ;
static struct CNCompiler *      s_compiler   = NULL ;

static struct CNVariable
allocateStoreStatement(CNValueType restype, struct CNValue * src) ;
static struct CNVariable
allocateCastExpression(CNValueType dsttype, const struct CNVariable * src) ;
static struct CNVariable
allocateConvertExpression(CNValueType dsttype, const struct CNVariable * src) ;

static bool
unionValueType(struct CNVariable * dst0, struct CNVariable * dst1,
               const struct CNVariable * src0, const struct CNVariable * src1) ;

void
CNSetCompilerToSyntaxParser(struct CNCompiler * compiler, struct CNValuePool * vpool)
{
        s_value_pool    = vpool ;
        s_compiler      = compiler ;
}

static inline struct CNVariable
allocateCompareExpression(struct CNVariable * src0, struct CNVariable * src1, CNCompareOperation op)
{
        struct CNVariable lvar, rvar, result ;
        if(unionValueType(&lvar, &rvar, src0, src1)){
                uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ; // allocate register after cast operation
                struct CNCodeValue * code = CNAllocateCompareCode(s_value_pool, op, dstid, lvar.valueType, lvar.registerId, rvar.registerId) ;
                CNAppendCodeToCompiler(s_compiler, code) ;
                CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
                result = CNMakeVariable(CNBooleanType, dstid) ;
        } else {
                uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ;
                result = CNMakeVariable(src0->valueType, dstid) ;
        }
        return result ;
}

static inline struct CNVariable
allocateBitBinaryExpression(struct CNVariable * src0, struct CNVariable * src1, CNBitBinaryOperation op)
{
        struct CNVariable lvar = allocateCastExpression(CNUnsignedIntType, src0) ;
        struct CNVariable rvar = allocateCastExpression(CNUnsignedIntType, src1) ;
        uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ;
        struct CNCodeValue * code = CNAllocateBitBinaryCode(s_value_pool, op, dstid, lvar.registerId, rvar.registerId) ;
        CNAppendCodeToCompiler(s_compiler, code) ;
        CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        return CNMakeVariable(CNBooleanType, dstid) ;
}

static inline struct CNVariable
allocateNumberBinaryExpression(struct CNVariable * src0, struct CNVariable * src1, CNNumberBinaryOperation op)
{
        if(!CNIsNumberValueType(src0->valueType)){
                unsigned int line = CNGetCurrentParsingLine() ;
                struct CNParseError error = CNMakeUnexpectedTypeError(src0->valueType, line) ;
                CNPutParseErrorToCompiler(s_compiler, &error) ;
                CNDeinitParseError(s_value_pool, &error) ;

                uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ;
                return CNMakeVariable(src1->valueType, dstid) ;
        }
        if(!CNIsNumberValueType(src1->valueType)){
                unsigned int line = CNGetCurrentParsingLine() ;
                struct CNParseError error = CNMakeUnexpectedTypeError(src1->valueType, line) ;
                CNPutParseErrorToCompiler(s_compiler, &error) ;
                CNDeinitParseError(s_value_pool, &error) ;

                uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ;
                return CNMakeVariable(src0->valueType, dstid) ;
        }
        struct CNVariable lvar, rvar, result ;
        if(unionValueType(&lvar, &rvar, src0, src1)){
                uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ; // allocate register after cast operation
                struct CNCodeValue * code = CNAllocateNumberBinaryCode(s_value_pool, op, dstid, lvar.valueType, lvar.registerId, rvar.registerId) ;
                if(code != NULL){
                        CNAppendCodeToCompiler(s_compiler, code) ;
                        CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
                        result = CNMakeVariable(lvar.valueType, dstid) ;
                } else {
                        uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ;
                        result = CNMakeVariable(lvar.valueType, dstid) ;
                }
        } else {
                uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ;
                result = CNMakeVariable(src0->valueType, dstid) ;
        }
        return result ;
}

static inline struct CNVariable
allocateLogicalBinaryExpression(struct CNVariable * src0, struct CNVariable * src1, CNLogicalBinaryOperation op)
{
        struct CNVariable lvar = allocateCastExpression(CNBooleanType, src0) ;
        struct CNVariable rvar = allocateCastExpression(CNBooleanType, src1) ;
        uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ;
        struct CNCodeValue * code = CNAllocateLogicalBinaryCode(s_value_pool, op, dstid, lvar.registerId, rvar.registerId) ;
        CNAppendCodeToCompiler(s_compiler, code) ;
        CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        return CNMakeVariable(CNBooleanType, dstid) ;
}

static inline struct CNVariable
allocateFloatBinaryExpression(struct CNVariable * src0, struct CNVariable * src1, CNNumberBinaryOperation op)
{
        if(src0->valueType == CNFloatType || src1->valueType == CNFloatType){
                return allocateNumberBinaryExpression(src0, src1, op) ;
        } else {
                unsigned int line = CNGetCurrentParsingLine() ;

                struct CNParseError error = CNMakeFloatDataRequiredError(op, line) ;
                CNPutParseErrorToCompiler(s_compiler, &error) ;
                CNDeinitParseError(s_value_pool, &error) ;

                uint64_t regid = CNAllocateFreeRegisterId(s_compiler) ;
                return CNMakeVariable(CNFloatType, regid) ;
        }
}

static inline struct CNVariable
allocateIntBinaryExpression(struct CNVariable * src0, struct CNVariable * src1, CNNumberBinaryOperation op)
{
        if(CNIsIntValueType(src0->valueType) || CNIsIntValueType(src1->valueType)){
                return allocateNumberBinaryExpression(src0, src1, op) ;
        } else {
                unsigned int line = CNGetCurrentParsingLine() ;

                struct CNParseError error = CNMakeIntDataRequiredError(op, line) ;
                CNPutParseErrorToCompiler(s_compiler, &error) ;
                CNDeinitParseError(s_value_pool, &error) ;

                uint64_t regid = CNAllocateFreeRegisterId(s_compiler) ;
                return CNMakeVariable(CNFloatType, regid) ;
        }
}

static inline struct CNVariable
allocateNumberUnaryExpression(struct CNVariable * src, CNNumberUnaryOperation op)
{
        if(!CNIsNumberValueType(src->valueType)){
                unsigned int line = CNGetCurrentParsingLine() ;
                struct CNParseError error = CNMakeUnexpectedTypeError(src->valueType, line) ;
                CNPutParseErrorToCompiler(s_compiler, &error) ;
                CNDeinitParseError(s_value_pool, &error) ;

                uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ;
                return CNMakeVariable(src->valueType, dstid) ;
        }
        uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ; // allocate register after cast operation
        struct CNCodeValue * code = CNAllocateNumberUnaryCode(s_value_pool, op, dstid, src->valueType, src->registerId) ;
        CNAppendCodeToCompiler(s_compiler, code) ;
        CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        return CNMakeVariable(CNBooleanType, dstid) ;
}

static inline struct CNVariable
allocateLogicalUnaryExpression(struct CNVariable * src, CNLogicalUnaryOperation op)
{
        uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ; // allocate register to store result
        struct CNCodeValue * code = CNAllocateLogicalUnaryCode(s_value_pool, op, dstid, src->valueType, src->registerId) ;
        CNAppendCodeToCompiler(s_compiler, code) ;
        CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        return CNMakeVariable(CNBooleanType, dstid) ;
}

static inline struct CNVariable
allocateBitUnaryExpression(struct CNVariable * src, CNBitUnaryOperation op)
{
        uint64_t dstid  = CNAllocateFreeRegisterId(s_compiler) ; // allocate register to store result
        struct CNCodeValue * code = CNAllocateBitUnaryCode(s_value_pool, op, dstid, src->registerId) ;
        CNAppendCodeToCompiler(s_compiler, code) ;
        CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        return CNMakeVariable(CNBooleanType, dstid) ;
}

%}

%locations

%token  IDENTIFIER TYPE_IDENTIFIER
%token  IF ELSE ENDIF LET PRINT STRING THEN
%token  OP_AND OP_OR OP_BIT_OR OP_BIT_AND OP_BIT_XOR OP_LOG_NOT OP_EQUAL OP_NOT_EQUAL
%token  OP_LESS_THAN OP_LESS_EQUAL OP_GREATER_THAN OP_GREATE_EQUAL
%token  OP_SHIFT_LEFT OP_SHIFT_RIGHT OP_DIV OP_MOD
%token  INT_VALUE FLOAT_VALUE FALSE_VALUE TRUE_VALUE

%%

%start  statement_list ;

statement_list
        : statement
        | statement_list statement
        ;

statement_list_opt
        : /* empty */
        | statement_list_opt statement
        ;

statement: LET IDENTIFIER '=' expression
        {
                struct CNStringValue *  ident = $2.identifier ;
                struct CNVariable       src   = $4.variable ;
                struct CNVariable       dstvar ;
                if(!CNHasRegisterIdForIdentifier(&dstvar, s_compiler, ident)){
                        /* the identifier is not in register table */
                        uint64_t dstid = CNAllocateFreeRegisterId(s_compiler) ;
                        dstvar = CNMakeVariable(src.valueType, dstid) ;
                        /* register the identifier */
                        CNSetRegisterIdForIdentifier(s_compiler, &dstvar, ident) ;
                }
                struct CNCodeValue * code = CNAllocateMoveCode(s_value_pool, dstvar.registerId, src.registerId) ;
                CNAppendCodeToCompiler(s_compiler, code) ;
                CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
                CNReleaseValue(s_value_pool, CNSuperClassOfStringValue(ident)) ;
        }
        | IF expression THEN
          {
          }
          statement_list_opt
          {
                  /* append jump statement to end of if statement */
                  uint32_t label = CNUpdateLabelInCompiler(s_compiler) ;
                  struct CNCodeValue * code = CNAllocateJumpCode(s_value_pool, label) ;
                  CNAppendCodeToCompiler(s_compiler, code) ;
                  CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
          }
          else_statement_opt ENDIF
          {
                  /* add nop code with target label */
                  struct CNCodeValue * nopcode = CNAllocateNopCode((s_value_pool)) ;

                  struct CNCodeValueAttribute attr = CNIntToCodeValueAttribute(nopcode->codeAttribute) ;
                  attr.label = CNCurrentLabelInCompiler(s_compiler) ;
                  nopcode->codeAttribute = CNCodeValueAttributeToInt(&attr) ;

                  CNAppendCodeToCompiler(s_compiler, nopcode) ;
                  CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(nopcode)) ;
          }
        | PRINT expression
        {
                struct CNVariable src = $2.variable ;
                struct CNCodeValue * code = CNAllocatePrintCode(s_value_pool, src.registerId) ;
                CNAppendCodeToCompiler(s_compiler, code) ;
                CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        }
        ;

else_statement_opt
        : /* empty */
        {
                $$.unsignedIntValue = 0 ; // no else part
        }
        | ELSE statement_list_opt
        {
                $$.unsignedIntValue = 1 ; // has else part
        }
        ;

expression
        : logical_and_expression
        {
                $$ = $1 ;
        }
        | expression OP_OR logical_and_expression
        {
                $$.variable = allocateLogicalBinaryExpression(&($1.variable), &($3.variable), CNLogicalOrOperation) ;
        }
        ;

logical_and_expression
        : inclusive_or_expression
        {
                $$ = $1 ;
        }
        | logical_and_expression OP_AND inclusive_or_expression
        {
                $$.variable = allocateLogicalBinaryExpression(&($1.variable), &($3.variable), CNLogicalAndOperation) ;
        }
        ;

inclusive_or_expression
        : exclusive_or_expression
        {
                $$ = $1 ;
        }
        | inclusive_or_expression OP_BIT_OR exclusive_or_expression
        {
                $$.variable = allocateBitBinaryExpression(&($1.variable), &($3.variable), CNBitOrOperation) ;
        }
        ;

exclusive_or_expression
        : and_expression
        {
                $$ = $1 ;
        }
        | exclusive_or_expression OP_BIT_XOR and_expression
        {
                $$.variable = allocateBitBinaryExpression(&($1.variable), &($3.variable), CNBitXorOperation) ;
        }
        ;

and_expression
        : equarilty_expression
        {
                $$ = $1 ;
        }
        | and_expression OP_BIT_AND equarilty_expression
        {
                $$.variable = allocateBitBinaryExpression(&($1.variable), &($3.variable), CNBitAndOperation) ;
        }
        ;

equarilty_expression
        : relational_expression
        {
                $$ = $1 ;
        }
        | equarilty_expression OP_EQUAL relational_expression
        {
                $$.variable =  allocateCompareExpression((&$1.variable), &($3.variable), CNCompareEqual) ;
        }
        | equarilty_expression OP_NOT_EQUAL relational_expression
        {
                $$.variable =  allocateCompareExpression((&$1.variable), &($3.variable), CNCompareNotEqual) ;
        }
        ;

relational_expression
        : shift_expression
        {
                $$ = $1 ;
        }
        | relational_expression OP_LESS_THAN shift_expression
        {
                $$.variable =  allocateCompareExpression((&$1.variable), &($3.variable), CNCompareLessThan) ;
        }
        | relational_expression OP_LESS_EQUAL shift_expression
        {
                $$.variable =  allocateCompareExpression((&$1.variable), &($3.variable), CNCompareLessEqual) ;
        }
        | relational_expression OP_GREATER_THAN shift_expression
        {
                $$.variable =  allocateCompareExpression((&$1.variable), &($3.variable), CNCompareGreaterThan) ;
        }
        | relational_expression OP_GREATE_EQUAL shift_expression
        {
                $$.variable =  allocateCompareExpression((&$1.variable), &($3.variable), CNCompareGreateEqual) ;
        }
        ;

shift_expression
        : additive_expression
        {
                $$ = $1 ;
        }
        | shift_expression OP_SHIFT_LEFT additive_expression
        {
                $$.variable = allocateBitBinaryExpression(&($1.variable), &($3.variable), CNBitShiftLeftOperation) ;
        }
        | shift_expression OP_SHIFT_RIGHT additive_expression
        {
                $$.variable = allocateBitBinaryExpression(&($1.variable), &($3.variable), CNBitShiftRightOperation) ;
        }
        ;

additive_expression
        : multiplicative_expression
        {
                $$ = $1 ;
        }
        | additive_expression '+' multiplicative_expression
        {
                $$.variable = allocateNumberBinaryExpression(&($1.variable), &($3.variable), CNAddOperation) ;
        }
        | additive_expression '-' multiplicative_expression
        {
                $$.variable = allocateNumberBinaryExpression(&($1.variable), &($3.variable), CNSubOperation) ;
        }
        ;

multiplicative_expression
        : cast_expression
        {
                $$ = $1 ;
        }
        | multiplicative_expression '*'    cast_expression
        {
                $$.variable = allocateNumberBinaryExpression(&($1.variable), &($3.variable), CNMultOperation) ;
        }
        | multiplicative_expression '/'    cast_expression
        {
                $$.variable = allocateFloatBinaryExpression(&($1.variable), &($3.variable), CNDivFloatOperation) ;
        }
        | multiplicative_expression OP_DIV cast_expression
        {
                $$.variable = allocateIntBinaryExpression(&($1.variable), &($3.variable), CNDivIntOperation) ;
        }
        | multiplicative_expression OP_MOD cast_expression
        {
                $$.variable = allocateIntBinaryExpression(&($1.variable), &($3.variable), CNModOperation) ;
        }
        ;

cast_expression
        : unary_expression
        {
                $$ = $1 ;
        }
        | '(' TYPE_IDENTIFIER ')' cast_expression
        {
                $$.variable = allocateCastExpression($2.valueType, &($4.variable)) ;
        }
        ;

unary_expression
        : postfix_expression
         {
                 $$ = $1 ;
         }
        | '+' cast_expression
         {
                 $$ = $1 ;
         }
        | '-' cast_expression
         {
                 struct CNVariable srcvar = $2.variable ;
                 if(srcvar.valueType == CNSignedIntType){
                         srcvar = allocateCastExpression(CNUnsignedIntType, &srcvar) ;
                 }
                 $$.variable = allocateNumberUnaryExpression(&srcvar, CNNegateOperation) ;
         }
        | '~' cast_expression
        {
                $$.variable = allocateBitUnaryExpression(&($2.variable), CNBitNotOperation) ;
        }
        | OP_LOG_NOT cast_expression
        {
                $$.variable = allocateLogicalUnaryExpression(&($2.variable), CNLogicalNotOperation) ;
        }
        ;

postfix_expression
        : primary_expression
        {
                $$ = $1 ;
        }
        ;

primary_expression
        : '(' expression ')'
          {
                  $$ = $2 ;
          }
        | IDENTIFIER
        {
                struct CNStringValue *  ident = $1.identifier ;
                struct CNVariable       srcvar ;
                if(CNHasRegisterIdForIdentifier(&srcvar, s_compiler, ident)){
                        $$.variable = srcvar ;
                } else {
                        unsigned int line = CNGetCurrentParsingLine() ;
                        struct CNParseError error = CNMakeUndefinedVariableError(ident, line) ;
                        CNPutParseErrorToCompiler(s_compiler, &error) ;
                        CNDeinitParseError(s_value_pool, &error) ;

                        uint64_t regid = CNAllocateFreeRegisterId(s_compiler) ;
                        $$.variable = CNMakeVariable(CNNullType, regid) ;
                }
                CNReleaseValue(s_value_pool, CNSuperClassOfStringValue(ident)) ;
        }
        | TRUE_VALUE
        {
                struct CNBooleanValue * bval ;
                bval = CNAllocateBooleanValue(s_value_pool, true) ;
                $$.variable = allocateStoreStatement(CNBooleanType, CNSuperClassOfBooleanValue(bval)) ;
        }
        | FALSE_VALUE
        {
                struct CNBooleanValue * bval ;
                bval = CNAllocateBooleanValue(s_value_pool, false) ;
                $$.variable = allocateStoreStatement(CNBooleanType, CNSuperClassOfBooleanValue(bval)) ;
        }
        | INT_VALUE
        {
                struct CNUnsignedIntValue * intval ;
                intval = CNAllocateUnsignedIntValue(s_value_pool, $1.unsignedIntValue) ;
                $$.variable = allocateStoreStatement(CNUnsignedIntType, CNSuperClassOfUnsignedIntValue(intval)) ;
        }
        | FLOAT_VALUE
        {
                struct CNFloatValue * fltval ;
                fltval = CNAllocateFloatValue(s_value_pool, $1.floatValue) ;
                $$.variable = allocateStoreStatement(CNFloatType, CNSuperClassOfFloatValue(fltval)) ;
        }
        | STRING
        {
                $$.variable = allocateStoreStatement(CNStringType, CNSuperClassOfStringValue($1.string)) ;
        }

%%

void CNStartParser(void)
{
        yyparse() ;
}

static struct CNVariable
allocateStoreStatement(CNValueType vtype, struct CNValue * src)
{
        uint64_t dstid = CNAllocateFreeRegisterId(s_compiler) ;
        struct CNCodeValue * code = CNAllocateLoadCode(s_value_pool, dstid, src) ;
        CNAppendCodeToCompiler(s_compiler, code) ;
        CNReleaseValue(s_value_pool, src) ;
        CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        return CNMakeVariable(vtype, dstid) ;
}

static struct CNVariable
allocateCastExpression(CNValueType dsttype, const struct CNVariable * src)
{
        if(src->valueType == dsttype){
                return *src ;
        }
        bool              succeeded = false ;
        struct CNVariable result    = *src ;
        struct CNVariable dummyres  = CNMakeVariable(CNBooleanType, src->registerId) ;
        switch(dsttype){
                case CNNullType: {
                        switch(src->valueType){
                                case CNNullType: {
                                        succeeded = true ;
                                        result    = *src ;      // needless to convert
                                } break ;
                                case CNStringType: {
                                        result = allocateConvertExpression(dsttype, src) ;
                                        succeeded = true ;
                                } break ;
                                case CNBooleanType:
                                case CNSignedIntType:
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNBooleanType: {
                        switch(src->valueType){
                                case CNBooleanType: {
                                        succeeded = true ;
                                        result    = *src ;      // needless to convert
                                } break ;
                                case CNStringType: {
                                        result = allocateConvertExpression(dsttype, src) ;
                                        succeeded = true ;
                                } break ;
                                case CNNullType:
                                case CNSignedIntType:
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNSignedIntType: {
                        switch(src->valueType){
                                case CNSignedIntType: {
                                        result    = *src ;      // needless to convert
                                        succeeded = true ;
                                } break ;
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNStringType: {
                                        result = allocateConvertExpression(dsttype, src) ;
                                        succeeded = true ;
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNUnsignedIntType: {
                        switch(src->valueType){
                                case CNUnsignedIntType: {
                                        result    = *src ;      // needless to convert
                                        succeeded = true ;
                                } break ;
                                case CNSignedIntType:
                                case CNFloatType:
                                case CNStringType: {
                                        result = allocateConvertExpression(dsttype, src) ;
                                        succeeded = true ;
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNFloatType: {
                        switch(src->valueType){
                                case CNFloatType: {
                                        result    = *src ;      // needless to convert
                                        succeeded = true ;
                                } break ;
                                case CNUnsignedIntType:
                                case CNSignedIntType:
                                case CNStringType: {
                                        result = allocateConvertExpression(dsttype, src) ;
                                        succeeded = true ;
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }

                } break ;
                case CNStringType: {
                        switch(src->valueType){
                                case CNStringType: {
                                        succeeded = true ;
                                        result    = *src ;      // needless to convert
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNSignedIntType:
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNArrayType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNArrayType: {
                        switch(src->valueType){
                                case CNArrayType: {
                                        succeeded = true ;
                                        result    = *src ;      // needless to convert
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNSignedIntType:
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNStringType:
                                case CNDictionaryType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNDictionaryType: {
                        switch(src->valueType){
                                case CNDictionaryType: {
                                        succeeded = true ;
                                        result    = *src ;      // needless to convert
                                } break ;
                                case CNNullType:
                                case CNBooleanType:
                                case CNSignedIntType:
                                case CNUnsignedIntType:
                                case CNFloatType:
                                case CNStringType:
                                case CNArrayType:
                                case CNCodeType: {
                                        succeeded = false ;
                                        result    = dummyres ;
                                } break ;
                        }
                } break ;
                case CNCodeType: {
                        /* always fail */
                        succeeded = false ;
                        result    = dummyres ;
                } break ;
        }
        if(!succeeded){
                unsigned int line = CNGetCurrentParsingLine() ;
                struct CNParseError error = CNMakeCanNotCastError(dsttype, src->valueType, line) ;
                CNPutParseErrorToCompiler(s_compiler, &error) ;
                CNDeinitParseError(s_value_pool, &error) ;
        }
        return result ;
}

static struct CNVariable
allocateConvertExpression(CNValueType dsttype, const struct CNVariable * src)
{
        uint64_t dstreg = CNAllocateFreeRegisterId(s_compiler) ;
        struct CNCodeValue * code = CNAllocateConvertCode(s_value_pool, dsttype, dstreg, src->valueType, src->registerId) ;
        CNAppendCodeToCompiler(s_compiler, code) ;
        CNReleaseValue(s_value_pool, CNSuperClassOfCodeValue(code)) ;
        return CNMakeVariable(dsttype, dstreg) ;
}

static bool
unionValueType(struct CNVariable * dst0, struct CNVariable * dst1,
               const struct CNVariable * src0, const struct CNVariable * src1)
{
        CNValueType utype ;
        CNValueType ltype = src0->valueType ;
        CNValueType rtype = src1->valueType ;
        if(CNUnionValueType(&utype, ltype, rtype)){
                *dst0 = allocateCastExpression(utype, src0) ;
                *dst1 = allocateCastExpression(utype, src1) ;
                return true ;
        } else {
                unsigned int line = CNGetCurrentParsingLine() ;
                struct CNParseError error = CNMakeUnmatchedTypesError(ltype, rtype, line) ;
                CNPutParseErrorToCompiler(s_compiler, &error) ;
                CNDeinitParseError(s_value_pool, &error) ;
                return false ;
        }
}

static void yyerror(const char * message)
{
        struct CNValuePool * vpool = s_compiler->valuePool ;
        struct CNStringValue * str = CNAllocateStringValue(vpool, strlen(message), message) ;

        unsigned int line = CNGetCurrentParsingLine() ;
        struct CNParseError error = CNMakeSyntaxError(str, line) ;
        CNPutParseErrorToCompiler(s_compiler, &error) ;
        CNDeinitParseError(s_value_pool, &error) ;
}

