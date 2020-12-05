//zdroooooooooooooooooooooooooooooooj!!
#ifndef SYM_H_
#define SYM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#define TRUE 1
#define FALSE 0
typedef enum
{
	INT,
	DOUBLE,
	STRING
} TYPE;

union Value
{
	int i;
	double f;
	char *s;
};

typedef struct Data_struct
{
	TYPE type;
	int defined;
	union Value value;
	int paramsNumber;
	char params[];
} Data;

typedef struct Node
{
	unsigned long Key; /* klíč */
	Data data;		   /* užitečný obsah uzlu */
	struct Node *LPtr; /* levý podstrom */
	struct Node *RPtr; /* pravý podstrom */
} * NodePtr;

/* prototypy funkcí */

void Init(NodePtr *);
int Search(NodePtr, char *, Data *);
void Insert(NodePtr *, char *, Data *);
void Delete(NodePtr *, char *);
void Dispose(NodePtr *);

#endif