#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>

#include "types.h"

// functions
char *read_file(char *path);
char *substr(const char *start, const char *end);

U64 tok_i = 0;
Token tokens[256];

int main()
{
	char *code = read_file("test/add.c");

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

					// TODO: this means that the token value has to be a union that can be an integer
					// or a char pointer or maybe something else
					Token token = {
						TOKEN_INTEGER,
						begin,
						stream,
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
		if (tokens[i].type == NULL) continue;

		/* printf("SIZE %ld", sizeof(tokens[i].value)); */

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

char *read_file(char *path)
{
#define	READ_FILE_PANIC \
    do { \
        fprintf(stderr, "Could not read file `%s`: %s\n", path, strerror(errno)); \
        exit(1); \
    } while (0)

	FILE *f = fopen(path, "r");
	if (f == NULL) READ_FILE_PANIC;
	if (fseek(f, 0, SEEK_END) < 0) READ_FILE_PANIC;

	long size = ftell(f);
	if (size < 0) READ_FILE_PANIC;
	
	char *buffer = malloc(size + 1);
	if (buffer == NULL) READ_FILE_PANIC;

	if (fseek(f, 0, SEEK_SET) < 0) READ_FILE_PANIC;

	fread(buffer, sizeof(char), size, f);
	if (ferror(f) < 0) READ_FILE_PANIC;

	buffer[size] = '\0';

	if (fclose(f) < 0) READ_FILE_PANIC;

	return buffer;
#undef READ_FILE_PANIC
}
