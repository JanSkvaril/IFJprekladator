//zdroooooooooooooooooooooooooooooooj!!
#include "sym_table.h"

unsigned long hash(char *str)
{

	unsigned long hash = 5381;
	int c;
	while ((c = *str++))
		hash = ((hash << 5) + hash) + c;
	return hash;
}

/* 
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**/
void Init(NodePtr *RootPtr)
{
	*RootPtr = NULL;
}

/*
** Funkce vyhledá uzel v BVS s klíčem K.
**
** Pokud je takový nalezen, vrací funkce hodnotu TRUE a v proměnné Content se
** vrací obsah příslušného uzlu.´Pokud příslušný uzel není nalezen, vrací funkce
** hodnotu FALSE a obsah proměnné Content není definován (nic do ní proto
** nepřiřazujte).
**/
int BSTSearch(NodePtr RootPtr, unsigned long K, Data **Content)
{
	if (RootPtr == NULL)
		return FALSE;
	//printf("key is: %ld", RootPtr->Key);
	if (RootPtr->Key == K)
	{
		*Content = RootPtr->data;
		//printf("typ %d paramNum %d\n", RootPtr->data->type, RootPtr->data->paramsNumber);
		return TRUE;
	}

	if (RootPtr->Key < K)
		return BSTSearch(RootPtr->RPtr, K, Content);

	if (RootPtr->Key > K)
		return BSTSearch(RootPtr->LPtr, K, Content);

	return FALSE;
}

/*
** Vloží do stromu RootPtr Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**/
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

/*
** Pomocná funkce pro vyhledání, přesun a uvolnění nejpravějšího uzlu.
**
** Ukazatel PtrReplaced ukazuje na uzel, do kterého bude přesunuta hodnota
** nejpravějšího uzlu v podstromu, který je určen ukazatelem RootPtr.
** Předpokládá se, že hodnota ukazatele RootPtr nebude NULL (zajistěte to
** testováním před volání této funkce).
**/
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

/*
** Zruší uzel stromu, který obsahuje klíč K.
**
** Pokud uzel se zadaným klíčem neexistuje, nedělá funkce nic.
** Pokud má rušený uzel jen jeden podstrom, pak jej zdědí otec rušeného uzlu.
** Pokud má rušený uzel oba podstromy, pak je rušený uzel nahrazen nejpravějším
** uzlem levého podstromu. Pozor! Nejpravější uzel nemusí být listem.
**/
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

/*
** Zruší celý binární vyhledávací strom a korektně uvolní paměť.
**/
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
	//printf("\n\n\n\n%s\n", k);
	unsigned long key = hash(k);
	int result = BSTSearch(RootPtr, key, Content);
	//printf("TFJDKJFKD %d\n", (*Content)->paramsNumber);
	return result;
}

void Delete(NodePtr *RootPtr, char *k)
{
	if (k == NULL)
		return;
	unsigned long key = hash(k);
	BSTDelete(RootPtr, key);
}

/*

void main()
{
	Data a;
	NodePtr x;
	Init(&x);
	char *s ="erik";

	printf("%d",Search(x,s,&a));
	Insert(&x,s,&a);
	printf("%d",Search(x,s,&a));
	Delete(&x,s);
	printf("%d",Search(x,s,&a));

	Insert(&x,s,&a);
	Insert(&x,"kire",&a);
	Insert(&x,"emil",&a);
	Insert(&x,"era",&a);
	
	printf("\n");
	
	printf("%d",Search(x,s,&a));
	printf("%d",Search(x,"kire",&a));
	printf("%d",Search(x,"emil",&a));
	printf("%d",Search(x,"era",&a));
	printf("%d",Search(x,"erk",&a));

	printf("\n");
	Dispose(&x);

	printf("%d",Search(x,s,&a));
	printf("%d",Search(x,"kire",&a));
	printf("%d",Search(x,"emil",&a));
	printf("%d",Search(x,"era",&a));
	printf("%d",Search(x,"erk",&a));
}
*/