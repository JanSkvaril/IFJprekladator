#include "generator.h"
#include "../debug.h"

void gen_code(tTokenPtr token, Exp *exp) {

  DEBUG_PRINT(("---------- start gen ----------\n"));
  int type = is_token(token);
  char *a;

  switch (type) {
    case ID_ADD:
    a = is_token_add(exp->value, is_token_lit(exp->LPtr->value), is_token_lit(exp->RPtr->value));
    printf("DEFVAR GF@%s\n", a);

    printf("ADD GF@%s GF@%s GF@%s\n", a, is_token_lit(exp->LPtr->value), is_token_lit(exp->RPtr->value));
    //printf("ADD\n");
    break;

    case ID_SUB:
    a = is_token_add(exp->value, is_token_lit(exp->LPtr->value), is_token_lit(exp->RPtr->value));
    printf("DEFVAR GF@%s\n", a);

    printf("SUB GF@%s GF@%s GF@%s\n", a, is_token_lit(exp->LPtr->value), is_token_lit(exp->RPtr->value));
    //printf("SUB\n");
    break;

    case ID_MULT:

    a = is_token_add(exp->value, is_token_lit(exp->LPtr->value), is_token_lit(exp->RPtr->value));
    printf("DEFVAR GF@%s\n", a);

    printf("MUL GF@%s GF@%s GF@%s\n", a, is_token_lit(exp->LPtr->value), is_token_lit(exp->RPtr->value));
    //printf("MULT\n");
    break;

    case ID_DIV:
    a = is_token_add(exp->value, is_token_lit(exp->LPtr->value), is_token_lit(exp->RPtr->value));
    printf("DEFVAR GF@%s\n", a);

    printf("DIV GF@%s GF@%s GF@%s\n", a,is_token_lit(exp->LPtr->value), is_token_lit(exp->RPtr->value));
    //printf("DIV\n");
    break;

    case ID_DEFINE:
    a = is_token_add(exp->value, is_token_lit(exp->LPtr->value), is_token_lit(exp->RPtr->value));
    printf("DEFVAR GF@%s\n", a);

    printf("MOVE GF@%s GF@%s\n", a, is_token_lit(exp->LPtr->value));

    printf("WRITE %s\n", a);
    //printf("MOVE\n");
    break;

    case ID_KEY_IF:

    //todo

    printf("JUMPIFEQ else%s GF@b GF@b\n", is_token_lit(exp->value));

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


void generator(Exp *exp, Exp *root)
{
    root = exp;


  Exp *id_exp = NULL;

  //   printf("-----------\n");
  // print_token(exp->value);
  // print_token(exp->LPtr->value);
  // print_token(exp->LPtr->LPtr->value);
  // print_token(exp->LPtr->LPtr->LPtr->value);
  // printf("-----------\n");
  // print_token(exp->RPtr->value);
  // print_token(exp->LPtr->RPtr->value);
  //
  // printf("-----------\n");

  if (is_token(exp->value) == ID_SEMICOLLON) {


    //print_token(exp->LPtr->RPtr->value);
    //print_token(exp->RPtr->LPtr->value);


    //printf("---------- here 2 ----------\n");

    if (is_token(exp->value) == ID_SEMICOLLON)
    {

      while (is_token(exp->value) == ID_SEMICOLLON) {

        if (is_token(exp->LPtr->value) == ID_IDENTIFIER) {

          id_exp = exp->RPtr;
          exp = id_exp;
          //print_token(exp->value);

        }
        else if (is_token(exp->RPtr->value) == ID_IDENTIFIER){

          id_exp = exp->LPtr;
          exp = id_exp;
          //print_token(exp->value);

        }
        else {

          id_exp = exp->RPtr;
          exp = id_exp;
          //print_token(exp->value);
        }


      }

    }

  }
  else if (is_token(exp->value) == ID_DEFINE)
  {

    id_exp = exp;
    exp = id_exp;
    //print_token(exp->value);

  }


  if (is_token(exp->value) == ID_DEFINE || is_token(exp->value) == ID_ADD || is_token(exp->value) == ID_SUB || is_token(exp->value) == ID_MULT || is_token(exp->value) == ID_DIV) {

    //printf("---------- here ----------\n");
    //print_token(exp->value);
    int cyc = 0;

    //size of left node
    while(exp != NULL) {

      if (is_token(exp->value) != ID_IDENTIFIER) {

        exp = exp->LPtr;
        cyc++;

      }
      else{
        break;
      }

    }

    //printf("Cyc length: %d\n", cyc);
    //print_token(exp->value);
    exp = id_exp;



    //tree - move
    int cyc_node = 1;

    while (exp != NULL && cyc_node < cyc) {

      if (is_token(exp->value) != ID_IDENTIFIER) {

        exp = exp->LPtr;
        //print_token(exp->value);
        cyc_node++;

      }
      else
      {
        break;
      }

    }

    gen_code(exp->value, exp);
    exp = id_exp;

    //cyc_node = 0;
    //cyc = 0;


    exp = root;
    // print_token(exp->value);
    // print_token(exp->LPtr->value);
    // print_token(exp->RPtr->value);
    generator(exp, root);

  }
  else {
    DEBUG_PRINT(("EXIT 0\n"));
    //printf("--else--\n");
    exit(0);



  }

}
