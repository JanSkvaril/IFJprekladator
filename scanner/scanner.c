#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 		//isalnum
#include <string.h>
#include <stdbool.h>
#include "scanner.h"

static int strIter;
static int strSize;

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