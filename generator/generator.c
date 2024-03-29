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
#include "generator.h"
#include "../debug.h"

int inFunction = 0;
int inMain = 0;
char isMain[] = "main";
Exp *id_exp = NULL;
Exp *root;

//literal counter
static int counter;
char *buffer;

// if counter
static int ifCounterArray[2024];
static int ifCounter = 0;
static int ifCounterLength = 0;

// for counter
static int forCounterArray[2024];
static int forCounter = 0;
static int forCounterLength = 0;

static int forCounter;
char *forBuffer;

// int type
bool intIdiv = false;
static int ifIdivCounter = 0;
static char *ifIdivArray[2024];
bool intTypeFound = false;

static int ifStringCounter = 0;
static char *ifStringArray[2024];
bool stringTypeFound = false;


static int counter;

typedef enum {
	CALLER_ARGS,
	CALLER_RETVALS,
	CALLEE_PARAMS,
	CALLEE_RETVALS
}tPARAM_RETVALS;

void print_string_lit (char *str)
{
	int i = 0;
	char c;
	printf("string@");
	while ((c = str[i]) != '\0')
	{
		if ((c >= 0 && c <= 32) || c == 35 || c == 92)
			printf("\\0%d", c);
		else
			printf("%c", c);
		i++;
	}
	return;
}

int built_flags[10] = {0};

void built_flags_seen(tBUILT type)
{
	built_flags[type] = 1;
	return;
}

void built_flags_print()
{
	if (built_flags[0])
		built_inputx('s', "string");
	if (built_flags[1])
		built_inputx('i', "int");
	if (built_flags[2])
		built_inputx('f', "float");
	if (built_flags[4])
		built_int2float();
	if (built_flags[5])
		built_float2int();
	if (built_flags[6])
		built_len();
	if (built_flags[7])
		built_substr();
	if (built_flags[8])
		built_ord();
	if (built_flags[9])
		built_chr();
	return;
}

