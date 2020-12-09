/**
* NÁZEV PROJEKTU:   Implementace překladače imperativního jazyka IFJ20
* ČÍSLO TÝMU:       115
* VARIANTA:         1.
*
* Michal Zavadil    login: xzavad18
* Jakub Novotný     login: xnovot2a
* Jan Škvařil       login: xskvar09
* Erik Báča         login: xbacae00
*/
#include "semantics.h"
#include "../debug.h"
#include "../error/error.h"

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

int isBuiltIn(tToken *func)
{
    char builtInFunc[][10] = {"inputs", "inputi", "inputf", "print", "int2float", "float2int", "len", "substr", "ord", "chr"};

    for(size_t i = 0; i < sizeof(builtInFunc) / sizeof(builtInFunc[0]); i++)
    {

         if(!strcmp(func->att.s, builtInFunc[i]))
            return TRUE;
    }
   return FALSE;
}

int varNumber(Tree *tree)
{
    if (tree == NULL)
        return 0;
    if (tree->RPtr == NULL && tree->LPtr == NULL)
        return 1;
    else
        return varNumber(tree->RPtr) + varNumber(tree->LPtr);
}

void symTabDefine(Scope *scope, tToken *name, Tree *Value)
{
    Data *dataType;
    if (Search(scope->table, name->att.s, &dataType))
        parser_free_exit(3);

    Data *data = malloc(sizeof(struct Data_struct));
    if (data == NULL)
        return;

    data->paramsNumber = 0;
    data->params = NULL;
    data->returnsNumber = 0;
    data->returns = NULL;
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
    DEBUG_PRINT(("inserted %s, type: %d, scope: %ld \n", name->att.s, (int)data->type, scope->table->Key));
}

void checkMembersType(tID value, int error)
{
    nextMember = value;
    if (member != nextMember && member != ID_SEMICOLLON)
    {
        parser_free_exit(error);
    }


    member = nextMember;
}

