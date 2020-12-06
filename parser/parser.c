#include "parser.h"
#include "../debug.h"
#include <stdlib.h>
#include <stdbool.h>

#define solve_and_replace_exp(ID_OF_TOKEN)                                                 \
    do                                                                                     \
    {                                                                                      \
        st = searchForRule(stack, ID_OF_TOKEN, endToken);                                  \
        if (st != NULL)                                                                    \
        {                                                                                  \
            /*DEBUG_PRINT(("Expression rule found: %d\n", ID_OF_TOKEN));*/                 \
            new_exp = makeTree(st->exp, st->prev->prev->exp, st->prev->token); \
            ReplaceWithExp(st, new_exp, 2);                                                \
            return true;                                                                   \
        }                                                                                  \
    } while (false)

#define replace_return

/* Returns true if anything changed */
bool ResolveExpresionRules(TokenStack *stack, id_t endToken, scopeStack *scope)
{
    bool changed = false;
    sToken *st;
    Exp *new_exp;
    /* =    Aritmetics  = */
    /* == MULT == */
    solve_and_replace_exp(ID_MULT);
    /* == DIV == */
    solve_and_replace_exp(ID_DIV);
    /* == ADD == */
    solve_and_replace_exp(ID_ADD);
    /* == SUB == */
    solve_and_replace_exp(ID_SUB);
    /* =    Boolean   = */
    /* == EQ ==*/
    solve_and_replace_exp(ID_EQ);
    /* == NEQ ==*/
    solve_and_replace_exp(ID_NEQ);
    /* == LESS ==*/
    solve_and_replace_exp(ID_LESS);
    /* == LESS_EQ ==*/
    solve_and_replace_exp(ID_LESS_EQ);
    /* == GREATER==*/
    solve_and_replace_exp(ID_GREATER);
    /* == GREATER==*/
    solve_and_replace_exp(ID_GREATER_EQ);
    /* =    Others   = */
    /* == COMMA == */
    solve_and_replace_exp(ID_COMMA);
    /* == WRITE == */
    solve_and_replace_exp(ID_ASSIGN);
    /* == DEFINE == */
    solve_and_replace_exp(ID_DEFINE);
    /* == RETURN VAL == */
    st = searchForDualRule(stack, ID_KEY_RETURN, endToken);
    if (st != NULL)
    {
        tToken *init_token = malloc(sizeof(tToken));
        init_token->id = ID_KEY_RETURN;
        new_exp = makeTree(st->exp, NULL, init_token);
        ReplaceWithExp(st, new_exp, 1);
        return true;
    }
    /* == PACKAGE == */
    st = searchForDualRule(stack, ID_KEY_PACKAGE, endToken);
    if (st != NULL)
    {
        tToken *init_token = malloc(sizeof(tToken));
        init_token->id = ID_KEY_PACKAGE;
        new_exp = makeTree(st->exp, NULL, init_token);
        ReplaceWithExp(st, new_exp, 1);
        return true;
    }
    /* == ONLY RETURN == */
    sToken *curr = stack->top->prev;
    while ((IsToken(curr) && curr->token->id == endToken) == false)
    {

        if (IsToken(curr) && curr->token->id == ID_KEY_RETURN)
        {
            DEBUG_PRINT(("Replacing return without value\n"));
            tToken *init_token = malloc(sizeof(tToken));
            init_token->id = ID_KEY_RETURN;
            free(curr->token);
            curr->token = NULL;
            new_exp = makeTree(NULL, NULL, init_token);
            curr->exp = new_exp;
            return true;
        }
        curr = curr->prev;
    }
    /* == CONNECT == */
    st = searchForDoubleExp(stack);
    if (st != NULL)
    {
        tToken *init_token = malloc(sizeof(tToken));
        init_token->id = ID_SEMICOLLON;
        new_exp = makeTree(st->exp, st->prev->exp, init_token);
        ReplaceWithExp(st, new_exp, 1);
        return true;
    }

    /* == CONNECT FOR == */
    solve_and_replace_exp(ID_SEMICOLLON);

    //TODO: add other rules
    return changed;
}

