// to compile and run: 
// gcc -std=c99 -Wextra -Wall -g -o tokenizer main.c && ./tokenizer
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <stdint.h>

// functions
char *read_file(char *path);
char *substr(const char *start, const char *end);

// types
//
// the numbers are ascii codes. They correlate
// directly to all the string characters in test.add.c
typedef enum {
	TOKEN_VOID = 1,
	TOKEN_LPAREN = 40,
	TOKEN_RPAREN = 41,
	TOKEN_LBRACE = 123,
	TOKEN_RBRACE = 125,
	TOKEN_SEMICOLON = 59,
	TOKEN_PERCENT = 37,
	TOKEN_PLUS = 43,
	TOKEN_EQUALS = 61,
	TOKEN_DOUBLE_QUOTE = 34,
	TOKEN_BACKSLASH = 92,
	TOKEN_COMMA = 44,
	TOKEN_POUND = 35,
	TOKEN_PERIOD = 46,
	TOKEN_GT= 62,
	TOKEN_LT= 60,

	TOKEN_INTEGER,
	TOKEN_IDENTIFIER,
	TOKEN_RETURN,
} Type;

typedef struct {
	Type type;
	const char *value;
} Keyword;

typedef struct {
	Type type;
	char *start;
	char *end;
	char *value;
} Token;

uint64_t tok_i = 0;
Token tokens[256];

int main()
{
	char *code = "#include <stdio.h>\
		\
		int add(int x, int y) {\
			return x + y;\
		}\
		\
		int main() {\
			int sum = add(2, 3);\
		\
			return sum;\
		}";

	Keyword k_void = {TOKEN_VOID, "void"};
	Keyword k_integer = {TOKEN_INTEGER, "int"};
	Keyword k_return = {TOKEN_RETURN, "return"};
	Keyword keywords[3] = {k_void, k_integer, k_return};

	char *begin = code;
	char *stream = code;

	for(;;)
	{
		if (*stream == '\0') break;

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


					Token token = {
						TOKEN_IDENTIFIER,
						begin, 
						stream,
						substr(begin, stream)
					};

					for (int i = 0; i < 3; i++)
					{
						if (strcmp(token.value, keywords[i].value) == 0)
						{
							token.type = keywords[i].type;
							break;
						}
					}

					begin = stream;
					tokens[tok_i++] = token;
					break;
				}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': 
				{
					while (isdigit(*stream))
					{
						stream++;
					}

					Token token = {
						TOKEN_INTEGER,
						begin,
						stream,
						// TODO you'd want to turn the string type of the number to it's integer type (e.g. via atoi() ref: https://git.musl-libc.org/cgit/musl/tree/src/stdlib/atoi.c) 
						substr(begin, stream)
					};

					tokens[tok_i++] = token;
					begin = stream;

					break;
				}
			case '(':
			case ')':
			case '{':
			case '}':
			case ';':
			case '"':
			case '\\':
			case '%':
			case '+':
			case '=':
			case ',':
			case '#':
			case '.':
			case '<':
			case '>':
				{
					char c = *stream;
					char *cp = malloc(sizeof(char) * 2);
					cp[0] = c;
					cp[1] = '\0';
					Token token = {(int)*stream, begin, stream, cp};
					tokens[tok_i++] = token;
					stream++;
					begin = stream;
					break;
				}
			case ' ':
			case '\t':
				stream++;
				begin = stream;
				break;
			

			default:
				stream++;
				begin = stream;
				break;

		}
	}

	printf("TOKENS {\n");
	for (int i = 0; i < 256; i++)
	{
		if (tokens[i].type == 0) continue;

		printf("	{type: %d, value: '%s'}\n", tokens[i].type, tokens[i].value);

	}
	printf("}\n");

	return 0;
}

char *substr(const char *start, const char *end)
{
	int len = end - start;
	char *str = malloc(len + 1);
	strncpy(str, start, len);

	str[len] = '\0';

	return str;
}
