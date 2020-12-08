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
	FLOAT,
	STRING,
	FUNC
} TYPE;

typedef struct Data_struct
{
	TYPE type;
	int paramsNumber;
	struct T *params;
	int returnsNumber;
	struct T *returns;
} Data;

typedef struct Node
{
	unsigned long Key;
	Data *data;
	struct Node *LPtr;
	struct Node *RPtr;
} * NodePtr;

void Init(NodePtr *);
int Search(NodePtr, char *, Data **);
void Insert(NodePtr *, char *, Data *);
void Delete(NodePtr *, char *);
void Dispose(NodePtr *);

#endif