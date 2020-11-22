#include "parser.h"
#include <stdlib.h>
#include <stdbool.h>

#define solve_and_replace_exp()                                            \
    do                                                                     \
    {                                                                      \
        new_exp = makeTree(st->exp, st->prev->prev->exp, st->prev->token); \
        ReplaceWithExp(st, new_exp, 2);                                    \
        changed = true;                                                    \
    } while (false)

/* Returns true if anything changed */
bool ResolveExpresionRules(TokenStack *stack, id_t endToken)
{
    bool changed = false;
    sToken *st;
    Exp *new_exp;

    /* == MULT == */
    st = searchForRule(stack, ID_MULT, endToken);
    if (st != NULL)
    {
        solve_and_replace_exp();
    }

    /* == WRITE == */
    st = searchForRule(stack, ID_EQ, endToken);
    if (st != NULL)
    {
        solve_and_replace_exp();
    }
    //TODO: add other rules
    return changed;
}

bool ResolveRules(TokenStack *stack)
{
    bool changed = false;
    do
    {
        if (IsToken(stack->top))
        {
            /* == Identifier to Exp == */
            if (stack->top->token->id == ID_IDENTIFIER)
            {
                tsPushExp(stack, makeLeaf(tsPopToken(stack)));
                changed = true;
            }
            /* == Right bracket: ) == */
            if (stack->top->token->id == ID_ROUND_2)
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
            if (stack->top->token->id == ID_SEMICOLLON)
            {
                changed = ResolveExpresionRules(stack, ID_SEMICOLLON);
            }
        }
    } while (changed == false);
    return false;
}

Exp *Parse()
{
    /* == Create stack == */
    TokenStack *stack = malloc(sizeof(TokenStack));
    if (stack == NULL)
        return NULL; //TODO: rework
    tsInit(stack);
    tToken *init_token = malloc(sizeof(tToken));
    init_token->id = ID_SEMICOLLON;
    tsPushToken(stack, init_token);

    /* == Parse == */
    tTokenRet status;
    do
    {
        tToken *token = NULL;
        status = get_token(&token, EOL_REQ); //TODO: error handling
        tsPushToken(stack, token);
        ResolveRules(stack);
    } while (status == RET_OK);

    /* == Cleanup and return tre == */
    Exp *final_tree = stack->top->exp;
    tsDispose(stack);
    return final_tree;
}
