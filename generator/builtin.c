#include <string.h>
#include "builtin.h"
#include "generator.h"

static int built_counter = 0;

void built_inputx(Exp *retvals, char *type)
{
	tTokenPtr retval0 = retvals->RPtr->value;
	tTokenPtr retval1 = retvals->LPtr->value;

	print_token(retval1);
	if(retval1->id != ID_UNDER)
		printf("MOVE LF@%s int@1\n", retval1->att.s);

	printf("READ LF@%s %s\n", retval0->att.s, type);
	printf("JUMPIFEQ *end%d LF@%s nil@nil\n", built_counter, retval0->att.s);
	
	if(retval1->id != ID_UNDER)
		printf("MOVE LF@%s int@0\n", retval1->att.s);

	printf("LABEL *end%d\n", built_counter);

	built_counter++;
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

void built_int2float(Exp *params, Exp *retvals)
{
	printf("INT2FLOAT LF@%s ", retvals->value->att.s);
	if (params->value->id == ID_IDENTIFIER)
		printf("LF@%s\n", params->value->att.s);
	else
		printf("int@%ld\n", params->value->att.i);

	return;
}

void built_float2int(Exp *params, Exp *retvals)
{
	printf("FLOAT2INT LF@%s ", retvals->value->att.s);
	if (params->value->id == ID_IDENTIFIER)
		printf("LF@%s\n", params->value->att.s);
	else
		printf("float@%a\n", params->value->att.d);
	
	return;
}

void built_len(Exp *params, Exp *retvals)
{
	printf("STRLEN LF@%s ", retvals->value->att.s);
	if (params->value->id == ID_IDENTIFIER)
		printf("LF@%s\n", params->value->att.s);
	else
		printf("string@%s\n", params->value->att.s);

	return;
}

void built_substr(Exp *params, Exp *retvals)
{
	
	return;
}



void built_ord(Exp *params, Exp *retvals)
{
	tTokenPtr retval0 = retvals->RPtr->value;
	tTokenPtr retval1 = retvals->LPtr->value;
	tTokenPtr param0 = params->RPtr->value;
	tTokenPtr param1 = params->LPtr->value;

	if(retval1->id != ID_UNDER)
		printf("MOVE LF@%s int@1\n", retval1->att.s);
	
	printf("DEFVAR LF@*len%d\n", built_counter);
	printf("STRLEN LF@*len%d ", built_counter);
	if (param0->id == ID_IDENTIFIER)
		printf("LF@%s\n", param0->att.s);
	else
	{
		print_string_lit(param0->att.s);
		printf("\n");
	}

	printf("DEFVAR LF@*tmp%d\n", built_counter);
	printf("GT LF@*tmp%d LF@*len%d ", built_counter, built_counter);
	if (param1->id == ID_IDENTIFIER)
		printf("LF@%s\n", param1->att.s);
	else
		printf("int@%ld\n", param1->att.i);

	printf("JUMPIFNEQ *end%d LF@*tmp%d bool@true\n", built_counter, built_counter);
	
	if(retval1->id != ID_UNDER)
		printf("MOVE LF@%s int@0\n", retval1->att.s);

	printf("STRI2INT LF@%s ", retval0->att.s);
	if (param0->id == ID_IDENTIFIER)
		printf("LF@%s ", param0->att.s);
	else
	{
		print_string_lit(param0->att.s);
		printf(" ");
	}
	if (param1->id == ID_IDENTIFIER)
		printf("LF@%s\n", param1->att.s);
	else
		printf("int@%ld\n", param1->att.i);

	printf("LABEL *end%d\n", built_counter);
	return;
}



void built_chr(Exp *params, Exp *retvals)
{
	tTokenPtr retval0 = retvals->RPtr->value;
	tTokenPtr retval1 = retvals->LPtr->value;
	tAttPtr param = params->value->att;

	if(retval1->id != ID_UNDER)
		printf("MOVE LF@%s int@1\n", retval1->att.s);

	printf("DEFVAR LF@*tmp%d\n", built_counter);

	printf("GT LF@*tmp%d ", built_counter);
	if (params->value->id == ID_IDENTIFIER)
		printf("LF@%s int@255\n", param.s);
	else
		printf("int@%ld int@255\n", param.i);

	printf("JUMPIFEQ *end%d LF@*tmp%d bool@true\n", built_counter, built_counter);
	
	if(retval1->id != ID_UNDER)
		printf("MOVE LF@%s int@0\n", retval1->att.s);

	printf("INT2CHAR LF@%s ", retval0->att.s);

	if (params->value->id == ID_IDENTIFIER)
		printf("LF@%s\n", param.s);
	else
		printf("int@%ld\n", param.i);

	printf("LABEL *end%d\n", built_counter);

	built_counter++;
	return;
}	