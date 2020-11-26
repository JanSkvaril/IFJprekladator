#include "semantics.h"
#include "../debug.h"

tToken *getValue(Tree *tree)
{
    return tree->value;
}

int symTabAdd(Scope *scope, tToken *name, Tree *Value)
{
    Data *data = malloc(sizeof(struct Data_struct));
    if (data == NULL)
        return;
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

tID member = ID_SEMICOLLON;
tID nextMember = ID_SEMICOLLON;
void assignCheck(Scope *scope, Tree *tree)
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

    
    
    if(tree->LPtr == NULL)
        return;
    if(tree->RPtr == NULL)
        return;

    assignCheck(scope,tree->LPtr);
    assignCheck(scope,tree->RPtr);
}

void identifierScopeCheck(Scope *scope, tToken *term)
{
    Data *dataType = malloc(sizeof(struct Data_struct));
    if (!Search(scope->table, term->att.s, &dataType))
    {
        if (scope->prev != NULL)
            identifierScopeCheck(scope->prev, term);
        else
            parser_free_exit(3);
    }
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

void CheckTypes(Tree *tree, scopeStack *scopeS)
{
    //tutaj pisaj
    if (tree != NULL)
    {
        if(tree->value->id == ID_KEY_IF || tree->value->id == ID_KEY_FOR || tree->value->id == ID_KEY_FUNC || tree->value->id == ID_KEY_ELSE)
            ssAdd(scopeS);

        //if(tree->value->id == ID_IDENTIFIER)
            //identifierScopeCheck(scopeS->top, tree->value); //not done yet

        if (tree->value->id == ID_DEFINE)
        {
            assignCheck(scopeS->top, tree);
            symTabDefine(scopeS->top, tree->RPtr->value, tree->LPtr);
            member = ID_SEMICOLLON;
            nextMember = ID_SEMICOLLON;
        }
        
        if(tree->value->id == ID_ASSIGN)
        {
            assignCheck(scopeS->top, tree);
            DEBUG_PRINT(("type is alright\n"));
            member = ID_SEMICOLLON;
            nextMember = ID_SEMICOLLON;
        }        
            
        CheckTypes(tree->RPtr, scopeS);
        CheckTypes(tree->Condition, scopeS);
        CheckTypes(tree->LPtr, scopeS);
    }
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