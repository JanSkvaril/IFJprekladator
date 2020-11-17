#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 		//isalnum
#include <string.h>
#include <stdbool.h>
#include "scanner.h"

static int strIter;
static int strSize;

void print_token(tTokenPtr token)
{
	switch (token->id)
	{
		case ID_IDENTIFIER:
			printf("type: identifier, name: %s\n", token->att.s);
			break;
		case ID_INT_LIT:
			printf("type: int literal, value: %ld\n", token->att.i);
			break;
		case ID_FLOAT_LIT:
			printf("type: float literal, value: %lf\n", token->att.d);
			break;
		case ID_STRING_LIT:
			printf("type: string literal, value: %s\n", token->att.s);
			break;
		case ID_KEY_INT:
			printf("int (keyword)\n");
			break;
		case ID_KEY_FLOAT64:
			printf("float64 (keyword)\n");
			break;
		case ID_KEY_STRING:
			printf("string (keyword)\n");
			break;
		case ID_KEY_FUNC:
			printf("func (keyword)\n");
			break;
		case ID_KEY_RETURN:
			printf("return (keyword)\n");
			break;
		case ID_KEY_PACKAGE:
			printf("package (keyword)\n");
			break;
		case ID_KEY_IF:
			printf("if (keyword)\n");
			break;
		case ID_KEY_ELSE:
			printf("else (keyword)\n");
			break;
		case ID_KEY_FOR:
			printf("for (keyword)\n");
			break;
		case ID_ROUND_1:
			printf("\"(\"\n");
			break;
		case ID_ROUND_2:
			printf("\")\"\n");
			break;
		case ID_CURLY_1:
			printf("\"{\"\n");
			break;
		case ID_CURLY_2:
			printf("\"}\"\n");
			break;
		case ID_ADD:
			printf("\"+\"\n");
			break;
		case ID_SUB:
			printf("\"-\"\n");
			break;
		case ID_MULT:
			printf("\"*\"\n");
			break;
		case ID_DIV:
			printf("\"/\"\n");
			break;
		case ID_EQ:
			printf("\"==\"\n");
			break;
		case ID_NEQ:
			printf("\"!=\"\n");
			break;
		case ID_LESS:
			printf("\"<\"\n");
			break;
		case ID_GREATER:
			printf("\">\"\n");
			break;
		case ID_LESS_EQ:
			printf("\"<=\"\n");
			break;
		case ID_GREATER_EQ:
			printf("\">=\"\n");
			break;
		case ID_ASSIGN:
			printf("\":=\"\n");
			break;
		case ID_COMMA:
			printf("\",\"\n");
			break;
		case ID_SEMICOLLON:
			printf("\";\"\n");
			break;
		case ID_UNDER:
			printf("\"_\"\n");
			break;
		
		default:
			printf("lol je to v riti\n");
			break;
	}
}

int str_alloc(char ** str)
{
	*str = malloc(256 * sizeof(char));
	if (*str == NULL)
		return 1;
	strIter = 0;
	strSize = 256;
	return 0;
}

void str_add(char *str, char c)
{
	str[strIter] = c;
	strIter++;
	if (strIter >= strSize)
		str = realloc(str, strSize = strSize*2);

}

typedef enum
{
	START,
	IDEN
} tState;


//nebo nejakou tabulkou???
bool is_keyword (char *str, tTokenPtr token)
{
	if (!strcmp(str, "int"))
	{
		token->id = ID_KEY_INT;
		return true;
	}
	else if (!strcmp(str, "float64"))
	{
		token->id = ID_KEY_FLOAT64;
		return true;
	}
	else if (!strcmp(str, "string"))
	{
		token->id = ID_KEY_STRING;
		return true;
	}
	else if (!strcmp(str, "func"))
	{
		token->id = ID_KEY_FUNC;
		return true;
	}
	else if (!strcmp(str, "return"))
	{
		token->id = ID_KEY_RETURN;
		return true;
	}
	else if (!strcmp(str, "package"))
	{
		token->id = ID_KEY_PACKAGE;
		return true;
	}
	else if (!strcmp(str, "if"))
	{
		token->id = ID_KEY_IF;
		return true;
	}
	else if (!strcmp(str, "else"))
	{
		token->id = ID_KEY_ELSE;
		return true;
	}
	else if (!strcmp(str, "for"))
	{
		token->id = ID_KEY_FOR;
		return true;
	}
	else 
		return false; 
	
}



tTokenRet get_token(tTokenPtr *token)
{
	char *str = NULL;
	int c;
	tState state = START;
	while (1)
	{
		c = getchar();
		switch (state)
		{
			case START:
				if (isalpha(c) || c == '_')
				{
					state = IDEN;
					if (str_alloc(&str))
						return RET_INTERNAL_ERR;
					str_add(str, c);
				}
				else if (isspace(c));
				else if (c == EOF)
					return RET_EOF;
				else
					return RET_LEX_ERR;
				break;
			case IDEN:
				if (isalnum(c) || c == '_')
				{
					str_add(str, c);
					break;
				}
				//correct identifier
				else
				{
					str_add(str, 0);
					str = realloc(str, strIter);
					*token = malloc(sizeof (tToken));//do funkce
					ungetc(c, stdin);
					if (is_keyword(str, *token))
						return RET_OK;
					else
					{
						(*token)->id = ID_IDENTIFIER;
						(*token)->att.s = str; //realoc na velikost;
						return RET_OK;
					}

				}
		 
			//is an invalid lexeme
			default:
				return RET_LEX_ERR;

		}
	}
	
	token = NULL;
	return RET_OK;
}