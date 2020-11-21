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

    /* == Parse == */
    do
    {
        tToken *token = malloc(sizeof(tToken));
        if (token == NULL)
            return NULL;           //TODO: rework
        get_token(token, EOL_REQ); //TODO: error handling
        tsPush(stack, token);
        ResolveRules(stack);
    } while (1); //TODO: zjistit jak poznám že je konec
}

void ResolveRules(TokenStack *stack)
{
    bool changed = false;
    do
    {
        if (IsToken(stack->top))
        {
            /* == Identifier to Exp == */
            if (stack->top->token->id == ID_IDENTIFIER)
            {
                if (stack->top->prev != NULL)
                {
                    //push(CreateLeaf(pop(stack->top->token)));
                    changed = true;
                }
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
                    ResolveExpresionRules(stack, ID_ROUND_1);
                }
            }
        }
    } while (changed == false);
}

void ResolveExpresionRules(TokenStack *stack, id_t endToken)
{
    sToken *st;
    Exp *new_exp;

    /* == MULT == */
    st = searchForRule(stack, ID_MULT, endToken);
    if (st != NULL)
    {
        //new_exp = CreateTree(st->prev->prev, st->prev, st);
        ReplaceWithExp(st, new_exp, 2);
    }
}