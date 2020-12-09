#ifndef BUILTIN_H_
#define BUILTIN_H_
/**
* NÁZEV PROJEKTU:   Implementace překladače imperativního jazyka IFJ20
* ČÍSLO TÝMU:       115
* VARIANTA:         1.
*
* Michal Zavadil    login: xzavad18
* Jakub Novotný     login: xnovot2a
* Jan Škvařil       login: xskvar09
* Erik Báča         login: xbacae00
*/
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

void built_inputx(char, char*);
void built_print(Exp*);
void built_int2float();
void built_float2int();
void built_len();
void built_substr();
void built_ord();
void built_chr();
#endif
