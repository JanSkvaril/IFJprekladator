#ifndef SEM_H_
#define SEM_H_

#include <stdio.h>
#include <stdlib.h>
#include "../scanner/scanner.h"

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
void disposeTree(Tree *);

#endif