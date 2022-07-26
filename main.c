#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "types.h"

// functions
void substr(const char *start, const char *end, String *string);

U64 tok_i = 0;
Token tokens[256];

char *code = "void add(int x, int y) {\n\	return x + y;\n}";


int main(int argc, char **argv) 
{
	Keyword k_void = {TOKEN_VOID, "void"};
	Keyword k_integer = {TOKEN_INTEGER, "int"};
	Keyword k_return = {TOKEN_RETURN, "return"};
	Keyword keywords[3] = {k_void, k_integer, k_return};

	const char *begin = code;
	const char *stream = code;

	for(;;)
	{
		switch (*stream) {
			case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': case 'j':
			case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': case 's': case 't':
			case 'u': case 'v': case 'w': case 'x': case 'y': case 'z':
			case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': case 'J':
			case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T':
			case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z':
			case '_': 
				{
					while (isalnum(*stream) || *stream == '_') 
					{
						stream++;
					}


					Token token = {};
					String string;

					substr(begin, stream, &string);

					token.value = string;

					for (int i = 0; i < 3; i++)
					{
						if (strcmp(string.str, keywords[i].value) == 0)
						{
							token.type = keywords[i].type;
							break;
						}
					}

					if (token.type == NULL) {
						token.type = TOKEN_IDENTIFIER;
					}

					begin = stream;
					tokens[tok_i++] = token;
					break;
				}
			case '(':
				{
					Token token;
					String string = {1, "("};
					token.type = TOKEN_LPAREN;
					token.value = string;
					tokens[tok_i++] = token;
					stream++;
					begin = stream;
					break;
				}
			case ')':
				{
					Token token;
					String string = {1, ")"};
					token.type = TOKEN_RPAREN;
					token.value = string;
					tokens[tok_i++] = token;
					stream++;
					begin = stream;
					break;
				}
			case '{':
				{
					Token token;
					String string = {1, "{"};
					token.type = TOKEN_LBRACE;
					token.value = string;
					tokens[tok_i++] = token;
					stream++;
					begin = stream;
					break;
				}
			case '}':
				{
					Token token;
					String string = {1, "}"};
					token.type = TOKEN_RBRACE;
					token.value = string;
					tokens[tok_i++] = token;
					stream++;
					begin = stream;
					break;
				}
			case ' ':
			case '\t':
			case '\n':
				stream++;
				begin = stream;
				break;
			

			default:
				stream++;
				break;

		}

		// TODO: remove this
		if (*stream == '}') break;
		
	}

	printf("TOKENS = {\n");
	for (int i = 0; i < 256; i++)
	{
		if (tokens[i].value.str == NULL) continue;

		printf("	{type: %d, value: %s}\n", tokens[i].type, tokens[i].value.str);

	}
	printf("}\n");

	return 0;
}

void substr(const char *start, const char *end, String *string)
{
	int len = end - start;
	char *str = malloc(len);
	strncpy(str, start, len);

	string->len = len;
	string->str = str;
}

