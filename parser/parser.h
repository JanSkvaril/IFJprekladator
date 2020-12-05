#ifndef PARSER_H_
#define PARSER_H_

#include "token_stack.h"
#include "../scanner/scanner.h"
#include "semantics.h"
#include "scope.h"

/* Creates ASS and returns pointer to root */
Exp *Parse();

/* Checks and resolves rules in stack, returns true, if there are no more rules to chechk */
bool ResolveRules(TokenStack *, scopeStack *);
#endif