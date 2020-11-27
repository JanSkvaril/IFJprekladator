#include "./parser/parser.h"
#include "./generator/generator.h"

#include <stdio.h>
int main()
{
	//Parse();
	//printf("Program compiled! yay\n");


	//Exp *Exp = Parse();


	Exp *exp = Parse();
	Exp *root = NULL;
	root = exp;
	//printf("----------Generator start ----------\n");
	generator(exp, root);
	//printf("------------Generator exit!---------\n");



	return 0;
}
