#include "semantics.h"

tToken *getValue(Tree *tree)
{
    return tree->value;
}

Tree *makeLeaf(tToken *term)
{
    Tree *newTree = malloc(sizeof(struct T));
    newTree->value = term;
    newTree->LPtr = NULL;
    newTree->Condition = NULL;
    newTree->RPtr = NULL;
    return newTree;
}

Tree *makeTree(Tree *x, Tree *y, tToken *term)
{
    Tree *newTree = malloc(sizeof(struct T));
    newTree->value = term;
    newTree->LPtr = x;
    newTree->RPtr = y;
    return newTree;
}

Tree *makeIfTree(Tree *trueTree, Tree *cond, Tree *falseTree, tToken *term)
{
    Tree *newTree = malloc(sizeof(struct T));
    newTree->value = term;
    newTree->LPtr = trueTree;
    newTree->Condition = cond;
    newTree->RPtr = falseTree;
    return newTree;
}

void disposeTree(Tree *tree)
{
    if (tree != NULL)
    {
        disposeTree(tree->LPtr);
        disposeTree(tree->RPtr);
        free(tree);
        tree = NULL;
    }
}