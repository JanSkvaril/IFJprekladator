#include "semantics.h"

tToken *getValue(Tree *tree)
{
    return tree->value;
}

void symTabDefine(Scope *scope, tToken *name, Tree *Value)
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
        Data *dataType = malloc(sizeof(struct Data_struct));
        Search(scope->table, Value->value->att.s, &dataType);
        data->type = (int)dataType->type;
    }
    else
        data->type = (int)Value->value->id - 1;

    Insert(&scope->table, name->att.s, data);
    printf("inserted %s, type: %d \n",name->att.s, (int)data->type);
}

tID member = ID_SEMICOLLON;
tID nextMember = ID_SEMICOLLON;
void assignCheck(Scope *scope, Tree *tree)
{
    if(tree->value->id == ID_IDENTIFIER)
    {
        Data *dataType = malloc(sizeof(struct Data_struct));
        if(Search(scope->table, tree->value->att.s, &dataType))
        { 
            nextMember = dataType->type+1;
            if(member!=nextMember && member != ID_SEMICOLLON)
                parser_free_exit(3);
            member = nextMember;
            
        }  
    }
    if(tree->value->id < 4 && tree->value->id > 0)
    {
        nextMember = tree->value->id;
        if(member!=nextMember && member != ID_SEMICOLLON)
            parser_free_exit(3);
        member = nextMember;
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
    //if(op->id == ID_DEFINE)
    //symTabAdd(scope, y->value,x);

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
            printf("type is alright\n");
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