#include "sym_table.h"

unsigned long hash(char *str)
{

	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return hash;
}

void Init(NodePtr *RootPtr)
{
	*RootPtr = NULL;
}

int BSTSearch(NodePtr RootPtr, unsigned long K, Data **Content)
{
	if (RootPtr == NULL)
		return FALSE;
	if (RootPtr->Key == K)
	{
		*Content = RootPtr->data;
		return TRUE;
	}

	if (RootPtr->Key < K)
		return BSTSearch(RootPtr->RPtr, K, Content);

	if (RootPtr->Key > K)
		return BSTSearch(RootPtr->LPtr, K, Content);

	return FALSE;
}

void BSTInsert(NodePtr *RootPtr, unsigned long K, Data *Content)
{
	if (*RootPtr == NULL)
	{
		NodePtr new = malloc(sizeof(struct Node) + sizeof(Content));
		if (new == NULL)
			return;
		new->data = Content;
		new->Key = K;
		new->RPtr = NULL;
		new->LPtr = NULL;
		(*RootPtr) = new;
	}
	else
	{
		if (K == (*RootPtr)->Key)
			(*RootPtr)->data = Content;
		else if (K < (*RootPtr)->Key)
			BSTInsert(&(*RootPtr)->LPtr, K, Content);
		else if (K > (*RootPtr)->Key)
			BSTInsert(&(*RootPtr)->RPtr, K, Content);
	}
}

void ReplaceByRightmost(NodePtr PtrReplaced, NodePtr *RootPtr)
{
	if (*RootPtr != NULL)
	{
		if ((*RootPtr)->RPtr == NULL)
		{
			PtrReplaced->Key = (*RootPtr)->Key;
			PtrReplaced->data = (*RootPtr)->data;

			NodePtr old = *RootPtr;
			(*RootPtr) = (*RootPtr)->LPtr;
			free(old);
		}
		else
			ReplaceByRightmost(PtrReplaced, &(*RootPtr)->RPtr);
	}
}

void BSTDelete(NodePtr *RootPtr, unsigned long K)
{
	if ((*RootPtr) != NULL)
	{
		if ((*RootPtr)->Key > K)
			BSTDelete(&(*RootPtr)->LPtr, K);
		else if ((*RootPtr)->Key < K)
			BSTDelete(&(*RootPtr)->RPtr, K);
		else
		{
			NodePtr old = *RootPtr;

			if ((*RootPtr)->RPtr != NULL && (*RootPtr)->LPtr != NULL)
				ReplaceByRightmost((*RootPtr), &(*RootPtr)->LPtr);
			else if ((*RootPtr)->RPtr == NULL)
			{
				free(old);
				*RootPtr = (*RootPtr)->LPtr;
			}
			else if ((*RootPtr)->LPtr == NULL)
			{
				free(old);
				*RootPtr = (*RootPtr)->RPtr;
			}
			else
			{
				free(old);
				*RootPtr = NULL;
			}
		}
	}
}

void Dispose(NodePtr *RootPtr)
{
	if (*RootPtr != NULL)
	{
		Dispose(&(*RootPtr)->LPtr);
		Dispose(&(*RootPtr)->RPtr);
		free(*RootPtr);
		(*RootPtr) = NULL;
	}
}

void Insert(NodePtr *RootPtr, char *k, Data *Content)
{
	if (k == NULL)
		return;
	unsigned long key = hash(k);
	BSTInsert(RootPtr, key, Content);
}

int Search(NodePtr RootPtr, char *k, Data **Content)
{
	if (k == NULL)
		return -1;
	unsigned long key = hash(k);
	int result = BSTSearch(RootPtr, key, Content);
	return result;
}

void Delete(NodePtr *RootPtr, char *k)
{
	if (k == NULL)
		return;
	unsigned long key = hash(k);
	BSTDelete(RootPtr, key);
}