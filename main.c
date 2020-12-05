#include "./parser/parser.h"
#include "./generator/generator.h"
#include "debug.h"
#include "error.h"
#include <stdio.h>
int main()
{
	//Parse();
	//Exp *Exp = Parse();
	Exp *exp = Parse();
	Exp *root = NULL;
	root = exp;
	DEBUG_PRINT(("----------Generator start ----------\n"));
	//print_token(exp->value);
	// DEBUG_PRINT(("----\n"));
	//print_token(exp->RPtr->RPtr->value);
	//print_token(exp->LPtr->value);
	//DEBUG_PRINT(("----\n"));
	//print_token(exp->RPtr->RPtr->value);
	generator(exp, root);
	DEBUG_PRINT(("------------Generator exit!---------\n"));
	return 0;
}
