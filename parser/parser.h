#ifndef PARSER_H_
#define PARSER_H_
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
#include "token_stack.h"
#include "../scanner/scanner.h"
#include "semantics.h"
#include "scope.h"
#include "../error/error.h"

/* Creates ASS and returns pointer to root */
Exp *Parse();

/* Checks and resolves rules in stack, returns true, if there are no more rules to chechk */
bool ResolveRules(TokenStack *);
#endif
