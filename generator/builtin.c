#include "builtin.h"

//funguje jen pro jeden parametr v print
//TODO
void built_print(Exp *exp)
{
	printf("WRITE LF@%s\n", exp->RPtr->value->att.s);
	return;
}

void built_int2float(Exp *exp, Exp *retvals)
{
	printf("INT2FLOAT LF@%s ", retvals->value->att.s);
	if (exp->RPtr->value->id == ID_IDENTIFIER)
		printf("LF@%s\n", exp->RPtr->value->att.s);
	else
		printf("int@%a\n", exp->RPtr->value->att.i);
}

void built_float2int(Exp *exp, Exp *retvals)
{
	printf("FLOAT2INT LF@%s ", retvals->value->att.s);
	if (exp->RPtr->value->id == ID_IDENTIFIER)
		printf("LF@%s\n", exp->RPtr->value->att.s);
	else
		printf("float@%a\n", exp->RPtr->value->att.d);
}