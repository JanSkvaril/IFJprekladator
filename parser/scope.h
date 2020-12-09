#ifndef SCOPE_H_
#define SCOPE_H_
/**
* NÁZEV PROJEKTU:   Implementace překladače imperativního jazyka IFJ20
* ČÍSLO TÝMU:       115
* VARIANTA:         1.
*
* Michal Zavadil    login: xzavad18
* Jakub Novotný     login: xnovot2a
* Jan Škvařil       login: xskvar09
* Erik Báča         login: xbacae00
*/
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
