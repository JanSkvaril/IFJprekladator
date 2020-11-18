//zdroooooooooooooooooooooooooooooooj!!
#include "sym_table.h"

/* 
** Funkce provede počáteční inicializaci stromu před jeho prvním použitím.
**/
void BSTInit (NodePtr *RootPtr) {
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
int BSTSearch (NodePtr RootPtr, char K, Data *Content)	{
	if(RootPtr == NULL)
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

/*
** Vloží do stromu RootPtr Content s klíčem K.
**
** Pokud již uzel se zadaným klíčem ve stromu existuje, bude obsah uzlu
** s klíčem K nahrazen novou hodnotou. Pokud bude do stromu vložen nový
** uzel, bude vložen vždy jako list stromu.
**/
void BSTInsert (NodePtr* RootPtr, char K, Data Content)	{
	if(*RootPtr == NULL)
	{
		NodePtr new = malloc(sizeof(struct Node));
		if(new==NULL)
			return;
		new->data = Content;
		new->Key = K;
		new->RPtr = NULL;
		new->LPtr = NULL;
		(*RootPtr) = new;
	} 
	else
	{
		if(K == (*RootPtr)->Key)
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
void ReplaceByRightmost (NodePtr PtrReplaced, NodePtr *RootPtr) {
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
void BSTDelete (NodePtr *RootPtr, char K) 		{
if((*RootPtr) != NULL) {
		if((*RootPtr)->Key>K)
			BSTDelete(&(*RootPtr)->LPtr, K);
		else if((*RootPtr)->Key<K)
			BSTDelete(&(*RootPtr)->RPtr, K);
		else 
		{
			NodePtr old = *RootPtr;
			
			if((*RootPtr)->RPtr != NULL && (*RootPtr)->LPtr != NULL) 
				ReplaceByRightmost((*RootPtr), &(*RootPtr)->LPtr);
			else if((*RootPtr)->RPtr == NULL)	
			{	
				free(old);					
				*RootPtr = (*RootPtr)->LPtr;
			} 
			else if((*RootPtr)->LPtr == NULL)	
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
void BSTDispose (NodePtr *RootPtr) {	
	if(*RootPtr != NULL)
	{
		BSTDispose(&(*RootPtr)->LPtr);
		BSTDispose(&(*RootPtr)->RPtr);
		free(*RootPtr);
		(*RootPtr) = NULL;
	}
}

void main()
{

}