#include "semantics.h"

TreePtr *makeLeaf(tToken term)
{
    TreePtr *newTree = malloc(sizeof(struct Tree));
    newTree->value = term;
    newTree->LPtr = NULL;
    newTree->RPtr = NULL;
    return newTree;
}

TreePtr *makeTree(TreePtr *x, TreePtr *y, tToken term)
{
    TreePtr *newTree = malloc(sizeof(struct Tree));
    newTree->value = term;
    newTree->LPtr = x;
    newTree->RPtr = y;
    return newTree;
}

void disposeTree(TreePtr *tree)
{
    if(tree != NULL)
	{
		disposeTree(tree->LPtr);
		disposeTree(tree->RPtr);
		free(tree);
		tree = NULL;
	}
}