bool ResolveRules(TokenStack *stack, scopeStack *scope)
{
    bool changed = false;
    bool scopeChanged = false;
    do
    {
        changed = false;
        if (IsToken(stack->top))
        {
            /* == Identifier to Exp == */
            if (stack->top->token->id == ID_IDENTIFIER)
            {
                tsPushExp(stack, makeLeaf(tsPopToken(stack)));
                changed = true;
            }
            /* == LITERAL ==*/
            else if (stack->top->token->id == ID_INT_LIT || stack->top->token->id == ID_FLOAT_LIT || stack->top->token->id == ID_STRING_LIT)
            {
                tsPushExp(stack, makeLeaf(tsPopToken(stack)));
                changed = true;
            }
            /* == UNDERLINE == */
            else if (stack->top->token->id == ID_UNDER)
            {
                tsPushExp(stack, makeLeaf(tsPopToken(stack)));
                changed = true;
            }
            /* == TYPE DEF == */
            else if (stack->top->token->id == ID_KEY_INT || stack->top->token->id == ID_KEY_FLOAT64 || stack->top->token->id == ID_KEY_STRING)
            {
                tToken *type = tsPopToken(stack);
                Exp *leaf = makeLeaf(type);
                if (stack->top != NULL && IsToken(stack->top) == false)
                {
                    tToken *typed = malloc(sizeof(tToken));
                    if (typed == NULL)
                        return false; //TODO: error
                    typed->id = ID_TYPE_DEF;
                    tsPushExp(stack, makeTree(tsPopExp(stack), leaf, typed));
                }
                else
                {
                    tsPushExp(stack, leaf);
                }
                changed = true;
            }
            /* == Left Curly bracket: { ==*/
            else if (stack->top->token->id == ID_CURLY_1)
            {
                if (scopeChanged == false)
                {
                    ssAdd(scope);
                    scopeChanged = true;
                }
                if (stack->top->prev != NULL)
                {
                    if (IsToken(stack->top->prev) && (stack->top->prev->token->id == ID_SEMICOLLON))
                    {
                        tToken *bracket = tsPopToken(stack);
                        tToken *empty = malloc(sizeof(tToken));
                        empty->id = ID_SEMICOLLON;
                        Exp *emptyExp = makeTree(NULL, NULL, empty);
                        tsPushExp(stack, emptyExp);
                        tsPushToken(stack, bracket);
                        changed = true;
                    }
                    sToken *curr = stack->top->prev;
                    /* Try to find if or for */
                    while ((IsToken(curr) && curr->token->id == ID_CURLY_1) != true)
                    {
                        /* IF */
                        if (IsToken(curr) && curr->token->id == ID_KEY_IF)
                        {
                            bool changed = true;
                            while (changed)
                            {
                                changed = ResolveExpresionRules(stack, ID_KEY_IF, scope);
                            }
                            break;
                        }
                        /* FOR */
                        else if (IsToken(curr) && curr->token->id == ID_KEY_FOR)
                        {
                            bool changed = true;
                            while (changed)
                            {
                                changed = ResolveExpresionRules(stack, ID_KEY_FOR, scope);
                            }
                            break;
                        }
                        curr = curr->prev;
                    }
                }
            }
            /* == Right bracket: ) == */
            else if (stack->top->token->id == ID_ROUND_2)
            {
                //sToken *curr = stack->top->prev;

                /* == Single Exp in bracket: (Exp) */
                if (IsToken(stack->top->prev->prev) && stack->top->prev->prev->token->id == ID_ROUND_1)
                {
                    tsPopToken(stack);          //)
                    Exp *exp = tsPopExp(stack); //exp
                    tsPopToken(stack);          //(
                    tsPushExp(stack, exp);
                    changed = true;
                    /* == FUNC CALL == */
                    if (stack->top->prev != NULL && IsToken(stack->top->prev) == false)
                    {
                        // if its not a function definition
                        bool isDefinition = false;
                        sToken *curr = stack->top->prev;
                        while (curr != NULL && (IsToken(curr) && curr->token->id == ID_CURLY_1) != true)
                        {
                            if (IsToken(curr) && curr->token->id == ID_KEY_FUNC)
                            {
                                isDefinition = true;
                                break;
                            }
                            curr = curr->prev;
                        }
                        if (isDefinition == false)
                        {
                            if (getValue(stack->top->prev->exp)->id == ID_IDENTIFIER)
                            {
                                Exp *funcArgs = tsPopExp(stack);
                                Exp *funcName = tsPopExp(stack);
                                tToken *funcToken = malloc(sizeof(tToken));
                                funcToken->id = ID_FUNC_CALL;
                                if (funcToken == NULL)
                                    return false; //TODO: error?
                                tsPushExp(stack, makeTree(funcName, funcArgs, funcToken));
                            }
                        }
                    }
                }
                /* Empty brackets */
                else if (IsToken(stack->top->prev) && stack->top->prev->token->id == ID_ROUND_1)
                {
                    tsPopToken(stack);
                    tsPopToken(stack);
                    //function call with empty brackets
                    if (IsToken(stack->top) == false && getValue(stack->top->exp)->id == ID_IDENTIFIER)
                    {
                        // if its not a function definition
                        bool isDefinition = false;
                        sToken *curr = stack->top->prev;
                        while (curr != NULL && (IsToken(curr) && curr->token->id == ID_CURLY_1) != true)
                        {
                            if (IsToken(curr) && curr->token->id == ID_KEY_FUNC)
                            {
                                isDefinition = true;
                                tToken *temp = malloc(sizeof(tToken));
                                temp->id = ID_EMPTY;
                                tsPushToken(stack, temp);
                                break;
                            }
                            curr = curr->prev;
                        }
                        if (isDefinition == false)
                        {
                            Exp *funcName = tsPopExp(stack);
                            tToken *funcToken = malloc(sizeof(tToken));
                            funcToken->id = ID_FUNC_CALL;
                            if (funcToken == NULL)
                                return false; //TODO: error?
                            tsPushExp(stack, makeTree(funcName, NULL, funcToken));
                        }
                    }
                    changed = true;
                }
                else
                {

                    changed = ResolveExpresionRules(stack, ID_ROUND_1, scope);
                }
            }
            /* == Semicollon: ; == */
            else if (stack->top->token->id == ID_SEMICOLLON)
            {
                if (IsToken(stack->top->prev) && (stack->top->prev->token->id == ID_KEY_FOR || stack->top->prev->token->id == ID_SEMICOLLON))
                {
                    tToken *semicol = tsPopToken(stack);
                    tToken *empty = malloc(sizeof(tToken));
                    empty->id = ID_SEMICOLLON;
                    Exp *emptyExp = makeTree(NULL, NULL, empty);
                    tsPushExp(stack, emptyExp);
                    tsPushToken(stack, semicol);
                    changed = true;
                }
                //tsPopToken(stack);
            }
            /* == Right Curly bracket: } ==*/
            else if (stack->top->token->id == ID_CURLY_2)
            {
                //empty
                if (stack->top->prev != NULL && IsToken(stack->top->prev) && stack->top->prev->token->id == ID_CURLY_1)
                {
                    tToken *bracket = tsPopToken(stack);

                    tToken *init_token = malloc(sizeof(tToken));
                    init_token->id = ID_SEMICOLLON;
                    //adds empty exp to bracket so function, if and for rules work corretly
                    tsPushExp(stack, makeTree(NULL, NULL, init_token));
                    tsPushToken(stack, bracket);
                }
                //not empty
                changed = ResolveExpresionRules(stack, ID_CURLY_1, scope);
                if (changed == false)
                {
                    if (stack->top == NULL || stack->top->prev == NULL || stack->top->prev->prev == NULL)
                    {
                        parser_free_exit(SYN_ERR);
                    }
                    else if ((IsToken(stack->top->prev) == false && IsToken(stack->top->prev->prev) && stack->top->prev->prev->token->id == ID_CURLY_1) == false)
                    {
                        parser_free_exit(SYN_ERR);
                    }
                    DEBUG_PRINT(("} - replacing\n"));
                    //should like this {EXP}
                    tsPopToken(stack);          //}
                    Exp *exp = tsPopExp(stack); //exp
                    tsPopToken(stack);          //{
                    tsPushExp(stack, exp);
                    changed = true;

                    /* == ELSE == */
                    if (stack->top->prev != NULL && IsToken(stack->top->prev))
                    {
                        if (stack->top->prev->token->id == ID_KEY_ELSE)
                        {
                            Exp *elseExp = tsPopExp(stack);
                            tsPopToken(stack); //else
                            AddToIfTree(stack->top->exp, elseExp);
                        }
                    }
                    /* == FUNC DEFINITION == */
                    sToken *curr = stack->top->prev;
                    int expCounter = 0;
                    while (curr != NULL && (IsToken(curr) && curr->token->id == ID_CURLY_1) != true)
                    {
                        if (IsToken(curr) && curr->token->id == ID_KEY_FUNC)
                        {
                            DEBUG_PRINT(("    Function definition: \n"));
                            DEBUG_PRINT(("Body: "));
                            Exp *body = tsPopExp(stack);

                            Exp *ret;
                            if (expCounter == 3)
                            {
                                DEBUG_PRINT(("Return value: "));
                                ret = tsPopExp(stack);
                            }
                            else
                            {
                                DEBUG_PRINT(("No return value\n"));
                                ret = NULL;
                            }
                            Exp *args;
                            if (expCounter == 1)
                            {
                                DEBUG_PRINT(("No arguments\n"));
                                args = NULL;
                            }
                            else
                            {
                                if (IsToken(stack->top))
                                {
                                    free(tsPopToken(stack));
                                    DEBUG_PRINT(("No arguments\n"));
                                    args = NULL;
                                }
                                else
                                {
                                    DEBUG_PRINT(("Arguments: "));
                                    args = tsPopExp(stack);
                                }
                            }
                            DEBUG_PRINT(("Name: "));
                            Exp *name = tsPopExp(stack);
                            tToken *funcT = tsPopToken(stack);
                            tToken *argsRetToken = malloc(sizeof(tToken));
                            argsRetToken->id = ID_SEMICOLLON;
                            Exp *argsRet = makeTree(args, ret, argsRetToken);

                            tsPushExp(stack, makeIfTree(name, argsRet, body, funcT));
                            break;
                        }
                        expCounter++;
                        curr = curr->prev;
                    }

                    /* Remove scope */
                    ssPop(scope);
                }
            }
        }
        else //expreession
        {

            if (stack->top->prev != NULL && !IsToken(stack->top->prev))
            {
                /* == IF ==*/
                if (IsToken(stack->top->prev->prev) && stack->top->prev->prev->token->id == ID_KEY_IF)
                {
                    DEBUG_PRINT(("Parsing if\n"));
                    Exp *trueExp = tsPopExp(stack);
                    Exp *condExp = tsPopExp(stack);
                    tToken *t = tsPopToken(stack);
                    Exp *ifExp = makeIfTree(trueExp, condExp, NULL, t);
                    tsPushExp(stack, ifExp);

                    changed = true;
                }
                /* == FOR == */
                else if (IsToken(stack->top->prev->prev) && stack->top->prev->prev->token->id == ID_KEY_FOR)
                {
                    DEBUG_PRINT(("Parsing for\n"));
                    Exp *body = tsPopExp(stack);
                    Exp *head = tsPopExp(stack);
                    tToken *t = tsPopToken(stack);
                    Exp *forExp = makeTree(body, head, t);
                    tsPushExp(stack, forExp);

                    changed = true;
                }
            }
        }
    } while (changed == true);
    return false;
}

