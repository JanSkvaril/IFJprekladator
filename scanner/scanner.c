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
			printf("type: float literal, value: %.16lf\n", token->att.d);
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
	START_S,
	IDEN_S,
	INT_FLOAT_LIT_S,
	FLOAT_DEC1_S,
	FLOAT_DEC2_S,
	FLOAT_EXP_S,
	FLOAT_EXP2_S,
	FLOAT_EXP_SIGN_S,
	STRING_S
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
	tState state = START_S;
	while (1)
	{
		c = getchar();
		switch (state)
		{
			//start***************************************************
			case START_S:
				//is identifier
				if (isalpha(c) || c == '_')
				{
					state = IDEN_S;
					if (str_alloc(&str))
						return RET_INTERNAL_ERR;
					str_add(str, c);
				}
				//is integer or float literal
				else if (isdigit(c))
				{
					state = INT_FLOAT_LIT_S;
					if (str_alloc(&str))
						return RET_INTERNAL_ERR;
					str_add(str, c);
				}
				//else if; //float64
				//is string
				else if (c == '"')
				{
					state = STRING_S;
					if (str_alloc(&str))
						return RET_INTERNAL_ERR;
					str_add(str, c);
				}
				//ignore whitespace characters
				else if (isspace(c));
				else if (c == EOF)
					return RET_EOF;
				//no match means invalid lexeme
				else
					return RET_LEX_ERR;

				*token = malloc(sizeof (tToken));//do funkce
				if (*token == NULL)
					return RET_INTERNAL_ERR;

				break;

			//identifier**********************************************************
			case IDEN_S:
				//continue to read characters of identifier
				if (isalnum(c) || c == '_')
					str_add(str, c);
				//end of identifier, return token
				else
				{
					str_add(str, 0);
					str = realloc(str, strIter);
					ungetc(c, stdin);
					if (is_keyword(str, *token))
						return RET_OK;
					else
					{
						(*token)->id = ID_IDENTIFIER;
						(*token)->att.s = str;
						return RET_OK;
					}
				}
				break;

			//integer of float literal********************************************
		 	case INT_FLOAT_LIT_S:
			 	//continue reading
				if (isdigit(c))
					str_add(str, c);
				//is a float literal, read decimal part
				else if (c == '.')
				{
					state = FLOAT_DEC1_S;
					str_add(str,c);
				}
				//is a float literal, read value of exponent
				else if (c == 'e' || c == 'E')
				{
					state = FLOAT_EXP_S;
					str_add(str,c);
				}
				//end of integer literal, return token
				else
				{
					ungetc(c, stdin);
					str_add(str, 0);
					(*token)->id = ID_INT_LIT;
					sscanf(str, "%lld", (long long *)&(*token)->att.i);
					free(str);
					return RET_OK;
				}
				break;

			//float literal******************************************************************
			case FLOAT_DEC1_S:
				//after decimal point there has to be at least one digit, otherwise invalid lexeme
				if (isdigit(c))
				{
					state = FLOAT_DEC2_S; 
					str_add(str, c);
				}
				else
					return RET_LEX_ERR;
				break;
			//continue reading decimal digits
			case FLOAT_DEC2_S:
				if (isdigit(c))
					str_add(str, c);
				//start of exponent
				else if (c == 'e' || c == 'E')
				{
					state = FLOAT_EXP_S;
					str_add(str,c);
				}
				//read string is a valid decimal number with whole and decimal part
				else
				{
					ungetc(c, stdin);
					str_add(str, 0);
					(*token)->id = ID_FLOAT_LIT;
					sscanf(str, "%lf",&(*token)->att.d);
					free(str);
					return RET_OK;
				} 
				break;
			//read first exponent digit, non mandatory + or - can be read
			case FLOAT_EXP_S:
				if (c == '+' || c == '-')
				{
					state = FLOAT_EXP_SIGN_S;
					str_add(str, c);
				}
				else if (isdigit(c))
				{
					state = FLOAT_EXP2_S;
					str_add(str, c);
				}
				else
				{
					free(str);
					return RET_LEX_ERR;
				}
				break;
			//sign was read, read the first exponent digit
			case FLOAT_EXP_SIGN_S:
				if (isdigit(c))
				{
					state = FLOAT_EXP2_S;
					str_add(str, c);
				}
				else
				{
					free(str);
					return RET_LEX_ERR;
				}
				break;
			//continue reading exponent digits, non digit character on input results in returning
			//a valid floating literal token
			case FLOAT_EXP2_S:
				if (isdigit(c))
					str_add(str, c);
				else 
				{
					ungetc(c, stdin);
					str_add(str, 0);
					(*token)->id = ID_FLOAT_LIT;
					sscanf(str, "%lf",&(*token)->att.d);
					free(str);
					return RET_OK;
				}
				break;
			
			//is an invalid lexeme
			default:
				return RET_LEX_ERR;

		}
	}
	
	token = NULL;
	return RET_OK;
}