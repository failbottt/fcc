#include <stdint.h>

typedef uint64_t U64;

typedef struct {
	U64 len;
	const char *str;
} String;

typedef enum {
	TOKEN_VOID = 1,
	TOKEN_INTEGER,
	TOKEN_IDENTIFIER,
	TOKEN_LPAREN,
	TOKEN_RPAREN,
	TOKEN_LBRACE,
	TOKEN_RBRACE,
	TOKEN_RETURN,
} Type;

typedef struct {
	Type type;
	const char *value;
} Keyword;

typedef struct {
	Type type;
	String value;
} Token;
