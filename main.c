#include "./parser/parser.h"
#include "./generator/generator.h"
#include "debug.h"
#include "error.h"
#include <stdio.h>

void print_tree(Exp *exp, int spaces_num)
{
	print_token(exp->value);
	spaces_num++;
	if (exp->Condition != NULL)
	{
		for (int i = 0; i < spaces_num; i++)
			printf("      ");
		printf("Cond->  ");
		print_tree(exp->Condition, spaces_num);
	}
	if (exp->RPtr != NULL)
	{
		for (int i = 0; i < spaces_num; i++)
			printf("      ");
		printf("R->  ");
		print_tree(exp->RPtr, spaces_num);
	}
	if (exp->LPtr != NULL)
	{
		for (int i = 0; i < spaces_num; i++)
			printf("      ");
		printf("L->  ");
		print_tree(exp->LPtr, spaces_num);
	}
	return;
}

int main()
{
	//Parse();
	//Exp *Exp = Parse();
	Exp *exp = Parse();
	Exp *root = NULL;
	root = exp;

	#ifdef DEBUG
	int spaces_num = 0;
	print_tree(exp, spaces_num);
	#endif

	DEBUG_PRINT(("----------Generator start ----------\n"));
	// print_token(exp->value);
	// // DEBUG_PRINT(("----\n"));
	// print_token(exp->LPtr->value);
	// print_token(exp->RPtr->value);
	// DEBUG_PRINT(("---------- if true----------\n"));
	// 	print_token(exp->Condition->value);
	// print_token(exp->Condition->LPtr->value);
	// 	print_token(exp->Condition->RPtr->value);
	//DEBUG_PRINT(("----\n"));
	//print_token(exp->RPtr->RPtr->value);
	generator(exp->LPtr, root);
	DEBUG_PRINT(("------------Generator exit!---------\n"));
	return 0;
}
