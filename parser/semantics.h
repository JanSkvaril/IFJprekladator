#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include "../scanner/scanner.h"

typedef struct Tree { 
    tToken value;
    struct Tree * LPtr;
	struct Tree * RPtr;  
} TreePtr;

TreePtr *makeLeaf(tToken);
TreePtr *makeTree(TreePtr *, TreePtr *, tToken);
void disposeTree(TreePtr *);

