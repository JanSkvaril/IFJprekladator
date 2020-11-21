#include "token_stack.h"

/* Creates ASS and returns pointer to root */
Exp *Parse();

/* Checks and resolves rules in stack */
void ResolveRules(TokenStack *);
