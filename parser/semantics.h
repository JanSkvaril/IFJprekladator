#ifndef SEM_H_
#define SEM_H_

#include <stdio.h>
#include <stdlib.h>
#include "../scanner/scanner.h"

typedef struct T
{
    tToken *value;
    struct Tree *LPtr;
    struct Tree *RPtr;
} Tree;

Tree *makeLeaf(tToken *);
Tree *makeTree(Tree *, Tree *, tToken *);
void disposeTree(Tree *);

#endif