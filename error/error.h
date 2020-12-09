#ifndef ERROR_H_
#define ERROR_H_
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

#include "../scanner/scanner.h"
typedef enum
{
	LEX_ERR = 1,
	SYN_ERR = 2,
	SEM_ERR3 = 3,
	SEM_ERR4 = 4,
	SEM_ERR5 = 5,
	SEM_ERR6 = 6,
	SEM_ERR7 = 7,
	DEV_ZERO_ERR = 9,
	INTER_ERR = 99,
}tErrCode;


void scanner_free_exit(tTokenPtr, tErrCode);

void parser_free_exit(tErrCode errCode);



#endif
