#include "semantics.h"
#include "../debug.h"

tID member = ID_SEMICOLLON;
tID nextMember = ID_SEMICOLLON;
int right = 0;

void membersDel()
{
    member = ID_SEMICOLLON;
    nextMember = ID_SEMICOLLON;
}

tToken *getValue(Tree *tree)
{
    return tree->value;
}

void symTabDefine(Scope *scope, tToken *name, Tree *Value)
{
    Data *dataType = malloc(sizeof(struct Data_struct));
    if (Search(scope->table, name->att.s, &dataType))
        parser_free_exit(3);

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
        Search(scope->table, Value->value->att.s, &dataType);
        data->type = (int)dataType->type;
    }
    else
        data->type = (int)Value->value->id - 1;

    Insert(&scope->table, name->att.s, data);
    printf("inserted %s, type: %d, scope: %ld \n", name->att.s, (int)data->type, scope->table->Key);
    free(dataType);
}

void checkMembersType(tID value, int error)
{
    nextMember = value;
    if (member != nextMember && member != ID_SEMICOLLON)
        parser_free_exit(error);

    member = nextMember;
}

void assignCheck(Scope *scope, Tree *tree, tID action)
{
    if (tree->value->id == ID_IDENTIFIER)
    {
        Data *dataType = malloc(sizeof(struct Data_struct));

        if (Search(scope->table, tree->value->att.s, &dataType))
            checkMembersType(dataType->type + 1, 3);
        else if (action == ID_ASSIGN || action == ID_EQ)
            identifierScopeCheck(scope, tree->value);
        else if (action == ID_DEFINE && right == 0)
            identifierScopeCheck(scope, tree->value);
    }
    if (tree->value->id < 4 && tree->value->id > 0 && action == ID_ASSIGN || tree->value->id < 4 && tree->value->id > 0 && action == ID_EQ)
        checkMembersType(tree->value->id, 5);
    else if (tree->value->id < 4 && tree->value->id > 0 && action == ID_DEFINE)
        checkMembersType(tree->value->id, 4);

    if (tree->LPtr == NULL)
        return;
    if (tree->RPtr == NULL)
        return;

    assignCheck(scope, tree->LPtr, action);
    right++;
    assignCheck(scope, tree->RPtr, action);
    right--;
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
    else
        checkMembersType(dataType->type + 1, 4);
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

int checkScopeIds(tID id)
{
    if (id == ID_KEY_IF || id == ID_KEY_FUNC || id == ID_KEY_ELSE)
        return TRUE;
    return FALSE;
}

int checkRelationIds(tID id)
{
    if (id == ID_EQ || id == ID_NEQ || id == ID_LESS || id == ID_GREATER || id == ID_LESS_EQ || id == ID_GREATER_EQ)
        return TRUE;
    return FALSE;
}

void CheckTypes(Tree *tree, scopeStack *scopeS)
{
    //tutaj pisaj
    if (tree != NULL)
    {
        if (tree->value->id == ID_DIV && tree->LPtr->value->att.i == 0)
            parser_free_exit(9);

        if (checkRelationIds(tree->value->id))
        {
            assignCheck(scopeS->top, tree, ID_EQ);
            membersDel();
        }

        if (tree->value->id == ID_DEFINE)
        {
            assignCheck(scopeS->top, tree, ID_DEFINE);
            symTabDefine(scopeS->top, tree->RPtr->value, tree->LPtr);
            membersDel();
        }

        if (tree->value->id == ID_ASSIGN)
        {
            assignCheck(scopeS->top, tree, ID_ASSIGN);
            membersDel();
        }

        if (checkScopeIds(tree->value->id) || tree->value->id == ID_KEY_FOR)
            ssAdd(scopeS);

        CheckTypes(tree->RPtr, scopeS);

        if (checkScopeIds(tree->value->id))
            ssPop(scopeS);

        CheckTypes(tree->Condition, scopeS);

        if (checkScopeIds(tree->value->id))
            ssAdd(scopeS);

        CheckTypes(tree->LPtr, scopeS);

        if (checkScopeIds(tree->value->id) || tree->value->id == ID_KEY_FOR)
            ssPop(scopeS);
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