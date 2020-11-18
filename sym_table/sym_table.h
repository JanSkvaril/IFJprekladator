//zdroooooooooooooooooooooooooooooooj!!

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define TRUE 1
#define FALSE 0
typedef enum {INT, FLOAT} TYPE;

union Value {
   int i;
   float f;
};

typedef struct Data_struct {
	TYPE type;
	union Value value;
} Data;

typedef struct Node {
	char Key;			                                                      /* klíč */
	Data data;                                            /* užitečný obsah uzlu */
	struct Node * LPtr;                                    /* levý podstrom */
	struct Node * RPtr;                                   /* pravý podstrom */
} *NodePtr;

/* prototypy funkcí */

void BSTInit   (NodePtr *);
int BSTSearch  (NodePtr, char, Data *);
void BSTInsert (NodePtr *, char, Data);
void BSTDelete (NodePtr *, char);
void BSTDispose(NodePtr *);