void identifierScopeCheck(Scope *scope, tToken *term)
{
    Data *dataType;
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

void checkReturnTypes(Scope *scope, Tree* root1, Tree* root2){
	if(root1->LPtr==NULL && root2->LPtr==NULL && root1->RPtr==NULL && root2->RPtr==NULL)
    {
        if(root1->value->id == ID_UNDER)
            return;

        Data *dataType;
        if(!Search(scope->table, root1->value->att.s, &dataType))
            parser_free_exit(3);

        if(dataType->type+1 != root2->value->id-3)
            parser_free_exit(6);

        return;
    }

	if(root1->LPtr==NULL || root2->LPtr==NULL || root1->RPtr==NULL || root2->RPtr==NULL)
        parser_free_exit(6);

	checkReturnTypes(scope, root1->LPtr,root2->LPtr);
    checkReturnTypes(scope, root1->RPtr,root2->RPtr);
}

void assignCheck(Scope *scope, Tree *tree, tID action)
{

    if (tree->LPtr != NULL)
        if (tree->LPtr->value->id == ID_FUNC_CALL)
        {
            Scope *tmp = scope;
            while (tmp->prev != NULL)
            {
                tmp = tmp->prev;
            }
            if(!isBuiltIn(tree->LPtr->LPtr->value))
            {
                Data *dataType;
                Search(tmp->table, tree->LPtr->LPtr->value->att.s, &dataType);
                if (varNumber(tree->RPtr) != dataType->returnsNumber)
                    parser_free_exit(6);

                checkReturnTypes(scope, tree->RPtr,dataType->returns);
            }
            return;
        }
    if (tree->value->id == ID_IDENTIFIER)
    {
        Data *dataType;

        if (Search(scope->table, tree->value->att.s, &dataType))
            checkMembersType(dataType->type + 1, 3);
        else if (action == ID_ASSIGN || action == ID_EQ)
            identifierScopeCheck(scope, tree->value);
        else if (action == ID_DEFINE && right == 0)
            identifierScopeCheck(scope, tree->value);
    }
    if ((tree->value->id < 4 && tree->value->id > 0 && action == ID_ASSIGN) || (tree->value->id < 4 && tree->value->id > 0 && action == ID_EQ))
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

Tree *makeLeaf(tToken *term)
{
    Tree *newTree = malloc(sizeof(struct T));
    newTree->value = term;
    newTree->LPtr = NULL;
    newTree->Condition = NULL;
    newTree->RPtr = NULL;
    return newTree;
}

Tree *makeTree(Tree *x, Tree *y, tToken *op)
{
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
        return AddToIfTree(mainTree->RPtr, minorTree);
    }
    return mainTree;
}

int checkScopeIds(tID id)
{
    if (id == ID_KEY_IF || id == ID_KEY_ELSE)
        return TRUE;
    return FALSE;
}

int checkRelationIds(tID id)
{
    if (id == ID_EQ || id == ID_NEQ || id == ID_LESS || id == ID_GREATER || id == ID_LESS_EQ || id == ID_GREATER_EQ)
        return TRUE;
    return FALSE;
}

void symTabDefineFunction(Scope *scope, tToken *name, Tree *Value)
{
    Data *dataType;
    if (Search(scope->table, name->att.s, &dataType))
        parser_free_exit(3);

    Data *data = malloc(sizeof(struct Data_struct));
    if (data == NULL)
        return;

    data->type = FUNC;
    data->paramsNumber = varNumber(Value->LPtr) / 2;
    data->params = Value->LPtr;
    data->returnsNumber = varNumber(Value->RPtr);

    data->returns = Value->RPtr;

    Insert(&scope->table, name->att.s, data);
    DEBUG_PRINT(("inserted %s, type: %d, scope: %ld, params:%d \n", name->att.s, (int)data->type, scope->table->Key, data->paramsNumber));
}

void defineFunctions(Tree *tree, scopeStack *scopeS)
{
    if (tree != NULL)
    {
        if (tree->value->id == ID_KEY_FUNC)
            symTabDefineFunction(scopeS->top, tree->LPtr->value, tree->Condition);

        defineFunctions(tree->RPtr, scopeS);
        defineFunctions(tree->Condition, scopeS);
        defineFunctions(tree->LPtr, scopeS);
    }
}

void checkParamTypes(Scope *scope, Tree *root1, Tree *root2)
{
	if(root1->value->id == ID_TYPE_DEF && root2->LPtr==NULL && root2->RPtr==NULL)
    {
        if(root2->value->id == ID_IDENTIFIER)
        {
            Data *dataType;
            if(Search(scope->table, root2->value->att.s, &dataType))
                if(root1->RPtr->value->id-3 != dataType->type+1)
                    parser_free_exit(6);
        }
        else if(root1->RPtr->value->id-3 != root2->value->id)
            parser_free_exit(6);
		return;
    }

	if(root1->value->id == ID_TYPE_DEF || root2->LPtr==NULL || root2->RPtr==NULL)
        parser_free_exit(6);

	checkParamTypes(scope, root1->LPtr, root2->LPtr);
    checkParamTypes(scope, root1->RPtr, root2->RPtr);
}

void funcCheck(Scope *scope, Tree *Value)
{
    Scope *tmp = scope;
    while (tmp->prev != NULL)
    {
        tmp = tmp->prev;
    }
    Data *dataType;
    if (!Search(tmp->table, Value->LPtr->value->att.s, &dataType))
    {
        if(!isBuiltIn(Value->LPtr->value))
            parser_free_exit(3);
    }
    else if (Value->RPtr != NULL)
    {
        if (dataType->paramsNumber != (varNumber(Value->RPtr)))
            parser_free_exit(6);
        checkParamTypes(scope, dataType->params, Value->RPtr);
    }
    else if (dataType->paramsNumber != 0)
        parser_free_exit(6);
}

void defineFuncParam(Scope *scope, Tree *tree)
{

    if (tree != NULL)
    {
        if (tree->value->id == ID_TYPE_DEF)
        {
            Data *dataType;
            if (Search(scope->table, tree->LPtr->value->att.s, &dataType))
                parser_free_exit(3);

            Data *data = malloc(sizeof(struct Data_struct));
            if (data == NULL)
                return;

            data->paramsNumber = 0;
            data->params = NULL;
            data->returnsNumber = 0;
            data->returns = NULL;

            data->type = tree->RPtr->value->id - 4;

            Insert(&scope->table, tree->LPtr->value->att.s, data);
            DEBUG_PRINT(("inserted %s, type: %d, scope: %ld \n", tree->LPtr->value->att.s, (int)data->type, scope->table->Key));
        }
        defineFuncParam(scope, tree->RPtr);
        defineFuncParam(scope, tree->LPtr);
    }
}

void defineFuncParams(Scope *scope, Tree *tree)
{
    Scope *tmp = scope;
    while (tmp->prev != NULL)
    {
        tmp = tmp->prev;
    }
    Data *dataType;
    Search(tmp->table, tree->LPtr->value->att.s, &dataType);

    defineFuncParam(scope, dataType->params);
}

void checkMain(Scope *scope)
{
    Data *dataType;
    if(!Search(scope->table, "main", &dataType))
        parser_free_exit(7);
}

void CheckTypes(Tree *tree, scopeStack *scopeS)
{
    if (tree != NULL)
    {
        if (tree->value->id == ID_DIV && tree->LPtr->value->att.i == 0)
            parser_free_exit(9);

        if (tree->value->id == ID_FUNC_CALL)
        {
            funcCheck(scopeS->top, tree);
        }

        if (tree->value->id == ID_KEY_FUNC)
        {
            ssAdd(scopeS);
            defineFuncParams(scopeS->top, tree);
        }

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

        if (checkScopeIds(tree->value->id) || tree->value->id == ID_KEY_FOR || tree->value->id == ID_KEY_FUNC)
            ssPop(scopeS);
    }
}

void disposeTree(Tree *tree)
{
    if (tree != NULL)
    {
        disposeTree(tree->LPtr);
        disposeTree(tree->Condition);
        disposeTree(tree->RPtr);
        free(tree);
        tree = NULL;
    }
}
