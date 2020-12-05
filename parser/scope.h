#ifndef SCOPE_H_
#define SCOPE_H_
#include "../sym_table/sym_table.h"

typedef struct Node SymTable;
typedef struct scope_struct
{
    SymTable *table;
    struct scope_struct *prev;
} Scope;
typedef struct
{
    Scope *top;
} scopeStack;

/* Sets top to NULL */
void ssInit(scopeStack *);
/* Adds scope to stack */
void ssAdd(scopeStack *);
/* Pops scope from stack */
void ssPop(scopeStack *);

void ssDispose(scopeStack *);

#endif