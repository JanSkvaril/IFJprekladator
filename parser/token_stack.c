#include <stdlib.h>
#include <stdbool.h>

#include "token_stack.h"
void tsInit(TokenStack *stack)
{
    stack->top = NULL;
}

void tsPushToken(TokenStack *stack, tToken *token)
{
    sToken *new_token = malloc(sizeof(sToken));
    if (new_token == NULL)
        return; //TODO: rework!!
    new_token->prev = stack->top;
    new_token->token = token;
    new_token->exp = NULL;
    stack->top = new_token;
    printf("Adding token to stack: ");
    print_token(token);
}

void tsPushExp(TokenStack *stack, Exp *exp)
{
    sToken *new_token = malloc(sizeof(sToken));
    if (new_token == NULL)
        return; //TODO: rework!!
    new_token->prev = stack->top;
    new_token->exp = exp;
    new_token->token = NULL;
    printf("Adding exp to stack:");
    print_token(exp->value);
    stack->top = new_token;
}

tToken *tsPopToken(TokenStack *stack)
{
    if (stack->top == NULL)
        return NULL;
    tToken *token = stack->top->token;
    sToken *deleted = stack->top;
    stack->top = deleted->prev;
    free(deleted);
    return token;
}

Exp *tsPopExp(TokenStack *stack)
{
    if (stack->top == NULL)
        return NULL;
    Exp *token = stack->top->exp;
    sToken *deleted = stack->top;
    stack->top = deleted->prev;
    free(deleted);
    return token;
}

bool IsToken(sToken *token)
{
    if (token == NULL)
        return false;
    return token->token != NULL;
}

void tsDispose(TokenStack *stack)
{
    while (stack->top != NULL)
    {
        sToken *deleted = stack->top;
        stack->top = deleted->prev;
        free(deleted);
    }
}
sToken *searchForRule(TokenStack *stack, tID tokenID, tID endToken)
{
    sToken *current = stack->top->prev;
    if (IsToken(current))
    {
        if (current->token->id == endToken)
        {
            return NULL;
        }
        current = current->prev;
    }
    else
    {
        if (IsToken(current->prev))
        {
            if (current->prev->token->id == tokenID)
            {
                if (IsToken(current->prev->prev) == false)
                {
                    return current;
                }
            }
        }
        else
            return NULL; //TODO: error?
    }
    return NULL;
}

void ReplaceWithExp(sToken *token, Exp *exp, int delete)
{
    if (IsToken(token) == false)
    {
        token->exp = exp;
        printf("Replacing with exp: ");
        print_token(exp->value);
        sToken *prev = token->prev;
        for (int i = 1; i <= delete; i++)
        {
            token->prev = prev->prev;
            if (IsToken(prev))
                free(prev->token); //TODO: zeptat se na tohle erika!
            free(prev);
            prev = token->prev;
        }
    }
    else
        return; //TODO: error?
}