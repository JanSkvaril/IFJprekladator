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


void gen_code(tTokenPtr, Exp *exp);
void generator(Exp *exp, Exp *root);


#endif
