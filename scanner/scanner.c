#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //isalnum
#include <string.h>
#include <stdbool.h>
#include "scanner.h"
#include "../error/error.h"
#include "../debug.h"

static int strIter;
static int strSize;

void print_token(tTokenPtr token)
{
	switch (token->id)
	{
	case ID_IDENTIFIER:
		DEBUG_PRINT(("type: identifier, name: %s\n", token->att.s));
		break;
	case ID_INT_LIT:
		DEBUG_PRINT(("type: int literal, value: %ld\n", token->att.i));
		break;
	case ID_FLOAT_LIT:
		DEBUG_PRINT(("type: float literal, value: %.16lf\n", token->att.d));
		break;
	case ID_STRING_LIT:
		DEBUG_PRINT(("type: string literal, value: >%s<\n", token->att.s));
		break;
	case ID_KEY_INT:
		DEBUG_PRINT(("int (keyword)\n"));
		break;
	case ID_KEY_FLOAT64:
		DEBUG_PRINT(("float64 (keyword)\n"));
		break;
	case ID_KEY_STRING:
		DEBUG_PRINT(("string (keyword)\n"));
		break;
	case ID_KEY_FUNC:
		DEBUG_PRINT(("func (keyword)\n"));
		break;
	case ID_KEY_RETURN:
		DEBUG_PRINT(("return (keyword)\n"));
		break;
	case ID_KEY_PACKAGE:
		DEBUG_PRINT(("package (keyword)\n"));
		break;
	case ID_KEY_IF:
		DEBUG_PRINT(("if (keyword)\n"));
		break;
	case ID_KEY_ELSE:
		DEBUG_PRINT(("else (keyword)\n"));
		break;
	case ID_KEY_FOR:
		DEBUG_PRINT(("for (keyword)\n"));
		break;
	case ID_ROUND_1:
		DEBUG_PRINT(("\"(\"\n"));
		break;
	case ID_ROUND_2:
		DEBUG_PRINT(("\")\"\n"));
		break;
	case ID_CURLY_1:
		DEBUG_PRINT(("\"{\"\n"));
		break;
	case ID_CURLY_2:
		DEBUG_PRINT(("\"}\"\n"));
		break;
	case ID_ADD:
		DEBUG_PRINT(("\"+\"\n"));
		break;
	case ID_SUB:
		DEBUG_PRINT(("\"-\"\n"));
		break;
	case ID_MULT:
		DEBUG_PRINT(("\"*\"\n"));
		break;
	case ID_DIV:
		DEBUG_PRINT(("\"/\"\n"));
		break;
	case ID_EQ:
		DEBUG_PRINT(("\"==\"\n"));
		break;
	case ID_NEQ:
		DEBUG_PRINT(("\"!=\"\n"));
		break;
	case ID_LESS:
		DEBUG_PRINT(("\"<\"\n"));
		break;
	case ID_GREATER:
		DEBUG_PRINT(("\">\"\n"));
		break;
	case ID_LESS_EQ:
		DEBUG_PRINT(("\"<=\"\n"));
		break;
	case ID_GREATER_EQ:
		DEBUG_PRINT(("\">=\"\n"));
		break;
	case ID_DEFINE:
		DEBUG_PRINT(("\":=\"\n"));
		break;
	case ID_ASSIGN:
		DEBUG_PRINT(("\"=\"\n"));
		break;
	case ID_COMMA:
		DEBUG_PRINT(("\",\"\n"));
		break;
	case ID_SEMICOLLON:
		DEBUG_PRINT(("\";\"\n"));
		break;
	case ID_UNDER:
		DEBUG_PRINT(("\"_\"\n"));
		break;
	case ID_FUNC_CALL:
		DEBUG_PRINT(("FUNC_CALL\n"));
		break;
	case ID_TYPE_DEF:
		DEBUG_PRINT(("TYPE DEF\n"));
		break;
	case ID_EMPTY:
		DEBUG_PRINT(("EMPTY TOKEN\n"));
		break;
	default:
		DEBUG_PRINT(("lol je to v riti\n"));
		break;
	}
}

int str_alloc(char **str)
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
		str = realloc(str, strSize = strSize * 2);
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
	STRING_S,
	STRING_ESCAPE_S,
	STRING_HEX1,
	STRING_HEX2,
	SLASH_SIGN_S,
	LINE_COMMENT_S,
	BLOCK_COMMENT_S,
	BLOCK_COMMENT_END_S,
	EQ_SIGN_S,
	NEQ_SIGN_S,
	LESS_SIGN_S,
	GREATER_SIGN_S,
	COLON_SIGN_S,
	UNDER_SIGN_S

} tState;

