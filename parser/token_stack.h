#include "../scanner/scanner.h"
#include <stdbool.h>
//placeholder - tohle bude v .h semantických akcí
typedef struct
{

} Exp;
typedef struct
{
    sToken *prev;
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
Exp *searchForRule(TokenStack *stack, tID token, tID endToken);

/* Replaces sToken with exp and delete some number of sTokens before it */
void ReplaceWithExp(sToken *token, Exp *exp, int delete);
