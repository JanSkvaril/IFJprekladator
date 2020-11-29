#include <stdlib.h>
#include <stdbool.h>
#include "../debug.h"
#include "token_stack.h"
void tsInit(TokenStack *stack)
{
    stack->top = NULL;
}

void tsPushToken(TokenStack *stack, tToken *token)
{
    if (token == NULL)
    {
        parser_free_exit(SYN_ERR);
    }
    sToken *new_token = malloc(sizeof(sToken));
    if (new_token == NULL)
        return; //TODO: rework!!
    new_token->prev = stack->top;
    new_token->token = token;
    new_token->exp = NULL;
    stack->top = new_token;
    DEBUG_PRINT(("Adding token to stack: "));
    print_token(token);
}

void tsPushExp(TokenStack *stack, Exp *exp)
{
    if (exp == NULL)
    {
        parser_free_exit(SYN_ERR);
    }
    sToken *new_token = malloc(sizeof(sToken));
    if (new_token == NULL)
        return; //TODO: rework!!
    new_token->prev = stack->top;
    new_token->exp = exp;
    new_token->token = NULL;
    DEBUG_PRINT(("Adding exp to stack:"));
    print_token(exp->value);
    stack->top = new_token;
}

tToken *tsPopToken(TokenStack *stack)
{
    if (stack->top == NULL)
    {
        parser_free_exit(SYN_ERR);
    }
    if (stack->top->token == NULL)
    {
        parser_free_exit(SYN_ERR);
    }
    tToken *token = stack->top->token;
    sToken *deleted = stack->top;
    stack->top = deleted->prev;
    DEBUG_PRINT(("Poping token: "));
    print_token(token);
    free(deleted);
    return token;
}

Exp *tsPopExp(TokenStack *stack)
{
    if (stack->top == NULL)
    {
        parser_free_exit(SYN_ERR);
    }
    if (stack->top->exp == NULL)
    {
        parser_free_exit(SYN_ERR);
    }
    Exp *token = stack->top->exp;
    sToken *deleted = stack->top;
    stack->top = deleted->prev;
    DEBUG_PRINT(("Poping Exp: "));
    free(deleted);
    print_token(token->value);
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
    while (1)
    {
        if (current == NULL || current->prev == NULL || current->prev->prev == NULL)
        {
            return NULL;
        }
        //current is token
        if (IsToken(current))
        {
            if (current->token->id == endToken)
            {
                return NULL;
            }
            current = current->prev;
        }
        //current is Exp
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
                    return NULL; //TODO: error
                }
                else
                    current = current->prev;
            }
            else
                current = current->prev;
        }
    }
    return NULL;
}

sToken *searchForDoubleExp(TokenStack *stack)
{
    sToken *current = stack->top->prev;

    if (current == NULL || current->prev == NULL)
    {
        return NULL;
    }
    if (IsToken(current) == false && IsToken(current->prev) == false)
    {
        return current;
    }
    return NULL;
}
sToken *searchForDualRule(TokenStack *stack, tID tokenID, tID endToken)
{
    sToken *current = stack->top->prev;
    while (1)
    {
        if (current == NULL || current->prev == NULL)
            return NULL;
        if (IsToken(current))
        {
            if (current->token->id == endToken)
                return NULL;
        }
        else
        {
            if (IsToken(current->prev) && current->prev->token->id == tokenID)
            {
                return current;
            }
        }
        current = current->prev;
    }
}

void ReplaceWithExp(sToken *token, Exp *exp, int delete)
{
    if (IsToken(token) == false)
    {
        token->exp = exp;
        DEBUG_PRINT(("Replacing with exp: "));
        print_token(token->exp->value);
        sToken *prev = token->prev;
        for (int i = 1; i <= delete; i++)
        {
            token->prev = prev->prev;
            free(prev);
            prev = token->prev;
        }
    }
    else
        parser_free_exit(SYN_ERR);
}

void AddSemicolom(TokenStack *stack)
{
    tToken *init_token = malloc(sizeof(tToken));
    init_token->id = ID_SEMICOLLON;
    tsPushToken(stack, init_token);
}
