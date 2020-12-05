#include "generator.h"
#include "../debug.h"

int inFunction = 0;
int inMain = 0;
char isMain[] = "main";
Exp *id_exp = NULL;

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
//     return snprintf((char *)token->att.s, 128, (char *)"TF@%s");
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

void gen_code(tTokenPtr token, Exp *exp) {

	DEBUG_PRINT(("---------- start gen ----------\n"));
	int type = token->id;
	char *a;

	switch (type) {
	case ID_ADD:

		exp->value->id = ID_IDENTIFIER;

    counter++;
    buffer = malloc(snprintf(0, 0, "var$%d", counter)+1);
    sprintf(buffer, "var$%d", counter);
    exp->value->att.s = buffer;

		//a = is_token_add(exp->value, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
		printf("DEFVAR TF@%s\n", exp->value->att.s);


		type = exp->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("ADD TF@%s TF@%s ", exp->value->att.s, exp->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("ADD TF@%s int@%ld ", exp->value->att.s, exp->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("ADD TF@%s float@%.16lf ", exp->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("ADD TF@%s string@%s ", exp->value->att.s, exp->LPtr->value->att.s);
			break;
		}

		type = exp->RPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("TF@%s\n", exp->RPtr->value->att.s);
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
		printf("DEFVAR TF@%s\n", exp->value->att.s);


		type = exp->RPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("SUB TF@%s TF@%s ", exp->value->att.s, exp->RPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("SUB TF@%s int@%ld ", exp->value->att.s, exp->RPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("SUB TF@%s float@%.16lf ", exp->value->att.s, exp->RPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("SUB TF@%s string@%s ", exp->value->att.s, exp->RPtr->value->att.s);
			break;
		}

		type = exp->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("TF@%s\n", exp->LPtr->value->att.s);
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
		printf("DEFVAR TF@%s\n", exp->value->att.s);


		type = exp->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("MUL TF@%s TF@%s ", exp->value->att.s, exp->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("MUL TF@%s int@%ld ", exp->value->att.s, exp->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("MUL TF@%s float@%.16lf ", exp->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("MUL TF@%s string@%s ", exp->value->att.s, exp->LPtr->value->att.s);
			break;
		}

		type = exp->RPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("TF@%s\n", exp->RPtr->value->att.s);
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
		printf("DEFVAR TF@%s\n", exp->value->att.s);


		type = exp->RPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("DIV TF@%s TF@%s ", exp->value->att.s, exp->RPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("DIV TF@%s int@%ld ", exp->value->att.s, exp->RPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("DIV TF@%s float@%.16lf ", exp->value->att.s, exp->RPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("DIV TF@%s string@%s ", exp->value->att.s, exp->RPtr->value->att.s);
			break;
		}

		type = exp->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("TF@%s\n", exp->LPtr->value->att.s);
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
			printf("DEFVAR TF@%s\n", exp->RPtr->value->att.s);
			break;
		}


		type = exp->LPtr->value->id;
		switch (type) {
		case ID_IDENTIFIER:
			printf("MOVE TF@%s TF@%s\n", exp->RPtr->value->att.s, exp->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("MOVE TF@%s int@%ld\n", exp->RPtr->value->att.s, exp->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("MOVE TF@%s float@%.16lf\n", exp->RPtr->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("MOVE TF@%s string@%s\n", exp->RPtr->value->att.s, exp->LPtr->value->att.s);
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
			printf("MOVE TF@%s TF@%s\n", exp->RPtr->value->att.s, exp->LPtr->value->att.s);
			break;
		case ID_INT_LIT:
			printf("MOVE TF@%s int@%ld\n", exp->RPtr->value->att.s, exp->LPtr->value->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("MOVE TF@%s float@%.16lf\n", exp->RPtr->value->att.s, exp->LPtr->value->att.d);
			break;
		case ID_STRING_LIT:
			printf("MOVE TF@%s string@%s\n", exp->RPtr->value->att.s, exp->LPtr->value->att.s);
			break;
		}


		//printf("WRITE %s\n", a);
		//printf("MOVE\n");
		break;

	case ID_INT_LIT:
		a = is_token_add(exp->value, exp->LPtr->value->att.s, exp->RPtr->value->att.s);
		printf("DEFVAR TF@%s\n", a);

		printf("MOVE TF@%s TF@%s\n", a, exp->LPtr->value->att.s);

		printf("WRITE %s\n", a);
		//printf("MOVE\n");
		break;

	case ID_KEY_IF:

		//todo

		printf("JUMPIFEQ else%s TF@b TF@b\n", is_token_lit(exp->value));

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

	DEBUG_PRINT(("---------- stop gen ----------\n"));
}


void semicollon ()
{

}

// ukazatel na přirazeni či vytvoření nove proměné
// rekurzivní volání a generování instrukcí, vyjma vestavených funkci či funkcí s navratem
void syntax(Exp *exp, Exp *root)
{
	root = exp;


	if (exp->value->id == ID_DEFINE || exp->value->id == ID_ASSIGN)
	{

		id_exp = exp;

	}


	if (exp->value->id == ID_DEFINE || exp->value->id == ID_ASSIGN || exp->value->id == ID_ADD || exp->value->id == ID_SUB || exp->value->id == ID_MULT || exp->value->id == ID_DIV)
	{

		if (((exp->RPtr->value->id == ID_IDENTIFIER  || exp->RPtr->value->id == ID_INT_LIT  || exp->RPtr->value->id == ID_FLOAT_LIT  || exp->RPtr->value->id == ID_STRING_LIT) && (exp->LPtr->value->id == ID_IDENTIFIER || exp->LPtr->value->id == ID_INT_LIT || exp->LPtr->value->id == ID_FLOAT_LIT || exp->LPtr->value->id == ID_STRING_LIT))) {
			gen_code(exp->value, exp);
			exp = root;
			syntax(exp, root);
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
				gen_code(exp->value, exp);
				exp = id_exp;
				syntax(exp, root);
			}
			else
			{
				//exp = root;
				syntax(exp, root);
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
				gen_code(exp->value, exp);
				exp = id_exp;
				syntax(exp, root);
			}
			else
			{
				//exp = root;
				syntax(exp, root);
			}

		}




	}
	else
	{
    DEBUG_PRINT(("__NEW TREE__\n"));
		//DEBUG_PRINT(("EXIT 0\n"));
		//exit(0);
	}
}


void generator(Exp *exp, Exp *root)
{
	root = exp;


	if (exp->value->id == ID_SEMICOLLON) {

		// // verify: is package?
		// if (exp->RPtr->value->id) == ID_KEY_PACKAGE) {
		// 	exp = exp->LPtr;
		// 	DEBUG_PRINT(("- package found! -\n"));
		// 	generator(exp, root);
		// }
		// else
		// {
		//   // set label - if main
		//   if (!strcmp((exp->RPtr->LPtr->value->att.s), isMain)) {
		//     printf("LABEL %s\n", exp->RPtr->LPtr->value->att.s);
		//   }
		//   else
		//   {
		//     printf("LABEL %s\n", isMain);
		//     printf("\n");
		//     printf("LABEL %s\n", exp->RPtr->LPtr->value->att.s);
		//   }
		//
		// }

		if (exp->RPtr->value->id == ID_DEFINE || exp->RPtr->value->id == ID_ASSIGN) {
			exp = exp->RPtr;
			syntax(exp, exp);
			exp = root;
			//print_token(exp->value);
			generator(exp, root);
		}

		if (exp->LPtr->value->id == ID_DEFINE || exp->LPtr->value->id == ID_ASSIGN) {
			exp = exp->LPtr;
			syntax(exp, exp);
			exp = root;
			//print_token(exp->value);
			generator(exp, root);
		}

		exp = exp->LPtr;
		generator(exp, root);


	}
	else if (exp->value->id == ID_KEY_FUNC) {
		printf("LABEL %s\n", exp->value->att.s);
		printf("CREATEFRAME\n");

	}
	else if (exp->value->id == ID_KEY_RETURN) {
		printf("POPFRAME\n");
		printf("RETURN\n");

	}
	else if (exp->value->id == ID_FUNC_CALL) {

	}
	else if (exp->value->id == ID_DEFINE || exp->value->id == ID_ASSIGN) {
		syntax(exp, exp);
		exp = root;
		//print_token(exp->value);
		generator(exp, root);
	}







}
