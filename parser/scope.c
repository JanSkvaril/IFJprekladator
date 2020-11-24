#include "scope.h"

void ssInit(scopeStack *stack)
{
    stack->top = NULL;
}

void ssAdd(scopeStack *stack)
{
    SymTable *newTable = malloc(sizeof(SymTable));
    if (newTable == NULL)
        return; //TODO: error?
    Init(&newTable);

    Scope *scope = malloc(sizeof(Scope));
    if (scope == NULL)
        return; //TODO: error?
    scope->table = newTable;
    scope->prev = stack->top;
    stack->top = scope;
}

void ssPop(scopeStack *stack)
{
    if (stack->top != NULL)
    {
        Scope *deleted = stack->top;
        Dispose(&(deleted->table));
        stack->top = deleted->prev;
        free(deleted);
    }
}

void ssDispose(scopeStack *stack)
{
    while (stack->top != NULL)
    {
        ssPop(stack);
    }
}