//  print arithmetic operation, defining and assigning variables
void gen_code(Exp *exp) {

	int type = exp->value->id;

	switch (type) {
	case ID_ADD:

		exp->value->id = ID_IDENTIFIER;

		counter++;
		buffer = malloc(snprintf(0, 0, "var$%d", counter)+1);
		sprintf(buffer, "var$%d", counter);
		exp->value->att.s = buffer;

		printf("DEFVAR LF@%s\n", exp->value->att.s);

		type = exp->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			for (int i=0; i<ifStringCounter; i++)
			{
				char *b = ifStringArray[i];
				if (!strcmp(b, exp->RPtr->value->att.s)) {
					stringTypeFound = true;
				}
				if (!strcmp(b, exp->LPtr->value->att.s)) {
					stringTypeFound = true;
				}
			}
			if (stringTypeFound) {
				printf("CONCAT LF@%s LF@%s ", exp->value->att.s, exp->RPtr->value->att.s);
				printf("LF@%s\n", exp->LPtr->value->att.s);
			}
			else
			{
				printf("ADD LF@%s LF@%s ", exp->value->att.s, exp->LPtr->value->att.s);
			}
			break;
		case ID_INT_LIT:
			printf("ADD LF@%s int@%ld ", exp->value->att.s, exp->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("ADD LF@%s float@%a ", exp->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("CONCAT LF@%s ", exp->value->att.s);
			print_string_lit(exp->LPtr->value->att.s);
			printf(" ");
			break;
		}

		type = exp->RPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			if (!stringTypeFound) {
				printf("LF@%s\n", exp->RPtr->value->att.s);
			}
			break;
		case ID_INT_LIT:
			printf("int@%ld\n", exp->RPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("float@%a\n", exp->RPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			if (!stringTypeFound) {
				print_string_lit(exp->RPtr->value->att.s);
				printf("\n");
			}
			break;
		}

		stringTypeFound = false;
		break;

	case ID_SUB:

		counter++;
		buffer = malloc(snprintf(0, 0, "var$%d", counter)+1);
		sprintf(buffer, "var$%d", counter);

		exp->value->id = ID_IDENTIFIER;
		exp->value->att.s = buffer;

		printf("DEFVAR LF@%s\n", exp->value->att.s);

		type = exp->RPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("SUB LF@%s LF@%s ", exp->value->att.s, exp->RPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("SUB LF@%s int@%ld ", exp->value->att.s, exp->RPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("SUB LF@%s float@%a ", exp->value->att.s, exp->RPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("SUB LF@%s ", exp->value->att.s);
			print_string_lit(exp->RPtr->value->att.s);
			printf(" ");
			break;
		}

		type = exp->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("LF@%s\n", exp->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("int@%ld\n", exp->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("float@%a\n", exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			print_string_lit(exp->LPtr->value->att.s);
			printf("\n");
			break;
		}
		break;

	case ID_MULT:

		counter++;
		buffer = malloc(snprintf(0, 0, "var$%d", counter)+1);
		sprintf(buffer, "var$%d", counter);
		exp->value->id = ID_IDENTIFIER;
		exp->value->att.s = buffer;

		printf("DEFVAR LF@%s\n", exp->value->att.s);

		type = exp->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("MUL LF@%s LF@%s ", exp->value->att.s, exp->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("MUL LF@%s int@%ld ", exp->value->att.s, exp->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("MUL LF@%s float@%a ", exp->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("MUL LF@%s ", exp->value->att.s);
			print_string_lit(exp->LPtr->value->att.s);
			printf(" ");
			break;
		}

		type = exp->RPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("LF@%s\n", exp->RPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("int@%ld\n", exp->RPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("float@%a\n", exp->RPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			print_string_lit(exp->RPtr->value->att.s);
			printf("\n");
			break;
		}

		break;

	case ID_DIV:

		counter++;
		buffer = malloc(snprintf(0, 0, "var$%d", counter)+1);
		sprintf(buffer, "var$%d", counter);
		exp->value->id = ID_IDENTIFIER;
		exp->value->att.s = buffer;

		for (int i=1; i<ifIdivCounter; i++)
		{
			char *a = ifIdivArray[i];
			if (!strcmp(a, exp->RPtr->value->att.s)) {
				intTypeFound = true;
			}
			if (!strcmp(a, exp->LPtr->value->att.s)) {
				intTypeFound = true;
			}
		}

		printf("DEFVAR LF@%s\n", exp->value->att.s);

		if ((exp->RPtr->value->id == ID_FLOAT_LIT || exp->LPtr->value->id == ID_FLOAT_LIT
		     || (exp->RPtr->value->id == ID_IDENTIFIER && exp->LPtr->value->id == ID_IDENTIFIER)
		     || exp->RPtr->value->id == ID_STRING_LIT || exp->LPtr->value->id == ID_STRING_LIT) && !intTypeFound) {
			type = exp->RPtr->value->id;
			switch (type) {
			case ID_IDENTIFIER:
				//printf("TYPE LF@%s LF@%s\n", exp->value->att.s, exp->RPtr->value->att.s);
				printf("DIV LF@%s LF@%s ", exp->value->att.s, exp->RPtr->value->att.s);
				break;
			case ID_INT_LIT:
				printf("DIV LF@%s int@%ld ", exp->value->att.s, exp->RPtr->value->att.i);
				break;
			case ID_FLOAT_LIT:
				printf("DIV LF@%s float@%a ", exp->value->att.s, exp->RPtr->value->att.d);
				break;
			case ID_STRING_LIT:
				printf("DIV LF@%s ", exp->value->att.s);
				print_string_lit(exp->RPtr->value->att.s);
				printf(" ");
				break;
			}

			type = exp->LPtr->value->id;
			switch (type) {
			case ID_IDENTIFIER:
				printf("LF@%s\n", exp->LPtr->value->att.s);
				break;
			case ID_INT_LIT:
				printf("int@%ld\n", exp->LPtr->value->att.i);
				break;
			case ID_FLOAT_LIT:
				printf("float@%a\n", exp->LPtr->value->att.d);
				break;
			case ID_STRING_LIT:
				print_string_lit(exp->LPtr->value->att.s);
				printf("\n");
				break;
			}
		}
		else if (exp->RPtr->value->id == ID_INT_LIT || exp->LPtr->value->id == ID_INT_LIT || intIdiv) {
			type = exp->RPtr->value->id;
			switch (type) {
			case ID_IDENTIFIER:
				printf("IDIV LF@%s LF@%s ", exp->value->att.s, exp->RPtr->value->att.s);
				break;
			case ID_INT_LIT:
				printf("IDIV LF@%s int@%ld ", exp->value->att.s, exp->RPtr->value->att.i);
				break;
			case ID_FLOAT_LIT:
				printf("IDIV LF@%s float@%a ", exp->value->att.s, exp->RPtr->value->att.d);
				break;
			case ID_STRING_LIT:
				printf("IDIV LF@%s ", exp->value->att.s);
				print_string_lit(exp->RPtr->value->att.s);
				printf(" ");
				break;
			}

			type = exp->LPtr->value->id;
			switch (type) {
			case ID_IDENTIFIER:
				printf("LF@%s\n", exp->LPtr->value->att.s);
				break;
			case ID_INT_LIT:
				printf("int@%ld\n", exp->LPtr->value->att.i);
				break;
			case ID_FLOAT_LIT:
				printf("float@%a\n", exp->LPtr->value->att.d);
				break;
			case ID_STRING_LIT:
				print_string_lit(exp->LPtr->value->att.s);
				printf("\n");
				break;
			}
		}
		intIdiv = false;
		intTypeFound = false;

		break;

	case ID_DEFINE:

		exp->value->id = ID_IDENTIFIER;

		type = exp->RPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			exp->value->att.s = exp->RPtr->value->att.s;

			if (ifCounterArray[ifCounterLength] > 0) {
				printf("DEFVAR LF@%s$%d\n", exp->RPtr->value->att.s, ifCounterArray[ifCounterLength]);
			}
			else
			{
				printf("DEFVAR LF@%s\n", exp->RPtr->value->att.s);
			}

			break;
		}

		type = exp->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("MOVE LF@%s LF@%s\n", exp->RPtr->value->att.s, exp->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("MOVE LF@%s int@%ld\n", exp->RPtr->value->att.s, exp->LPtr->value->att.i);
			ifIdivArray[ifIdivCounter] = (exp->RPtr->value->att.s);
			ifIdivCounter++;
			intIdiv = true;
			break;
		case ID_FLOAT_LIT:
			printf("MOVE LF@%s float@%a\n", exp->RPtr->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			ifStringArray[ifStringCounter] = (exp->RPtr->value->att.s);
			ifStringCounter++;
			printf("MOVE LF@%s ", exp->RPtr->value->att.s);
			print_string_lit(exp->LPtr->value->att.s);
			printf("\n");
			break;
		}

		break;

	case ID_ASSIGN:

		exp->value->id = ID_IDENTIFIER;
		exp->value->att.s = exp->RPtr->value->att.s;
		type = exp->LPtr->value->id;

		switch (type) {
		case ID_IDENTIFIER:
			printf("MOVE LF@%s LF@%s\n", exp->RPtr->value->att.s, exp->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("MOVE LF@%s int@%ld\n", exp->RPtr->value->att.s, exp->LPtr->value->att.i);
			ifIdivArray[ifIdivCounter] = (exp->RPtr->value->att.s);
			ifIdivCounter++;
			ifStringArray[ifStringCounter] = (exp->RPtr->value->att.s);
			ifStringCounter++;
			intIdiv = true;
			break;
		case ID_FLOAT_LIT:
			printf("MOVE LF@%s float@%a\n", exp->RPtr->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("MOVE LF@%s ", exp->RPtr->value->att.s);
			print_string_lit(exp->LPtr->value->att.s);
			printf("\n");
			break;
		}

		break;

	default:
		break;
	}

}

// process expression from abstract syntax tree (move from bottom to top),
// recursive function call together with 'gen_code' function
void syntax(Exp *exp)
{
	root = exp;

	if (exp->value->id == ID_DEFINE || exp->value->id == ID_ASSIGN)
	{
		id_exp = exp;
	}

	if (exp->value->id == ID_DEFINE || exp->value->id == ID_ASSIGN || exp->value->id == ID_ADD || exp->value->id == ID_SUB || exp->value->id == ID_MULT || exp->value->id == ID_DIV)
	{

		if (((exp->RPtr->value->id == ID_IDENTIFIER  || exp->RPtr->value->id == ID_INT_LIT  || exp->RPtr->value->id == ID_FLOAT_LIT  || exp->RPtr->value->id == ID_STRING_LIT) && (exp->LPtr->value->id == ID_IDENTIFIER || exp->LPtr->value->id == ID_INT_LIT || exp->LPtr->value->id == ID_FLOAT_LIT || exp->LPtr->value->id == ID_STRING_LIT))) {
			gen_code(exp);
			exp = root;
			syntax(exp);
		}

		if (exp->RPtr->value->id == ID_ADD || exp->RPtr->value->id == ID_SUB || exp->RPtr->value->id == ID_MULT || exp->RPtr->value->id == ID_DIV) {
			int cyc = 0;

			//size of right node
			while(exp != NULL) {

				if (exp->value->id != ID_INT_LIT && exp->value->id != ID_IDENTIFIER && exp->value->id != ID_FLOAT_LIT && exp->value->id != ID_STRING_LIT) {
					exp = exp->RPtr;
					cyc++;
				}
				else{
					break;
				}
			}

			exp = root;

			//tree - move
			int cyc_node = 1;

			while (exp != NULL && cyc_node < cyc) {

				if (exp->value->id != ID_INT_LIT && exp->value->id != ID_IDENTIFIER && exp->value->id != ID_FLOAT_LIT && exp->value->id != ID_STRING_LIT) {
					exp = exp->RPtr;
					cyc_node++;
				}
				else
				{
					break;
				}
			}

			if (((exp->RPtr->value->id == ID_IDENTIFIER || exp->RPtr->value->id == ID_INT_LIT || exp->RPtr->value->id == ID_FLOAT_LIT || exp->RPtr->value->id == ID_STRING_LIT) && (exp->LPtr->value->id == ID_IDENTIFIER || exp->LPtr->value->id == ID_INT_LIT || exp->LPtr->value->id == ID_FLOAT_LIT || exp->LPtr->value->id == ID_STRING_LIT))) {
				gen_code(exp);
				exp = id_exp;
				syntax(exp);
			}
			else
			{
				syntax(exp);
			}

		}

		if (exp->LPtr->value->id == ID_ADD || exp->LPtr->value->id == ID_SUB || exp->LPtr->value->id == ID_MULT || exp->LPtr->value->id == ID_DIV) {

			int cyc = 0;

			//size of left node
			while(exp != NULL) {

				if (exp->value->id != ID_INT_LIT && exp->value->id != ID_IDENTIFIER && exp->value->id != ID_FLOAT_LIT && exp->value->id != ID_STRING_LIT) {
					exp = exp->LPtr;
					cyc++;
				}
				else{
					break;
				}
			}
			exp = root;

			//tree - move
			int cyc_node = 1;

			while (exp != NULL && cyc_node < cyc) {

				if (exp->value->id != ID_INT_LIT && exp->value->id != ID_IDENTIFIER && exp->value->id != ID_FLOAT_LIT && exp->value->id != ID_STRING_LIT) {
					exp = exp->LPtr;
					cyc_node++;
				}
				else
				{
					break;
				}
			}

			if (((exp->RPtr->value->id == ID_IDENTIFIER || exp->RPtr->value->id == ID_INT_LIT || exp->RPtr->value->id == ID_FLOAT_LIT || exp->RPtr->value->id == ID_STRING_LIT) && (exp->LPtr->value->id == ID_IDENTIFIER || exp->LPtr->value->id == ID_INT_LIT || exp->LPtr->value->id == ID_FLOAT_LIT || exp->LPtr->value->id == ID_STRING_LIT))) {
				gen_code(exp);
				exp = id_exp;
				syntax(exp);
			}
			else
			{
				syntax(exp);
			}

		}

	}
	else
	{
		//DEBUG_PRINT(("EXIT 0\n"));
	}
}

Exp *get_return_node (Exp *exp)
{
	while (exp->value->id != ID_KEY_RETURN)
		exp = exp->LPtr;

	return exp;
}

//print instructions for built-in funtions
void call_builtin(tBUILT type)
{
	printf("CALL ");
	switch(type)
	{
	case BUILT_INPUTS:
		printf("$built_inputs\n");
		break;
	case BUILT_INPUTI:
		printf("$built_inputi\n");
		break;
	case BUILT_INPUTF:
		printf("$built_inputf\n");
		break;
	case BUILT_INT2FLOAT:
		printf("$built_int2float\n");
		break;
	case BUILT_FLOAT2INT:
		printf("$built_float2int\n");
		break;
	case BUILT_LEN:
		printf("$built_len\n");
		break;
	case BUILT_SUBSTR:
		printf("$built_substr\n");
		break;
	case BUILT_ORD:
		printf("$built_ord\n");
		break;
	case BUILT_CHR:
		printf("$built_chr\n");
		break;
	default:
		break;
	}
	built_flags_seen(type);
	return;
}

//check if function call calls a built-in funtion and return its ID
int is_builtin(char *func_name)
{
	static char *table[10] = {"inputs", "inputi", "inputf", "print", "int2float", "float2int", "len", "substr", "ord", "chr"};
	for (int i = 0; i < 10; i++)
	{
		if (strcmp(func_name, table[i]) == 0)
			return i;
	}
	return -1;
}

//print instructions for every argument, put the value on temp frame
void print_arg(Exp *exp, int counter)
{
	printf("DEFVAR TF@?param%d\n", counter);
	printf("MOVE TF@?param%d ", counter);
	switch (exp->value->id)
	{
	case ID_IDENTIFIER:
		printf("LF@%s\n", exp->value->att.s);
		break;
	case ID_INT_LIT:
		printf("int@%ld\n",exp->value->att.i);
		break;
	case ID_FLOAT_LIT:
		printf("float@%a\n", exp->value->att.d);
		break;
	case ID_STRING_LIT:
		print_string_lit(exp->value->att.s);
		printf("\n");
		break;
	default:
		break;
	}
}

//print instructions for every parameter
void print_param(Exp *exp, int counter)
{
	printf("DEFVAR LF@%s\n", exp->LPtr->value->att.s);
	printf("MOVE LF@%s LF@?param%d\n",exp->LPtr->value->att.s, counter);
	return;
}

void print_def_retval(Exp *exp, int counter)
{
	printf("DEFVAR LF@?retval%d\n", counter);
	printf("MOVE LF@?retval%d ", counter);
	switch (exp->value->id)
	{
	case ID_IDENTIFIER:
		printf("LF@%s\n", exp->value->att.s);
		break;
	case ID_INT_LIT:
		printf("int@%ld\n",exp->value->att.i);
		break;
	case ID_FLOAT_LIT:
		printf("float@%a\n", exp->value->att.d);
		break;
	case ID_STRING_LIT:
		print_string_lit(exp->value->att.s);
		printf("\n");
		break;
	default:
		break;
	}
	return;
}

void print_call_retval(Exp *exp, int counter)
{
	if (exp->value->id != ID_UNDER)
		printf("MOVE LF@%s TF@?retval%d\n", exp->value->att.s, counter);
	return;
}

void proc_params_retvals(Exp *exp, int counter, tPARAM_RETVALS type)
{
	if (exp->value->id == ID_COMMA)
	{
		switch (type)
		{
		case CALLER_ARGS:
			print_arg(exp->RPtr, counter);
			break;
		case CALLER_RETVALS:
			print_call_retval(exp->RPtr, counter);
			break;
		case CALLEE_PARAMS:
			print_param(exp->RPtr, counter);
			break;
		case CALLEE_RETVALS:
			print_def_retval(exp->RPtr, counter);
			break;

		}
		counter++;
		proc_params_retvals(exp->LPtr, counter, type);
	}
	else
	{
		switch (type)
		{
		case CALLER_ARGS:
			print_arg(exp, counter);
			break;
		case CALLER_RETVALS:
			print_call_retval(exp, counter);
			break;
		case CALLEE_PARAMS:
			print_param(exp, counter);
			break;
		case CALLEE_RETVALS:
			print_def_retval(exp, counter);
			break;
		}
	}
	return;
}

//  Condition IF - process Condition and print IF header
void startIf(Exp *exp)
{
	ifCounterLength++;
	ifCounter++;
	ifCounterArray[ifCounterLength] = ifCounter;

	if (exp->Condition->value->id == ID_LESS) {
		printf("LT ");
	}
	else if (exp->Condition->value->id == ID_LESS_EQ) {
		printf("LT ");
	}
	else if (exp->Condition->value->id == ID_GREATER) {
		printf("GT ");
	}
	else if (exp->Condition->value->id == ID_GREATER_EQ) {
		printf("GT ");
	}
	else if (exp->Condition->value->id == ID_EQ) {
		printf("EQ ");
	}
	else if (exp->Condition->value->id == ID_NEQ) {
		printf("EQ ");
	}

	int type = exp->Condition->RPtr->value->id;
	switch (type) {
	case ID_IDENTIFIER:
		printf("GF@bool$x LF@%s ", exp->Condition->RPtr->value->att.s);
		break;
	case ID_INT_LIT:
		printf("GF@bool$x int@%ld ", exp->Condition->RPtr->value->att.i);
		break;
	case ID_FLOAT_LIT:
		printf("GF@bool$x float@%a ", exp->Condition->RPtr->value->att.d);
		break;
	case ID_STRING_LIT:
		printf("GF@bool$x string@%s ", exp->Condition->RPtr->value->att.s);
		break;
	}

	type = exp->Condition->LPtr->value->id;
	switch (type) {
	case ID_IDENTIFIER:
		printf("LF@%s\n", exp->Condition->LPtr->value->att.s);
		break;
	case ID_INT_LIT:
		printf("int@%ld\n", exp->Condition->LPtr->value->att.i);
		break;
	case ID_FLOAT_LIT:
		printf("float@%a\n", exp->Condition->LPtr->value->att.d);
		break;
	case ID_STRING_LIT:
		printf("string@%s\n", exp->Condition->LPtr->value->att.s);
		break;
	}

	if (exp->Condition->value->id == ID_LESS_EQ || exp->Condition->value->id == ID_GREATER_EQ) {
		printf("EQ ");

		type = exp->Condition->RPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("GF@bool$x2 LF@%s ",exp->Condition->RPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("GF@bool$x2 int@%ld ",exp->Condition->RPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("GF@bool$x2 float@%a ",exp->Condition->RPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("GF@bool$x2 string@%s ",exp->Condition->RPtr->value->att.s);
			break;
		}

		type = exp->Condition->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("LF@%s\n", exp->Condition->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("int@%ld\n", exp->Condition->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("float@%a\n", exp->Condition->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("string@%s\n", exp->Condition->LPtr->value->att.s);
			break;
		}

		printf("OR GF@bool$x GF@bool$x GF@bool$x2\n");
	}

	if (exp->Condition->value->id == ID_NEQ) {
		printf("NOT GF@bool$x GF@bool$x\n");
	}

	printf("JUMPIFNEQ else$if$%d GF@bool$x bool@true\n", ifCounterArray[ifCounterLength]);

	DEBUG_PRINT(("------ if body -----\n"));
	// print if body
}

// print else
void elseIf()
{
	printf("JUMP end$if$%d\n", ifCounterArray[ifCounterLength]);
	printf("LABEL else$if$%d\n", ifCounterArray[ifCounterLength]);
	DEBUG_PRINT(("--- else body ----\n"));
	// print else body

}

// print end if
void endIf()
{
	printf("LABEL end$if$%d\n", ifCounterArray[ifCounterLength]);
	ifCounterLength--;
}

// print FOR loop header
void startFor(Exp *exp)
{
	forCounterLength++;
	forCounter++;
	forCounterArray[forCounterLength] = forCounter;
	int typeFor;

	//definition - check expression exist, if yes, generate code for exp
	if (exp->RPtr->RPtr->value->id == ID_SEMICOLLON) {
		/* nothing */
	}
	else if (exp->RPtr->RPtr->value->id == ID_ASSIGN || exp->RPtr->RPtr->value->id == ID_DEFINE) {
		syntax(exp->RPtr->RPtr);
	}

	// FOR expression process
	printf("LABEL start$for$%d\n", forCounterArray[forCounterLength]);

	if (exp->RPtr->LPtr->RPtr->value->id == ID_LESS) {
		printf("LT ");
	}
	else if (exp->RPtr->LPtr->RPtr->value->id == ID_LESS_EQ) {
		printf("LT ");
	}
	else if (exp->RPtr->LPtr->RPtr->value->id == ID_GREATER) {
		printf("GT ");
	}
	else if (exp->RPtr->LPtr->RPtr->value->id == ID_GREATER_EQ) {
		printf("GT ");
	}
	else if (exp->RPtr->LPtr->RPtr->value->id == ID_EQ) {
		printf("EQ ");
	}
	else if (exp->RPtr->LPtr->RPtr->value->id == ID_NEQ) {
		printf("EQ ");
	}

	typeFor = exp->RPtr->LPtr->RPtr->RPtr->value->id;
	switch (typeFor) {
	case ID_IDENTIFIER:
		printf("GF@bool$x LF@%s ",exp->RPtr->LPtr->RPtr->RPtr->value->att.s);
		break;
	case ID_INT_LIT:
		printf("GF@bool$x int@%ld ",exp->RPtr->LPtr->RPtr->RPtr->value->att.i);
		break;
	case ID_FLOAT_LIT:
		printf("GF@bool$x float@%a ",exp->RPtr->LPtr->RPtr->RPtr->value->att.d);
		break;
	case ID_STRING_LIT:
		printf("GF@bool$x string@%s ",exp->RPtr->LPtr->RPtr->RPtr->value->att.s);
		break;
	}

	typeFor = exp->RPtr->LPtr->RPtr->LPtr->value->id;
	switch (typeFor) {
	case ID_IDENTIFIER:
		printf("LF@%s\n", exp->RPtr->LPtr->RPtr->LPtr->value->att.s);
		break;
	case ID_INT_LIT:
		printf("int@%ld\n", exp->RPtr->LPtr->RPtr->LPtr->value->att.i);
		break;
	case ID_FLOAT_LIT:
		printf("float@%a\n", exp->RPtr->LPtr->RPtr->LPtr->value->att.d);
		break;
	case ID_STRING_LIT:
		printf("string@%s\n", exp->RPtr->LPtr->RPtr->LPtr->value->att.s);
		break;
	}


	if (exp->RPtr->LPtr->RPtr->value->id == ID_LESS_EQ || exp->RPtr->LPtr->RPtr->value->id == ID_GREATER_EQ) {
		printf("EQ ");

		typeFor = exp->RPtr->LPtr->RPtr->RPtr->value->id;
		switch (typeFor) {
		case ID_IDENTIFIER:
			printf("GF@bool$x2 LF@%s ",exp->RPtr->LPtr->RPtr->RPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("GF@bool$x2 int@%ld ",exp->RPtr->LPtr->RPtr->RPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("GF@bool$x2 float@%a ",exp->RPtr->LPtr->RPtr->RPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("GF@bool$x2 string@%s ",exp->RPtr->LPtr->RPtr->RPtr->value->att.s);
			break;
		}

		typeFor = exp->RPtr->LPtr->RPtr->LPtr->value->id;
		switch (typeFor) {
		case ID_IDENTIFIER:
			printf("LF@%s\n", exp->RPtr->LPtr->RPtr->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("int@%ld\n", exp->RPtr->LPtr->RPtr->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("float@%a\n", exp->RPtr->LPtr->RPtr->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("string@%s\n", exp->RPtr->LPtr->RPtr->LPtr->value->att.s);
			break;
		}

		printf("OR GF@bool$x GF@bool$x GF@bool$x2\n");
	}

	if (exp->RPtr->LPtr->RPtr->value->id == ID_NEQ) {
		printf("NOT GF@bool$x GF@bool$x\n");
	}

	printf("JUMPIFNEQ end$for$%d GF@bool$x bool@true\n", forCounterArray[forCounterLength]);
	DEBUG_PRINT(("------ for body -----\n"));
}

// print end of FOR loop
void endFor(Exp *exp)
{
	int typeForEnd;

	//command of assign - iteration changed - check expression exist, if yes, generate code for exp
	if (exp->RPtr->LPtr->LPtr->value->id == ID_SEMICOLLON) {
		/* nothing */
	}
	else if (exp->RPtr->LPtr->LPtr->value->id == ID_ASSIGN
	         || exp->RPtr->LPtr->LPtr->value->id == ID_DEFINE) {


		if (exp->RPtr->LPtr->LPtr->LPtr->value->id == ID_ADD) {
			typeForEnd = exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->id;
			switch (typeForEnd) {
			case ID_IDENTIFIER:
				printf("ADD LF@%s LF@%s ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.s);
				break;
			case ID_INT_LIT:
				printf("ADD LF@%s int@%ld ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.i);
				break;
			case ID_FLOAT_LIT:
				printf("ADD LF@%s float@%a ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.d);
				break;
			case ID_STRING_LIT:
				printf("ADD LF@%s string@%s ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.s);
				break;
			}

			typeForEnd = exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->id;
			switch (typeForEnd) {
			case ID_IDENTIFIER:
				printf("LF@%s\n", exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.s);
				break;
			case ID_INT_LIT:
				printf("int@%ld\n", exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.i);
				break;
			case ID_FLOAT_LIT:
				printf("float@%a\n", exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.d);
				break;
			case ID_STRING_LIT:
				printf("string@%s\n", exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.s);
				break;
			}


		}
		else if (exp->RPtr->LPtr->LPtr->LPtr->value->id == ID_SUB) {
			typeForEnd = exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->id;
			switch (typeForEnd) {
			case ID_IDENTIFIER:
				printf("SUB LF@%s LF@%s ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.s);
				break;
			case ID_INT_LIT:
				printf("SUB LF@%s int@%ld ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.i);
				break;
			case ID_FLOAT_LIT:
				printf("SUB LF@%s float@%a ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.d);
				break;
			case ID_STRING_LIT:
				printf("SUB LF@%s string@%s ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.s);
				break;
			}

			typeForEnd = exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->id;
			switch (typeForEnd) {
			case ID_IDENTIFIER:
				printf("LF@%s\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.s);
				break;
			case ID_INT_LIT:
				printf("int@%ld\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.i);
				break;
			case ID_FLOAT_LIT:
				printf("float@%a\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.d);
				break;
			case ID_STRING_LIT:
				printf("string@%s\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.s);
				break;
			}

		}
		else if (exp->RPtr->LPtr->LPtr->LPtr->value->id == ID_DIV) {

			if (exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->id == ID_FLOAT_LIT
			    || exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->id == ID_FLOAT_LIT
			    || exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->id == ID_IDENTIFIER
			    || exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->id == ID_IDENTIFIER
			    || exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->id == ID_STRING_LIT
			    || exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->id == ID_STRING_LIT) {

				typeForEnd = exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->id;
				switch (typeForEnd) {
				case ID_IDENTIFIER:
					printf("DIV LF@%s LF@%s ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
					       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.s);
					break;
				case ID_INT_LIT:
					printf("DIV LF@%s int@%ld ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
					       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.i);
					break;
				case ID_FLOAT_LIT:
					printf("DIV LF@%s float@%a ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
					       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.d);
					break;
				case ID_STRING_LIT:
					printf("DIV LF@%s string@%s ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
					       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.s);
					break;
				}

				typeForEnd = exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->id;
				switch (typeForEnd) {
				case ID_IDENTIFIER:
					printf("LF@%s\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.s);
					break;
				case ID_INT_LIT:
					printf("int@%ld\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.i);
					break;
				case ID_FLOAT_LIT:
					printf("float@%a\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.d);
					break;
				case ID_STRING_LIT:
					printf("string@%s\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.s);
					break;
				}
			}
			else if (exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->id == ID_INT_LIT
			         || exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->id == ID_INT_LIT) {


				typeForEnd = exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->id;
				switch (typeForEnd) {
				case ID_IDENTIFIER:
					printf("IDIV LF@%s LF@%s ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
					       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.s);
					break;
				case ID_INT_LIT:
					printf("IDIV LF@%s int@%ld ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
					       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.i);
					break;
				case ID_FLOAT_LIT:
					printf("IDIV LF@%s float@%a ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
					       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.d);
					break;
				case ID_STRING_LIT:
					printf("IDIV LF@%s string@%s ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
					       exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.s);
					break;
				}

				typeForEnd = exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->id;
				switch (typeForEnd) {
				case ID_IDENTIFIER:
					printf("LF@%s\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.s);
					break;
				case ID_INT_LIT:
					printf("int@%ld\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.i);
					break;
				case ID_FLOAT_LIT:
					printf("float@%a\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.d);
					break;
				case ID_STRING_LIT:
					printf("string@%s\n", exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.s);
					break;
				}
			}
		}
		else if (exp->RPtr->LPtr->LPtr->LPtr->value->id == ID_MULT) {

			typeForEnd = exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->id;
			switch (typeForEnd) {
			case ID_IDENTIFIER:
				printf("MUL LF@%s LF@%s ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.s);
				break;
			case ID_INT_LIT:
				printf("MUL LF@%s int@%ld ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.i);
				break;
			case ID_FLOAT_LIT:
				printf("MUL LF@%s float@%a ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.d);
				break;
			case ID_STRING_LIT:
				printf("MUL LF@%s string@%s ", exp->RPtr->LPtr->LPtr->RPtr->value->att.s,
				       exp->RPtr->LPtr->LPtr->LPtr->LPtr->value->att.s);
				break;
			}

			typeForEnd = exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->id;
			switch (typeForEnd) {
			case ID_IDENTIFIER:
				printf("LF@%s\n", exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.s);
				break;
			case ID_INT_LIT:
				printf("int@%ld\n", exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.i);
				break;
			case ID_FLOAT_LIT:
				printf("float@%a\n", exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.d);
				break;
			case ID_STRING_LIT:
				printf("string@%s\n", exp->RPtr->LPtr->LPtr->LPtr->RPtr->value->att.s);
				break;
			}
		}
	}
	//------------------------------------
	printf("JUMP start$for$%d\n", forCounterArray[forCounterLength]);
	printf("LABEL end$for$%d\n", forCounterArray[forCounterLength]);
	forCounterLength--;
	DEBUG_PRINT(("------ end for -----\n"));
}

//process a function definition
void proc_func(Exp *exp)
{
	if (exp != NULL)
	{

		//is function call
		if (exp->value->id == ID_FUNC_CALL ||
		    (exp->value->id == ID_ASSIGN && exp->LPtr->value->id == ID_FUNC_CALL))
		{
			Exp *retvals = NULL;
			if (exp->value->id == ID_ASSIGN)
			{
				retvals = exp->RPtr;
				exp = exp->LPtr;
			}

			//is user function
			printf("CREATEFRAME\n"); //zbytecne kdyz nema parametry
			if (exp->RPtr != NULL) //has parameters
				proc_params_retvals(exp->RPtr, 0, CALLER_ARGS);

			int builtin_func;
			//is built-in
			if ((builtin_func = is_builtin(exp->LPtr->value->att.s)) != -1)
			{
				if (builtin_func == BUILT_PRINT)
					built_print(exp->RPtr);
				else
					call_builtin(builtin_func);
			}
			else
				printf("CALL $%s\n", exp->LPtr->value->att.s);
			if (retvals != NULL)
			{
				proc_params_retvals(retvals, 0, CALLER_RETVALS);
			}

			return;
		}
		//  is expression (DEFINE or ASSIGN)
		if (exp->value->id == ID_DEFINE || exp->value->id == ID_ASSIGN) {
			syntax(exp);
			return;
		}

		// is if
		if (exp->value->id == ID_KEY_IF) {
			startIf(exp);
			if (exp->LPtr->value != NULL) {
				proc_func(exp->LPtr);
			}
			elseIf();
			if (exp->RPtr->value != NULL) {
				proc_func(exp->RPtr);
			}
			endIf();
			return;
		}
		// is for
		if (exp->value->id == ID_KEY_FOR) {
			startFor(exp);
			if (exp->LPtr->value != NULL) {
				proc_func(exp->LPtr);
			}
			endFor(exp);
			return;
		}

		if (exp->value->id == ID_KEY_RETURN)
			return;
		//toto asi nemusi prochazet uplne vsechno, ale zatim to funguje
		proc_func(exp->RPtr);
		proc_func(exp->Condition);
		proc_func(exp->LPtr);
	}
}

//recursevily called for every ";" node, calls functions to process all function definitions
void generator(Exp *exp)
{
	//other than main
	//create label, manage frame, add instructions for eventual parameters and return values
	if (exp->value->id == ID_SEMICOLLON) {
		printf("\nLABEL $%s\n", exp->RPtr->LPtr->value->att.s);
		printf("PUSHFRAME\n");
		//parameters
		if (exp->RPtr->Condition->LPtr != NULL)
			proc_params_retvals(exp->RPtr->Condition->LPtr, 0, CALLEE_PARAMS);
		//function body
		proc_func(exp->RPtr);
		//return values
		if (exp->RPtr->Condition->RPtr != NULL)
		{
			Exp *returnNode = get_return_node(exp->RPtr->RPtr);
			proc_params_retvals(returnNode->LPtr, 0, CALLEE_RETVALS);
		}
		if (exp->LPtr != NULL)
			exp = exp->LPtr;
		printf("POPFRAME\n");
		printf("RETURN\n");
		generator(exp);
	}

	//is main
	else if (exp->value->id == ID_KEY_FUNC)
	{
		printf("\nLABEL $%s\n", exp->LPtr->value->att.s);
		printf("CREATEFRAME\n");
		printf("PUSHFRAME\n");
		proc_func(exp->RPtr);
		printf("POPFRAME\n");
		printf("JUMP $end_program\n");
		built_flags_print();
		printf("\nLABEL $end_program\n");
	}
	return;
}
