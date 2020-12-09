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
#include <stdlib.h>
#include <stdio.h>
#include "error.h"

void scanner_free_exit(tTokenPtr token, tErrCode errCode)
{
	free(token);
	parser_free_exit(errCode);
}

//pls doplnit uvolneni pameti pouzivanou parserem
void parser_free_exit(tErrCode errCode)
{

	fprintf(stderr, "ended with error code %d\n", errCode);
	exit(errCode);
}
