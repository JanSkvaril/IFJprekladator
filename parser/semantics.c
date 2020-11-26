#include "semantics.h"

tToken *getValue(Tree *tree)
{
    return tree->value;
}

int symTabAdd(Scope *scope, tToken *name, Tree *Value)
{
    Data *data = malloc(sizeof(struct Data_struct));
    data->defined = TRUE;
    data->paramsNumber = 0;
    while (Value->LPtr != NULL)
    {
        Value = Value->LPtr;
    }
    if (Value->value->id == ID_IDENTIFIER)
    {
        Data dataType;
        Search(scope->table, Value->value->att.s, &dataType);
        data->type = dataType.type;
    }
    else
        data->type = Value->value->id;
    Insert(&scope->table, name->att.s, data);
}

int identifierScopeCheck(Scope *scope, tToken *term, tToken *Value)
{
    Data data;
    if (Search(scope->table, term->att.s, &data))
    {
        return TRUE;
    }
    else
    {
        if (scope->prev != NULL)
            identifierScopeCheck(scope->prev, term, Value);
    }
    return FALSE;
}

//int identifierValueCheck(Scope *scope, Tree *Value)
//{

//}

Tree *makeLeaf(tToken *term)
{
    Tree *newTree = malloc(sizeof(struct T));
    newTree->value = term;
    newTree->LPtr = NULL;
    newTree->Condition = NULL;
    newTree->RPtr = NULL;
    return newTree;
}

Tree *makeTree(Tree *x, Tree *y, tToken *op, Scope *scope)
{
    if (op->id == ID_DEFINE)
        symTabAdd(scope, x->value, y);
    //if(op->id == ID_ASSIGN)
    //identifierScopeCheck(scope,x, y);
    //if(identifierCheck(scope))
    Tree *newTree = malloc(sizeof(struct T));
    newTree->value = op;
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

Tree *AddToIfTree(Tree *mainTree, Tree *minorTree)
{
    if (mainTree->RPtr == NULL)
    {
        mainTree->RPtr = minorTree;
    }
    else
    {
        *AddToIfTree(mainTree->RPtr, minorTree);
    }
    return mainTree;
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

void CheckTypes(Tree *tree)
{
    printf("Type check started\n");
    //tutaj pisaj
}