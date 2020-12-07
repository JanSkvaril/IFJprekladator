#ifndef BUILTIN_H_
#define BUILTIN_H_
#include "../parser/token_stack.h"

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

void built_inputx(Exp*, char*);
void built_print(Exp*);
void built_int2float(Exp*, Exp*);
void built_float2int(Exp*, Exp*);
void built_len(Exp*, Exp*);
void built_substr(Exp*, Exp*);
void built_ord(Exp*, Exp*);
void built_chr(Exp*, Exp*);
#endif