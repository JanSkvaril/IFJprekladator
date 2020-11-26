#include "semantics.h"

tToken *getValue(Tree *tree)
{
    return tree->value;
}

void symTabDefine(Scope *scope, tToken *name, Tree *Value)
{    
    Data *data = malloc(sizeof(struct Data_struct));
    if(data == NULL)
        return;
    data->defined = TRUE;
    data->paramsNumber = 0;
    while (Value->LPtr != NULL)
    {
        Value = Value->LPtr;
    }
    if(Value->value->id==0)
    {
        Data *dataType = malloc(sizeof(struct Data_struct));
        
        printf("searched: %d \n",Search(scope->table,Value->value->att.s, dataType));
        data->type = (int)dataType->type;
        printf("searched type: %d \n", (int)data->type);
    } else
        data->type = (int)Value->value->id - 1;
    
    Insert(&scope->table,name->att.s,data);
    //printf("inserted %s ",name->att.s);
    //printf(", type: %d \n", (int)data->type);
}


int getTreeType(Scope *scope, Tree *tree, tID treeType)
{    
    if(tree->LPtr != NULL)
    {
        if(tree->LPtr->value->id == ID_IDENTIFIER)
        {
            Data dataType;
            Search(scope->table,tree->value->att.s, &dataType);
            //data->type = dataType.type;
            //data->value = dataType.value;
        }
        else if(tree->value->id != tree->LPtr->value->id)
            return FALSE;
        getTreeType(scope, tree->LPtr, treeType);
    }
    if(tree->RPtr != NULL)
    {

        getTreeType(scope, tree->RPtr, treeType);
    }
}

int identifierScopeCheck(Scope *scope, tToken *term, tToken *Value)
{
    Data data;
    if(Search(scope->table,term->att.s, &data))
    {
        return TRUE;
    } 
    else
    {
        if(scope->prev != NULL)
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
    if(mainTree->RPtr == NULL)
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
    //printf("Type check started\n");
    //tutaj pisaj
    if(tree != NULL) {
        //ssAdd(scopeS);

        
        if(tree->value->id == ID_DEFINE)
            symTabDefine(scopeS->top, tree->RPtr->value, tree->LPtr);
            
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