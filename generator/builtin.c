#include "builtin.h"

//funguje jen pro jeden parametr v print
//TODO
void built_print(Exp *exp)
{
	if (exp->value->id == ID_COMMA)
	{
		built_print(exp->RPtr);
		built_print(exp->LPtr);
	}
	else
		printf("WRITE LF@%s\n", exp->value->att.s);
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