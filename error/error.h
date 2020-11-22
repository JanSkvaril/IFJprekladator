#ifndef ERROR_H_
#define ERROR_H_

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