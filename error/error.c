#include <stdlib.h>
#include <stdio.h>
#include "error.h"

void scanner_free_exit(tTokenPtr token, tErrCode errCode)
{
	//free(token);
	//parser_free_exit(errCode);
}

//pls doplnit uvolneni pameti pouzivanou parserem
void parser_free_exit(tErrCode errCode)
{
	//fprintf(stderr, "ended with error code %d\n", errCode);
	//exit(errCode);
}
