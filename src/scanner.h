#ifndef XELTU_LEXER_H
#define XELTU_LEXER_H

#include "common.h"

typedef enum {
	TOKEN_LEFT_PAREN,
	TOKEN_RIGHT_PAREN,
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_STAR,
	TOKEN_SLASH,
	TOKEN_COLON_EQUAL,
	TOKEN_IDENTIFIER,
	TOKEN_NUMBER,
	TOKEN_EOF,
	TOKEN_ERROR,
} TokenType;

typedef struct Token {
	TokenType type;
	const char* start;
	int length;
} Token;

typedef struct TokenArray {
	int count;
	int capacity;
	Token* tokens;
} TokenArray;

void initTokenArray(TokenArray* tokens);
void addTokenArray(TokenArray*, Token);
void freeTokenArray(TokenArray*);

typedef enum {
	SCANNING_OK,
	SCANNING_ERROR,
} ScanningResult;

ScanningResult scan(TokenArray* tokens, const char* source);

#endif