//nebo nejakou tabulkou???
bool is_keyword(char *str, tTokenPtr token)
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

tTokenRet get_token(tTokenPtr *token, tEolFlag eol)
{
	char *str = NULL;
	int c;
	tState state = START_S;
	char hexStr[2];
	unsigned hexNum;

	*token = malloc(sizeof(tToken)); //do funkce
	if (*token == NULL)
		scanner_free_exit(*token, INTER_ERR);

	while (1)
	{
		c = getchar();
		switch (state)
		{
		//start***************************************************
		case START_S:
			//if eol is required, only white space characters are allowed until the firt new line
			if (eol == EOL_REQ)
			{
				if (c == '\n')
					eol = EOL_OPT;
				else if (isspace(c) || c == '/')
					;
				else
					scanner_free_exit(*token, SYN_ERR);
			}
			//white space chars dont change state and are ignored
			//new line character while eol is forbidden causes error
			if (isspace(c))
			{
				if (eol == EOL_FORBID && c == '\n')
					scanner_free_exit(*token, SYN_ERR);
			}

			//is identifier
			else if (isalpha(c))
			{
				state = IDEN_S;
				if (str_alloc(&str))
					scanner_free_exit(*token, INTER_ERR);
				str_add(str, c);
			}
			//is identifier or underscore sign
			else if (c == '_')
			{
				state = UNDER_SIGN_S;
				if (str_alloc(&str))
					scanner_free_exit(*token, INTER_ERR);
				str_add(str, c);
			}
			//is integer or float literal
			else if (isdigit(c))
			{
				state = INT_FLOAT_LIT_S;
				if (str_alloc(&str))
					scanner_free_exit(*token, INTER_ERR);
				str_add(str, c);
			}
			//is string
			else if (c == '"')
			{
				state = STRING_S;
				if (str_alloc(&str))
					scanner_free_exit(*token, INTER_ERR);
			}
			//a division sign or start of a comment
			else if (c == '/')
			{
				state = SLASH_SIGN_S;
			}
			//brackets ****************************************
			else if (c == '(')
			{
				(*token)->id = ID_ROUND_1;
				return RET_OK;
			}
			else if (c == ')')
			{
				(*token)->id = ID_ROUND_2;
				return RET_OK;
			}
			else if (c == '{')
			{
				(*token)->id = ID_CURLY_1;
				return RET_OK;
			}
			else if (c == '}')
			{
				(*token)->id = ID_CURLY_2;
				return RET_OK;
			}
			//arithmetic operators ******************
			else if (c == '+')
			{
				(*token)->id = ID_ADD;
				return RET_OK;
			}
			else if (c == '-')
			{
				(*token)->id = ID_SUB;
				return RET_OK;
			}
			else if (c == '*')
			{
				(*token)->id = ID_MULT;
				return RET_OK;
			}

			//relation operators
			else if (c == '=')
			{
				state = EQ_SIGN_S;
			}
			else if (c == '!')
			{
				state = NEQ_SIGN_S;
			}
			else if (c == '<')
			{
				state = LESS_SIGN_S;
			}
			else if (c == '>')
			{
				state = GREATER_SIGN_S;
			}
			//others
			else if (c == ':')
			{
				state = COLON_SIGN_S;
			}
			else if (c == ',')
			{
				(*token)->id = ID_COMMA;
				return RET_OK;
			}
			else if (c == ';')
			{
				(*token)->id = ID_SEMICOLLON;
				return RET_OK;
			}
			else if (c == '_')
			{
				(*token)->id = ID_UNDER;
				return RET_OK;
			}

			else if (c == EOF)
			{
				free(*token);
				return RET_EOF;
			}
			//no match means invalid lexeme
			else
			{
				scanner_free_exit(*token, LEX_ERR);
			}
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
				str_add(str, c);
			}
			//is a float literal, read value of exponent
			else if (c == 'e' || c == 'E')
			{
				state = FLOAT_EXP_S;
				str_add(str, c);
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
			{
				free(str);
				scanner_free_exit(*token, LEX_ERR);
			}
			break;
		//continue reading decimal digits
		case FLOAT_DEC2_S:
			if (isdigit(c))
				str_add(str, c);
			//start of exponent
			else if (c == 'e' || c == 'E')
			{
				state = FLOAT_EXP_S;
				str_add(str, c);
			}
			//read string is a valid decimal number with whole and decimal part
			else
			{
				ungetc(c, stdin);
				str_add(str, 0);
				(*token)->id = ID_FLOAT_LIT;
				sscanf(str, "%lf", &(*token)->att.d);
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
				scanner_free_exit(*token, LEX_ERR);
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
				scanner_free_exit(*token, LEX_ERR);
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
				sscanf(str, "%lf", &(*token)->att.d);
				free(str);
				return RET_OK;
			}
			break;
		//string***************************************************************
		case STRING_S:
			//string literal has to be on one line
			if (c == '\n')
			{
				free(str);
				scanner_free_exit(*token, LEX_ERR);
			}
			//end of string, return token
			else if (c == '"')
			{
				str_add(str, 0);
				str = realloc(str, strIter);
				(*token)->id = ID_STRING_LIT;
				(*token)->att.s = str;
				return RET_OK;
			}
			//escape sequence
			else if (c == '\\')
				state = STRING_ESCAPE_S;
			else
				str_add(str, c);
			break;

		case STRING_ESCAPE_S:
			state = STRING_S;
			//only these characters are accepted after escape sequence
			if (c == '\"')
				str_add(str, '\"');
			else if (c == '\\')
				str_add(str, '\\');
			else if (c == 'n')
				str_add(str, '\n');
			else if (c == 't')
				str_add(str, '\t');
			else if (c == 'x')
				state = STRING_HEX1;

			else
			{
				free(str);
				scanner_free_exit(*token, LEX_ERR);
			}
			break;
		case STRING_HEX1:
			//read first hex digit
			if (isxdigit(c))
			{
				hexStr[0] = c;
				state = STRING_HEX2;
			}
			else
			{
				free(str);
				scanner_free_exit(*token, LEX_ERR);
			}

			break;
		case STRING_HEX2:
			//read second hex digit
			if (isxdigit(c))
			{
				hexStr[1] = c;
				hexStr[2] = 0;
				sscanf(hexStr, "%x", &hexNum);
				str_add(str, hexNum);
				state = STRING_S;
			}
			else
			{
				free(str);
				scanner_free_exit(*token, LEX_ERR);
			}

			break;
		case SLASH_SIGN_S:
			if (c == '/')
				state = LINE_COMMENT_S;
			else if (c == '*')
				state = BLOCK_COMMENT_S;
			else
			{
				ungetc(c, stdin);
				(*token)->id = ID_DIV;
				return RET_OK;
			}
			break;
		case LINE_COMMENT_S:
			if (c == '\n')
			{
				ungetc(c, stdin);
				state = START_S;
			}
			else if (c == EOF)
				return RET_EOF;
			break;
		case BLOCK_COMMENT_S:
			if (c == '*')
				state = BLOCK_COMMENT_END_S;
			else if (c == EOF)
				scanner_free_exit(*token, LEX_ERR);
			break;
		case BLOCK_COMMENT_END_S:
			if (c == '/')
				state = START_S;
			else
				state = BLOCK_COMMENT_S;
			break;
		//others
		case EQ_SIGN_S:
			if (c == '=')
			{
				(*token)->id = ID_EQ;
				return RET_OK;
			}
			else
			{
				ungetc(c, stdin);
				(*token)->id = ID_ASSIGN;
				return RET_OK;
			}
			break;
		case NEQ_SIGN_S:
			if (c == '=')
			{
				(*token)->id = ID_NEQ;
				return RET_OK;
			}
			else
			{
				scanner_free_exit(*token, LEX_ERR);
			}
			break;
		case LESS_SIGN_S:
			if (c == '=')
			{
				(*token)->id = ID_LESS_EQ;
				return RET_OK;
			}
			else
			{
				ungetc(c, stdin);
				(*token)->id = ID_LESS;
				return RET_OK;
			}
			break;
		case GREATER_SIGN_S:
			if (c == '=')
			{
				(*token)->id = ID_GREATER_EQ;
				return RET_OK;
			}
			else
			{
				ungetc(c, stdin);
				(*token)->id = ID_GREATER;
				return RET_OK;
			}
			break;
		case COLON_SIGN_S:
			if (c == '=')
			{
				(*token)->id = ID_DEFINE;
				return RET_OK;
			}
			else
			{
				scanner_free_exit(*token, LEX_ERR);
			}
			break;
		//underscore on ipnut, either identifier or alone underscore
		case UNDER_SIGN_S:
			if (isalnum(c) || c == '_')
			{
				state = IDEN_S;
				str_add(str, c);
			}
			else
			{
				free(str);
				ungetc(c, stdin);
				(*token)->id = ID_UNDER;
				return RET_OK;
			}
			break;

		//is an invalid lexeme
		default:
			scanner_free_exit(*token, LEX_ERR);
		}
	}

	token = NULL;
	return RET_OK;
}