Exp *Parse()
{
    /* == Create scope == */
    scopeStack *scopeS = malloc(sizeof(scopeStack));
    if (scopeS == NULL)
        return NULL; //TODO: rework
    ssInit(scopeS);
    ssAdd(scopeS);

    /* == Create stack == */
    TokenStack *stack = malloc(sizeof(TokenStack));
    if (stack == NULL)
        return NULL; //TODO: rework
    tsInit(stack);
    tToken *oBracker = malloc(sizeof(oBracker));
    oBracker->id = ID_CURLY_1;
    tToken *oBracker2 = malloc(sizeof(oBracker));
    oBracker2->id = ID_CURLY_2;
    tsPushToken(stack, oBracker);
    /* == Parse == */
    DEBUG_PRINT(("    == Parsing starts ==\n"));
    tTokenRet status;
    do
    {
        tToken *token = NULL;
        status = get_token(&token, EOL_OPT);
        if (status == RET_OK)
        {
            tsPushToken(stack, token);
            ResolveRules(stack, scopeS);
        }
    } while (status == RET_OK);
    //add ending brackert
    tsPushToken(stack, oBracker2);
    ResolveRules(stack, scopeS);

    DEBUG_PRINT(("    == Parsing finished ==\n"));
    /* == Cleanup and return tree == */
    /* Stack should end in state: ; EXP ;   */
    if (stack->top == NULL || IsToken(stack->top))
    {
        DEBUG_PRINT(("!! Error durring parsing !! \n"));
        parser_free_exit(SYN_ERR);
    }
    Exp *final_tree = tsPopExp(stack); //EXP
    DEBUG_PRINT(("Root token is: "));
    print_token(final_tree->value);
    if (stack->top == NULL)
    {
        DEBUG_PRINT(("Succesfuly reduced to one Exp\n"));
    }
    tsDispose(stack);
    ssDispose(scopeS);

    scopeStack *scopeStack = malloc(sizeof(scopeStack));
    if (scopeStack == NULL)
        return NULL;
    ssInit(scopeStack);
    DEBUG_PRINT(("Type check started\n"));
    ssAdd(scopeStack);
    defineFunctions(final_tree, scopeStack);
    CheckTypes(final_tree, scopeStack);
    printf("Type check alright\n");
    ssDispose(scopeStack);
    free(scopeStack);
    return final_tree;
}
