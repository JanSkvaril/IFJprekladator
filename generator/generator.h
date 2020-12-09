#ifndef GENERATOR_H_
#define GENERATOR_H_
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


#include "../parser/parser.h"
#include "../parser/semantics.h"
#include "../parser/token_stack.h"

#include "../scanner/scanner.h"
#include "../sym_table/sym_table.h"
#include "builtin.h"


#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


void print_string_lit(char*);
void startIf(Exp *exp);
void elseIf();
void endIf();
void startFor(Exp *exp);
void endFor(Exp *exp);
void gen_code(Exp *exp);
void generator(Exp *exp);


#endif
