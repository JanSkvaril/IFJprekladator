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
#include <string.h>
#include "builtin.h"
#include "generator.h"

void built_inputx(char c, char *type)
{
	printf("\nLABEL $built_input%c\n", c);
	printf("PUSHFRAME\n");
	printf("DEFVAR LF@?retval0\n");
	printf("DEFVAR LF@?retval1\n");
	printf("MOVE LF@?retval1 int@1\n");
	printf("READ LF@?retval0 %s\n", type);
	printf("JUMPIFEQ *end_input%c LF@?retval0 nil@nil\n", c);
	printf("MOVE LF@?retval1 int@0\n");
	printf("LABEL *end_input%c\n", c);
	printf("POPFRAME\n");
	printf("RETURN\n");
	return;
}


void built_print(Exp *params)
{

	if (params == NULL)
		return;
	else if (params->value->id == ID_COMMA)
	{
		built_print(params->RPtr);
		built_print(params->LPtr);
	}
	else
	{
		printf("WRITE ");
		switch (params->value->id)
		{
			case ID_IDENTIFIER:
				printf("LF@%s\n", params->value->att.s);
			break;
			case ID_INT_LIT:
				printf("int@%ld\n", params->value->att.i);
			break;
			case ID_FLOAT_LIT:
				printf("float@%a\n", params->value->att.d);
			break;
			case ID_STRING_LIT:
				print_string_lit(params->value->att.s);
				printf("\n");
			break;
			default:
			break;
		}
	}
	return;
}

void built_int2float()
{
	printf("\nLABEL $built_int2float\n");
	printf("PUSHFRAME\n");
	printf("DEFVAR LF@?retval0\n");
	printf("INT2FLOAT LF@?retval0 LF@?param0\n");
	printf("POPFRAME\n");
	printf("RETURN\n");
	return;
}

void built_float2int()
{
	printf("\nLABEL $built_float2int\n");
	printf("PUSHFRAME\n");
	printf("DEFVAR LF@?retval0\n");
	printf("FLOAT2INT LF@?retval0 LF@?param0\n");
	printf("POPFRAME\n");
	printf("RETURN\n");
	return;
}


void built_len()
{
	printf("\nLABEL $built_len\n");
	printf("PUSHFRAME\n");
	printf("DEFVAR LF@?retval0\n");
	printf("STRLEN LF@?retval0 LF@?param0\n");
	printf("POPFRAME\n");
	printf("RETURN\n");
	return;
}

void built_substr()
{
	printf("\nLABEL $built_substr\n");
	printf("PUSHFRAME\n");
	printf("DEFVAR LF@?retval0\n");
	printf("DEFVAR LF@?retval1\n");
	printf("DEFVAR LF@*tmp_substr\n");
	printf("DEFVAR LF@*sum_substr\n");


	printf("MOVE LF@?retval0 nil@nil\n");
	printf("MOVE LF@?retval1 int@1\n");



	printf("LT LF@*tmp_substr LF@?param1 int@0\n");
	printf("JUMPIFEQ *end_substr LF@*tmp_substr bool@true\n");

	printf("LT LF@*tmp_substr LF@?param2 int@0\n");
	printf("JUMPIFEQ *end_substr LF@*tmp_substr bool@true\n");

	printf("STRLEN LF@*tmp_substr LF@?param0\n");
	printf("ADD LF@*sum_substr LF@?param1 LF@?param2\n");
	printf("LT LF@*tmp_substr LF@*tmp_substr LF@*sum_substr\n");
	printf("JUMPIFEQ *end_substr LF@*tmp_substr bool@true\n");

	printf("MOVE LF@?retval1 int@0\n");

	//calculation
	printf("DEFVAR LF@*x_substr\n");
	printf("DEFVAR LF@*y_substr\n");
	printf("GETCHAR LF@*x_substr LF@?param0 LF@?param1\n");

	printf("SUB LF@?param2 LF@?param2 int@1\n");
	printf("JUMPIFEQ *once_substr LF@?param2 int@0\n");
	printf("LABEL *loop_substr\n");
	printf("ADD LF@?param1 LF@?param1 int@1\n");
	printf("GETCHAR LF@*y_substr LF@?param0 LF@?param1\n");
	printf("CONCAT LF@*x_substr LF@*x_substr LF@*y_substr\n");
	printf("SUB LF@?param2  LF@?param2  int@1\n");
	printf("JUMPIFNEQ *loop_substr LF@?param2  int@0\n");

	printf("LABEL *once_substr\n");
	printf("MOVE LF@?retval0 LF@*x_substr\n");

	printf("LABEL *end_substr\n");

	printf("POPFRAME\n");
	printf("RETURN\n");

	return;
}



void built_ord()
{
	printf("\nLABEL $built_ord\n");
	printf("PUSHFRAME\n");
	printf("DEFVAR LF@?retval0\n");
	printf("DEFVAR LF@?retval1\n");
	printf("DEFVAR LF@*tmp_ord\n");

	printf("MOVE LF@?retval0 nil@nil\n");
	printf("MOVE LF@?retval1 int@1\n");

	printf("LT LF@*tmp_ord LF@?param1 int@0\n");
	printf("JUMPIFEQ *end_ord LF@*tmp_ord bool@true\n");

	printf("DEFVAR LF@*len_ord\n");
	printf("STRLEN LF@*len_ord LF@?param0\n");

	printf("GT LF@*tmp_ord LF@*len_ord LF@?param1\n");
	printf("JUMPIFNEQ *end_ord LF@*tmp_ord bool@true\n");
	printf("MOVE LF@?retval1 int@0\n");

	printf("STRI2INT LF@?retval0 LF@?param0 LF@?param1\n");

	printf("LABEL *end_ord\n");
	printf("POPFRAME\n");
	printf("RETURN\n");
	return;
}



void built_chr()
{
	printf("\nLABEL $built_chr\n");
	printf("PUSHFRAME\n");
	printf("DEFVAR LF@?retval0\n");
	printf("DEFVAR LF@?retval1\n");

	printf("DEFVAR LF@*tmp_chr\n");


	printf("MOVE LF@?retval0 nil@nil\n");
	printf("MOVE LF@?retval1 int@1\n");

	printf("LT LF@*tmp_chr LF@?param0 int@0\n");
	printf("JUMPIFEQ *end_chr LF@*tmp_chr bool@true\n");

	printf("GT LF@*tmp_chr LF@?param0 int@255\n");
	printf("JUMPIFEQ *end_chr LF@*tmp_chr bool@true\n");

	printf("MOVE LF@?retval1 int@0\n");
	printf("INT2CHAR LF@?retval0 LF@?param0\n");

	printf("LABEL *end_chr\n");
	printf("POPFRAME\n");
	printf("RETURN\n");



	return;
}
