#ifndef SEM_H_
#define SEM_H_
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../scanner/scanner.h"
#include "scope.h"
#include "../sym_table/sym_table.h"
#include "scope.h"

typedef struct T
{
    tToken *value;
    struct T *LPtr;
    struct T *Condition;
    struct T *RPtr;
} Tree;

tToken *getValue(Tree *);
Tree *makeLeaf(tToken *);
Tree *makeTree(Tree *, Tree *, tToken *);
Tree *makeIfTree(Tree *tru, Tree *cond, Tree *fals, tToken *token);
Tree *AddToIfTree(Tree *, Tree *);
void checkMain(Scope *);
void disposeTree(Tree *);
void CheckTypes(Tree *, scopeStack *);
void defineFunctions(Tree *, scopeStack *);
#endif
