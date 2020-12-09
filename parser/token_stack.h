#ifndef TS_H_
#define TS_H_
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
#include "./semantics.h"
#include "../scanner/scanner.h"
#include "../error/error.h"
#include <stdbool.h>
//placeholder - tohle bude v .h semantických akcí
typedef Tree Exp;
typedef struct sTokenStruct
{
    struct sTokenStruct *prev;
    tToken *token;
    Exp *exp;
} sToken;

typedef struct
{
    sToken *top;
} TokenStack;

/* Sets top to NULL */
void tsInit(TokenStack *);
/* Adds token to stack */
void tsPushToken(TokenStack *, tToken *);
/* Adds expresion to stack */
void tsPushExp(TokenStack *, Exp *);
/* Removes and returns top of the stack */
tToken *tsPopToken(TokenStack *);
/* Removes and returns top of the stack */
Exp *tsPopExp(TokenStack *);
/* Removes everything from the stack */
void tsDispose(TokenStack *);
/* Returns true if stack token is token and false if expression */
bool IsToken(sToken *);

/* Search for aritmetic rule until endToken */
sToken *searchForRule(TokenStack *stack, tID token, tID endToken);

/* Replaces sToken with exp and delete some number of sTokens before it */
void ReplaceWithExp(sToken *token, Exp *exp, int delete);

/* Adds semicolon onto stack */
void AddSemicolom(TokenStack *stack);

sToken *searchForDoubleExp(TokenStack *stack);
sToken *searchForDualRule(TokenStack *stack, tID tokenID, tID endToken);
#endif
