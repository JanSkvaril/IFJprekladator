#include "builtin.h"
#include "generator.h"

void built_inputs(Exp *retvals)
{
	printf("READ LF@%s string", retvals->RPtr->value->att.s);
	//printf("TYPE ")
	return;
}

void built_inputi(Exp *retvals)
{
	printf("READ LF@%s int", retvals->RPtr->value->att.s);
	return;
}

void built_inputf(Exp *retvals)
{
	printf("READ LF@%s float", retvals->RPtr->value->att.s);
	return;
}


void built_print(Exp *exp)
{
	if (exp->value->id == ID_COMMA)
	{
		built_print(exp->RPtr);
		built_print(exp->LPtr);
	}
	else
	{
		printf("WRITE ");
		switch (exp->value->id)
		{
			case ID_IDENTIFIER:
				printf("LF@%s\n", exp->value->att.s);
			break;
			case ID_INT_LIT:
				printf("int@%ld\n", exp->value->att.i);
			break;
			case ID_FLOAT_LIT:
				printf("LF@%f\n", exp->value->att.d);
			break;
			case ID_STRING_LIT:
				print_string_lit(exp->value->att.s);
			break;
			default:
			break;
		}
	}
	return;
}

void built_int2float(Exp *exp, Exp *retvals)
{
	printf("INT2FLOAT LF@%s ", retvals->value->att.s);
	if (exp->value->id == ID_IDENTIFIER)
		printf("LF@%s\n", exp->value->att.s);
	else
		printf("int@%ld\n", exp->value->att.i);
}

void built_float2int(Exp *exp, Exp *retvals)
{
	printf("FLOAT2INT LF@%s ", retvals->value->att.s);
	if (exp->value->id == ID_IDENTIFIER)
		printf("LF@%s\n", exp->value->att.s);
	else
		printf("float@%a\n", exp->value->att.d);
}

void built_len(Exp *exp, Exp *retvals)
{
	printf("STRLEN LF@%s ", retvals->value->att.s);
	if (exp->value->id == ID_IDENTIFIER)
		printf("LF@%s\n", exp->value->att.s);
	else
		printf("string@%s\n", exp->value->att.s);
}