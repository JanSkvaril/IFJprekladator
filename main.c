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
	generator(exp, root);
	DEBUG_PRINT(("------------Generator exit!---------\n"));
	return 0;
}
