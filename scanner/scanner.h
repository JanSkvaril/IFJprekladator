#ifndef SCANNER_H_
#define SCANNER_H_
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
#include <stdint.h> //int64

//token return value type
typedef enum
{
	RET_OK,
	RET_EOF
} tTokenRet;

//eol flag type
typedef enum
{
	EOL_REQ,
	EOL_OPT,
	EOL_FORBID
} tEolFlag;

//token ID type
typedef enum
{
	ID_IDENTIFIER, //identifier of a variable, function ...
	ID_INT_LIT,	   //integer literal e.g. 10
	ID_FLOAT_LIT,  //float literal e.g. 10.2
	ID_STRING_LIT, //string literal
	//keywords
	ID_KEY_INT,
	ID_KEY_FLOAT64,
	ID_KEY_STRING,
	ID_KEY_FUNC,
	ID_KEY_RETURN,
	ID_KEY_PACKAGE,
	ID_KEY_IF,
	ID_KEY_ELSE,
	ID_KEY_FOR,
	//brackets
	ID_ROUND_1, // (
	ID_ROUND_2, // )
	ID_CURLY_1, // {
	ID_CURLY_2, // }
	//arithmetic operators
	ID_ADD,	 // +
	ID_SUB,	 // -
	ID_MULT, // *
	ID_DIV,	 // /
	//relation operators
	ID_EQ,		   // ==
	ID_NEQ,		   // !=
	ID_LESS,	   // <
	ID_GREATER,	   // >
	ID_LESS_EQ,	   // <=
	ID_GREATER_EQ, // >=
	//other
	ID_DEFINE,	   // :=
	ID_ASSIGN,	   // =
	ID_COMMA,	   // ,
	ID_SEMICOLLON, // ;
	ID_UNDER,	   // _
	//parser
	ID_FUNC_CALL,
	ID_TYPE_DEF,
	ID_EMPTY,
} tID;

//token attribute type
typedef union
{
	int64_t i;
	double d;
	char *s;
} tAttPtr;

//token type
typedef struct
{
	tID id;
	tAttPtr att;
} tToken;

typedef tToken *tTokenPtr;

/*returns RET_OK = 0 on success and token in tTokenPtr
token will always have an id and if necessarry an attribute
on error or eof returns non zero value and token data is undefined*/
tTokenRet get_token(tTokenPtr *, tEolFlag);

//printf conviniently token id and its attribute
//note that if ret value of get_token is not zero, the token data is undefined
void print_token(tTokenPtr);
int is_token(tTokenPtr);
char *is_token_lit(tTokenPtr);
char *is_token_add(tTokenPtr, char *name, char *name2);

#endif
