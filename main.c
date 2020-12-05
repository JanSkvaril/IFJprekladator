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
<<<<<<< Updated upstream
	//printf("----------Generator start ----------\n");
=======
	DEBUG_PRINT(("----------Generator start ----------\n"));
	//print_token(exp->value);
	// DEBUG_PRINT(("----\n"));
	//print_token(exp->RPtr->RPtr->value);
	//print_token(exp->LPtr->value);
	//DEBUG_PRINT(("----\n"));
	//print_token(exp->RPtr->RPtr->value);
>>>>>>> Stashed changes
	generator(exp, root);
	//printf("------------Generator exit!---------\n");



	return 0;
}
