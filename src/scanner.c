#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"

#include "scanner.h"
#include "memory.h"

typedef struct Scanner {
	const char* start;
	const char* current;
} Scanner;
Scanner scanner;

static void initScanner(const char* source) {
	scanner.start = source;
	scanner.current = source;
}


static Token makeToken(TokenType type) {
	Token token;
	token.type = type;
	token.start = scanner.start;
	token.length = (int)(scanner.current - scanner.start);
	return token;
}

static Token errorToken(const char* msg) {
	Token token;
	token.type = TOKEN_ERROR;
	token.length = (int)strlen(msg);
	return token;
}

void initTokenArray(TokenArray* tokens) {
	tokens->count = 0;
	tokens->capacity = 0;
	tokens->tokens = NULL;
}

void addTokenArray(TokenArray* tokens, Token token) {
	if (tokens->capacity < tokens->count + 1) {
		int oldCapacity = tokens->capacity;
		tokens->capacity = GROW_CAPACITY(tokens->capacity);
		tokens->tokens = GROW_ARRAY(
				Token, tokens->tokens, oldCapacity, tokens->capacity);
	}

	tokens->tokens[tokens->count] = token;
	tokens->count += 1;
}

void freeTokenArray(TokenArray* tokens) {
	FREE_ARRAY(Token, tokens->tokens, tokens->capacity);
	initTokenArray(tokens);
}


static char advance() {
	scanner.current += 1;
	return scanner.current[-1];
}

static char peek() {
	return *scanner.current;
}

static bool isAtEnd() {
	return *scanner.current == '\0';
}

static bool isDigit(const char c) {
	return '0' <= c && c <= '9';
}

static Token number() {
	while (isDigit(peek())) {
		advance();
	}

	if (*scanner.current == '.' && !isAtEnd()) {
		advance();
		while (isDigit(peek()) && !isAtEnd()) {
			advance();
		}
	}
	return makeToken(TOKEN_NUMBER);
}

static Token scanToken() {
	scanner.start = scanner.current;

	if (isAtEnd()) return makeToken(TOKEN_EOF);

	if (isDigit(peek())) return number();

	char c = advance();

	switch (c) {
		case '+': { return makeToken(TOKEN_PLUS); }
		case '-': { return makeToken(TOKEN_MINUS); }
		case '*': { return makeToken(TOKEN_STAR); }
		case '/': { return makeToken(TOKEN_SLASH); }
		case '(': { return makeToken(TOKEN_LEFT_PAREN); break;}
		case ')': { return makeToken(TOKEN_RIGHT_PAREN); }
		default: {
			return errorToken("Unexpected character.");
		}
	}

}

ScanningResult scan(TokenArray* tokens, const char* source) {
	initScanner(source);
	initTokenArray(tokens);

	ScanningResult result = SCANNING_OK;

	Token token;
	for (; token.type != TOKEN_EOF;) {
		token = scanToken();
		if (token.type == TOKEN_ERROR) result = SCANNING_ERROR;
		addTokenArray(tokens, token);
	}

#ifdef SCANNER_DEBUG
	disassembleTokenArray(*tokens);
#endif

	return result;
}
