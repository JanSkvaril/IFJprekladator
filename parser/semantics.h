#ifndef SEM_H_
#define SEM_H_

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
