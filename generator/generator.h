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

void print_string_lit(char*);
void startIf(Exp *exp);
void elseIf();
void endIf();
void startFor(Exp *exp);
void endFor(Exp *exp);
void gen_code(Exp *exp);
void generator(Exp *exp);


#endif
