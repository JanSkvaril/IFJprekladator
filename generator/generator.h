#ifndef GENERATOR_H_
#define GENERATOR_H_


#include "../parser/parser.h"
#include "../parser/semantics.h"
#include "../parser/token_stack.h"

#include "../scanner/scanner.h"
#include "../sym_table/sym_table.h"


#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

typedef enum
{
	BUILT_INPUTS,
	BUILT_INPUTI,
	BUILT_INPUTF,
	BUILT_PRINT,
	BUILT_INT2FLOAT,
	BUILT_FLOAT2INT,
	BUILT_LEN,
	BUILT_SUBSTR,
	BUILT_ORD,
	BUILT_CHR
} tBUILT;

void gen_code(Exp *exp);
void generator(Exp *exp);


#endif
