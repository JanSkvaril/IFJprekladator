#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>

#define solve_and_replace_exp(ID_OF_TOKEN)                                     \
    do                                                                         \
    {                                                                          \
        st = searchForRule(stack, ID_OF_TOKEN, endToken);                      \
        if (st != NULL)                                                        \
        {                                                                      \
            /*printf("Expression rule found: %d\n", ID_OF_TOKEN);     */       \
            new_exp = makeTree(st->exp, st->prev->prev->exp, st->prev->token); \
            ReplaceWithExp(st, new_exp, 2);                                    \
            changed = true;                                                    \
        }                                                                      \
    } while (false)

/* Returns true if anything changed */
bool ResolveExpresionRules(TokenStack *stack, id_t endToken)
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
    /* == WRITE == */
    solve_and_replace_exp(ID_ASSIGN);
    /* == DEFINE == */
    solve_and_replace_exp(ID_DEFINE);
    /* == CONNECT == */
    solve_and_replace_exp(ID_SEMICOLLON);

    //TODO: add other rules
    return changed;
}

bool ResolveRules(TokenStack *stack)
{
    bool changed = false;
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
            /* == Left Curly bracket: { ==*/
            else if (stack->top->token->id == ID_CURLY_1)
            {
                AddSemicolom(stack);
            }
            /* == Right bracket: ) == */
            else if (stack->top->token->id == ID_ROUND_2)
            {
                /* == Single Exp in bracket: (Exp) */
                if (stack->top->prev->prev->token->id == ID_ROUND_1)
                {
                    tsPopToken(stack);          //(
                    Exp *exp = tsPopExp(stack); //exp
                    tsPopToken(stack);          //)
                    tsPushExp(stack, exp);
                    changed = true;
                }
                else
                {

                    changed = ResolveExpresionRules(stack, ID_ROUND_1);
                }
            }
            /* == Semicollon: ; == */
            else if (stack->top->token->id == ID_SEMICOLLON)
            {
                changed = ResolveExpresionRules(stack, ID_SEMICOLLON);
            }
            /* == Right Curly bracket: } ==*/
            else if (stack->top->token->id == ID_CURLY_2)
            {
                printf("} - replacing\n");
                //should like this {;EXP;}
                tsPopToken(stack);          //}
                tsPopToken(stack);          //;
                Exp *exp = tsPopExp(stack); //exp
                tsPopToken(stack);          //;
                tsPopToken(stack);          //{
                tsPushExp(stack, exp);
                changed = true;
                //AddSemicolom(stack);

                /* == ELSE == */
                if (IsToken(stack->top->prev))
                {
                    if (stack->top->prev->token->id == ID_KEY_ELSE)
                    {
                        Exp *elseExp = tsPopExp(stack);
                        tsPopToken(stack); //else
                        tsPopToken(stack); //;
                        AddToIfTree(stack->top->exp, elseExp);
                        AddSemicolom(stack);
                    }
                }
            }
        }
        else //expreession
        {

            if (!IsToken(stack->top->prev))
            {
                /* == IF ==*/
                if (IsToken(stack->top->prev->prev) && stack->top->prev->prev->token->id == ID_KEY_IF)
                {
                    printf("Parsing if\n");
                    Exp *trueExp = tsPopExp(stack);
                    Exp *condExp = tsPopExp(stack);
                    tToken *t = tsPopToken(stack);
                    Exp *ifExp = makeIfTree(trueExp, condExp, NULL, t);
                    tsPushExp(stack, ifExp);

                    changed = true;
                    AddSemicolom(stack);
                }
            }
        }

    } while (changed == true);
    return false;
}

Exp *Parse()
{
    /* == Create stack == */
    TokenStack *stack = malloc(sizeof(TokenStack));
    if (stack == NULL)
        return NULL; //TODO: rework
    tsInit(stack);
    AddSemicolom(stack);

    /* == Parse == */
    printf("    == Parsing starts ==\n");
    tTokenRet status;
    do
    {
        tToken *token = NULL;
        status = get_token(&token, EOL_OPT); //TODO: error handling
        if (status == RET_OK)
        {
            tsPushToken(stack, token);
            ResolveRules(stack);
        }
    } while (status == RET_OK);
    printf("    == Parsing finished ==\n");
    /* == Cleanup and return tree == */
    /* Stack should end in state: ; EXP ;   */
    tsPopToken(stack);                 //first ;
    Exp *final_tree = tsPopExp(stack); //EXP
    printf("Root token is: ");
    print_token(final_tree->value);
    tsPopToken(stack); //second ;
    if (stack->top == NULL)
    {
        printf("Succesfuly reduced to one Exp\n");
    }
    tsDispose(stack);
    return final_tree;
}
