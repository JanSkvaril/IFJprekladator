#include "token_stack.h"

/* Creates ASS and returns pointer to root */
Exp *Parse();

/* Checks and resolves rules in stack, returns true, if there are no more rules to chechk */
bool ResolveRules(TokenStack *);
