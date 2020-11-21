#include "parser.h"
#include <stdlib.h>
#include "../scanner/scanner.h"
#include "token_stack.h";
#include <stdbool.h>
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
        tToken *token = malloc(sizeof(tToken));
        if (token == NULL)
            return NULL;                    //TODO: rework
        status = get_token(token, EOL_REQ); //TODO: error handling
        tsPush(stack, token);
        ResolveRules(stack);
    } while (status == RET_OK);

    /* == Cleanup and return tre == */
    Exp *final_tree = stack->top;
    tsDispose(stack);
    return final_tree;
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
                //push(CreateLeaf(pop(stack->top->token)));
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
}

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
        //new_exp = CreateTree(st->prev->prev, st->prev, st);
        ReplaceWithExp(st, new_exp, 2);
        changed = true;
    }

    /* == WRITE == */
    st = searchForRule(stack, ID_EQ, endToken);
    if (st != NULL)
    {
        //new_exp = CreateTree(st->prev->prev, st->prev, st);
        ReplaceWithExp(st, new_exp, 2);
        changed = true;
    }
    //TODO: add other rules
    return changed;
}
