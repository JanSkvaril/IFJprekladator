#include "generator.h"
#include "../debug.h"
#include "builtin.h"

int inFunction = 0;
int inMain = 0;
char isMain[] = "main";
Exp *id_exp = NULL;
Exp *root;

//literal name
static int counter;
char *buffer;

// char* literalType(tTokenPtr token, Exp *exp)
// {
//   char *a;
//   int type = token->id;
//   char str[128];
//
//   //exp = exp->value
//   //int type = exp->id;
//   //print_token(exp->id);
// DEBUG_PRINT(("---------- here here ----------\n"));
// //print_token(type);
// //printf("%d\n", token->att.i);
// //print_token(exp->id);
//   switch (type) {
//   case ID_IDENTIFIER:
//
//     return snprintf((char *)token->att.s, 128, (char *)"LF@%s");
//     break;
//   case ID_INT_LIT:
//     return("int@%ld", (char *)token->att.i);
//     break;
//   case ID_FLOAT_LIT:
//     //return (char *)("float@%.16lf", exp->att.d);
//     break;
//   case ID_STRING_LIT:
//     return("string@%s", token->att.s);
//     break;
//
//   default:
//   	break;
//   }
// }

void gen_code(Exp *exp) {

	int type = exp->value->id;
	char *a;

	switch (type) {
	case ID_ADD:

		exp->value->id = ID_IDENTIFIER;

		counter++;
		buffer = malloc(snprintf(0, 0, "var$%d", counter)+1);
		sprintf(buffer, "var$%d", counter);
		exp->value->att.s = buffer;

		//a = is_token_add(exp->value, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
		printf("DEFVAR LF@%s\n", exp->value->att.s);


		type = exp->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("ADD LF@%s LF@%s ", exp->value->att.s, exp->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("ADD LF@%s int@%ld ", exp->value->att.s, exp->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("ADD LF@%s float@%.16lf ", exp->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("ADD LF@%s string@%s ", exp->value->att.s, exp->LPtr->value->att.s);
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
			printf("float@%.16lf\n", exp->RPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("string@%s\n", exp->RPtr->value->att.s);
			break;
		}

		//printf("ADD GF@%s GF@%s GF@%s\n", a, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
		//printf("ADD\n");
		break;

	case ID_SUB:

		counter++;
		buffer = malloc(snprintf(0, 0, "var$%d", counter)+1);
		sprintf(buffer, "var$%d", counter);

		exp->value->id = ID_IDENTIFIER;
		exp->value->att.s = buffer;
		//a = is_token_add(exp->value, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
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
			printf("SUB LF@%s float@%.16lf ", exp->value->att.s, exp->RPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("SUB LF@%s string@%s ", exp->value->att.s, exp->RPtr->value->att.s);
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
			printf("float@%.16lf\n", exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("string@%s\n", exp->LPtr->value->att.s);
			break;
		}

		//printf("SUB GF@%s GF@%s GF@%s\n", a, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
		//printf("SUB\n");
		break;

	case ID_MULT:
		counter++;
		buffer = malloc(snprintf(0, 0, "var$%d", counter)+1);
		sprintf(buffer, "var$%d", counter);
		exp->value->id = ID_IDENTIFIER;
		exp->value->att.s = buffer;
		//a = is_token_add(exp->value, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
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
			printf("MUL LF@%s float@%.16lf ", exp->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("MUL LF@%s string@%s ", exp->value->att.s, exp->LPtr->value->att.s);
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
			printf("float@%.16lf\n", exp->RPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("string@%s\n", exp->RPtr->value->att.s);
			break;
		}


		//printf("MUL GF@%s %s %s\n", exp->value->att.s, literalType(exp->LPtr->value, exp), literalType(exp->RPtr->value, exp));

		//printf("MULT\n");
		//exit(0);
		break;

	case ID_DIV:
		counter++;
		buffer = malloc(snprintf(0, 0, "var$%d", counter)+1);
		sprintf(buffer, "var$%d", counter);
		exp->value->id = ID_IDENTIFIER;
		exp->value->att.s = buffer;
		//a = is_token_add(exp->value, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
		printf("DEFVAR LF@%s\n", exp->value->att.s);


		type = exp->RPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("DIV LF@%s LF@%s ", exp->value->att.s, exp->RPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("DIV LF@%s int@%ld ", exp->value->att.s, exp->RPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("DIV LF@%s float@%.16lf ", exp->value->att.s, exp->RPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("DIV LF@%s string@%s ", exp->value->att.s, exp->RPtr->value->att.s);
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
			printf("float@%.16lf\n", exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("string@%s\n", exp->LPtr->value->att.s);
			break;
		}

		//printf("DIV GF@%s GF@%s GF@%s\n", a, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
		//printf("DIV\n");
		break;

	case ID_DEFINE:

		//a = is_token_add(exp->value, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
		exp->value->id = ID_IDENTIFIER;

		type = exp->RPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			exp->value->att.s = exp->RPtr->value->att.s;
			printf("DEFVAR LF@%s\n", exp->RPtr->value->att.s);
			break;
		}


		type = exp->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("MOVE LF@%s LF@%s\n", exp->RPtr->value->att.s, exp->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("MOVE LF@%s int@%ld\n", exp->RPtr->value->att.s, exp->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("MOVE LF@%s float@%.16lf\n", exp->RPtr->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("MOVE LF@%s string@%s\n", exp->RPtr->value->att.s, exp->LPtr->value->att.s);
			break;
		}

		//printf("MOVE GF@%s GF@%s\n", a, exp->LPtr->value->att.s);
		//printf("WRITE %s\n", a);
		//printf("MOVE\n");
		break;

	case ID_ASSIGN:
		//a = is_token_add(exp->value, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
		//printf("DEFVAR GF@%s\n", a);
		//printf("MOVE GF@%s GF@%s\n", a, exp->LPtr->value->att.s);

		exp->value->id = ID_IDENTIFIER;
		exp->value->att.s = exp->RPtr->value->att.s;
		type = exp->LPtr->value->id;

		switch (type) {
		case ID_IDENTIFIER:
			printf("MOVE LF@%s LF@%s\n", exp->RPtr->value->att.s, exp->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("MOVE LF@%s int@%ld\n", exp->RPtr->value->att.s, exp->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("MOVE LF@%s float@%.16lf\n", exp->RPtr->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("MOVE LF@%s string@%s\n", exp->RPtr->value->att.s, exp->LPtr->value->att.s);
			break;
		}


		//printf("WRITE %s\n", a);
		//printf("MOVE\n");
		break;

	case ID_INT_LIT:
		a = is_token_add(exp->value, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
		printf("DEFVAR LF@%s\n", a);

		printf("MOVE LF@%s LF@%s\n", a, exp->LPtr->value->att.s);

		printf("WRITE %s\n", a);
		//printf("MOVE\n");
		break;

	case ID_KEY_IF:

		//todo

		printf("JUMPIFEQ else%s LF@b LF@b\n", is_token_lit(exp->value));

		printf("JUMP if&end%s\n", is_token_lit(exp->RPtr->value));
		printf("LABEL else%s\n", is_token_lit(exp->RPtr->value));

		printf("LABEL if&end%s\n", is_token_lit(exp->value));

		//printf("MOVE\n");
		break;

	case ID_KEY_FOR:

		//todo

		//printf("MOVE\n");
		break;

	case ID_KEY_FUNC:
		printf("LABEL %s\n", is_token_lit(exp->value));
		printf("CREATEFRAME\n");
		//todo

		printf("POPFRAME\n");
		printf("RETURN\n");

		//printf("MOVE\n");
		break;

	case ID_EQ:
		//todo
		printf("EQ %s %s %s\n", is_token_lit(exp->value), is_token_lit(exp->RPtr->value), is_token_lit(exp->LPtr->value));

		//printf("MOVE\n");
		break;

	case ID_LESS:
		//todo
		printf("LT %s %s %s\n", is_token_lit(exp->value), is_token_lit(exp->RPtr->value), is_token_lit(exp->LPtr->value));

		//printf("MOVE\n");
		break;

	case ID_GREATER:
		//todo
		printf("GT %s %s %s\n", is_token_lit(exp->value), is_token_lit(exp->RPtr->value), is_token_lit(exp->LPtr->value));

		//printf("MOVE\n");
		break;

	default:
		break;
	}

}


void semicollon ()
{

}

// ukazatel na přirazeni či vytvoření nove proměné
// rekurzivní volání a generování instrukcí, vyjma vestavených funkci či funkcí s navratem
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
			//print_token(exp->value);

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
			//printf("Cyc lengthllll: %d\n", cyc);
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
			//print_token(exp->value);

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
		//exit(0);
	}
}

Exp *get_return_node (Exp *exp)
{
	while (exp->value->id != ID_KEY_RETURN)
		exp = exp->LPtr;
	
	return exp;
}

//print instructions for built-in funtions
void proc_builtin(int builtin_func, Exp *exp, Exp *retvals)
{
	switch(builtin_func)
	{
		case BUILT_INPUTS:
			break;
		case BUILT_INPUTI:
			break;
		case BUILT_INPUTF:
			break;
		case BUILT_PRINT:
			built_print(exp);
			break;
		case BUILT_INT2FLOAT:
			built_int2float(exp, retvals);
			break;
		case BUILT_FLOAT2INT:
			built_float2int(exp, retvals);
			break;
		case BUILT_LEN:
			break;
		case BUILT_SUBSTR:
			break;
		case BUILT_ORD:
			break;
		case BUILT_CHR:
			break;
	}
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
		switch (exp->value->id)
		{
			case ID_IDENTIFIER:
				printf("MOVE TF@?param%d LF@%s\n", counter, exp->value->att.s);
			break;
			case ID_INT_LIT:
				printf("MOVE TF@?param%d int@%ld\n", counter, exp->value->att.i);
			break;
			case ID_FLOAT_LIT:
				printf("MOVE TF@?param%d float@%f\n", counter, exp->value->att.d);
			break;
			case ID_STRING_LIT:
				printf("MOVE TF@?param%d string@%s\n", counter, exp->value->att.s);
			break;
			default:
			break;
		}
}

//goes through all the function call arguments
void proc_func_args(Exp *exp, int counter)
{
	if (exp->value->id == ID_COMMA)
	{
		print_arg(exp->RPtr, counter);
		counter++;
		proc_func_args(exp->LPtr, counter);
	}
	else
		print_arg(exp, counter);
	return;
}

//print instructions for every parameter
void print_param(Exp *exp, int counter)
{
	printf("DEFVAR LF@%s\n", exp->LPtr->value->att.s);
	printf("MOVE LF@%s LF@?param%d\n",exp->LPtr->value->att.s, counter);
	return;
}

//at the start of a funtion definition, get params from temp frame
void proc_func_params(Exp *exp, int counter)
{
	if (exp->value->id == ID_COMMA)
	{
		print_param(exp->RPtr, counter);
		counter++;
		proc_func_params(exp->LPtr, counter);
	}
	else
		print_param(exp, counter);
	return;
}
void print_def_retval(Exp *exp, int counter)
{
	printf("DEFVAR LF@$retval%d\n", counter);
	printf("MOVE LF@$retval%d LF@%s\n", counter, exp->value->att.s );
	return;
}
//go through ret vals in function definition
void proc_func_def_retvals(Exp *exp, int counter)
{
	if (exp->value->id == ID_COMMA)
	{
		print_def_retval(exp->RPtr, counter);
		counter++;
		proc_func_def_retvals(exp->LPtr, counter);
	}
	else
		print_def_retval(exp, counter);
	return;
}

void print_call_retval(Exp *exp, int counter)
{
	printf("MOVE LF@%s LF@$retval%d\n", exp->value->att.s, counter);
	return;
}

void proc_func_call_retvals(Exp *exp, int counter)
{
	if (exp->value->id == ID_COMMA)
	{
		print_call_retval(exp->RPtr, counter);
		counter++;
		proc_func_call_retvals(exp->LPtr, counter);
	}
	else
		print_call_retval(exp, counter);
	return;
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
			int builtin_func;
			//is built-in
			if ((builtin_func = is_builtin(exp->LPtr->value->att.s)) != -1) 
				proc_builtin(builtin_func, exp, retvals);
			//is user function
			else
			{
				printf("CREATEFRAME\n"); //zbytecne kdyz nema parametry
				if (exp->RPtr != NULL) //has parameters
					proc_func_args(exp->RPtr, 0);
				printf("CALL $%s\n", exp->LPtr->value->att.s);
				if (retvals != NULL)
				{
					proc_func_call_retvals(retvals, 0);
				}
			}
			
			return;
		}
		//Kubova kouzelna funkce
		else if (exp->value->id == ID_DEFINE || exp->value->id == ID_ASSIGN) {
			syntax(exp);
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
	if (exp->value->id == ID_SEMICOLLON){
		printf("\nLABEL $%s\n", exp->RPtr->LPtr->value->att.s);
		printf("PUSHFRAME\n");
		//parameters
		if (exp->RPtr->Condition->LPtr != NULL)		
			proc_func_params(exp->RPtr->Condition->LPtr, 0);
		//function body
		proc_func(exp->RPtr);
		//return values
		if (exp->RPtr->Condition->RPtr != NULL)
		{
			Exp *returnNode = get_return_node(exp->RPtr->RPtr);
			proc_func_def_retvals(returnNode->LPtr, 0);
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
	}
	return;
}
