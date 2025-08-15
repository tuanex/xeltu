#include <stdio.h>
#include <string.h>

#include "common.h"
#include "debug.h"
#include "memory.h"
#include "lexer.h"

void initTokenArray(TokenArray* tokens) {
	tokens->count = 0;
	tokens->capacity = 0;
	tokens->first_token = NULL;
}

static void addTokenArray(TokenArray* tokens, Token token) {
	if (tokens->capacity < tokens->count + 1) {
		int oldCapacity = tokens->capacity;
		tokens->capacity = GROW_CAPACITY(oldCapacity);
		tokens->first_token = GROW_ARRAY(Token, tokens->first_token,
				oldCapacity, tokens->capacity);
	}

	tokens->first_token[tokens->count] = token;
	tokens->count++;
}

void freeTokenArray(TokenArray* tokens) {
	FREE_ARRAY(Token, tokens->first_token, tokens->capacity);
	initTokenArray(tokens);
}

typedef struct {
	const char* start;
	const char* current;
} Scanner;

Scanner scanner;

void initScanner(const char* source) {
	scanner.start = source;
	scanner.current = source;
}

static bool isAlpha(char c) {
	return (c >= 'a' && c <= 'z') ||
		(c >= 'A' && c <= 'Z') ||
		c == '_';
}

static bool isDigit(char c) {
	return c >= '0' && c <= '9';
}

static bool isAtEnd() {
	return *scanner.current == '\0';
}

static char advance() {
	scanner.current += 1;
	return scanner.current[-1];
}

static char peek() {
	return *scanner.current;
}

static char peekNext() {
  if (isAtEnd()) return '\0';
  return scanner.current[1];
}

static bool match(char expected) {
	if (isAtEnd()) return false;
	if (*scanner.current != expected) return false;
	scanner.current += 1;
	return true;
}

static Token makeToken(TokenType type) {
	Token token;
	token.type = type;
	token.start = scanner.start;
	token.length = (int)(scanner.current - 1 - scanner.start);
	return token;
}

static Token errorToken(const char* msg) {
	Token token;
	token.type = TOKEN_ERROR;
	token.start = msg;
	token.length = (int)strlen(msg);
	return token;
}

static void skipWhitespace() {
	for (;;) {
		char c = peek();
		switch (c) {
			case ' ':
			case '\r':
			case '\t':
			advance();
				break;
			case '\n':
				advance();
				break;
			default:
				return;
		}
	}
}

static TokenType checkKeyword(int start, int length,
    const char* rest, TokenType type) {

	if (scanner.current - scanner.start == start + length &&
		memcmp(scanner.start + start, rest, length) == 0) {
		return type;
	}

	return TOKEN_IDENTIFIER;
}

static TokenType identifierType() {
	switch (scanner.start[0]) {
	}
	return TOKEN_IDENTIFIER;
}

static Token identifier() {
	while (isAlpha(peek()) || isDigit(peek())) advance();
	return makeToken(TOKEN_IDENTIFIER);
}

static Token number() {
	while (isDigit(peek())) advance();

	if (peek() == '.' && isDigit(peekNext())) {
		advance();

		while (isDigit(peek())) advance();
	}

	return makeToken(TOKEN_NUMBER);
}

Token scanToken() {
	skipWhitespace();

	scanner.start = scanner.current;

	if (isAtEnd()) return makeToken(TOKEN_EOF);

	char c = advance();

	if (isDigit(c)) {
		return number();
	}
	if (isAlpha(c)) return identifier();

	switch (c) {
		case '(': return makeToken(TOKEN_LEFT_PAREN);
		case ')': return makeToken(TOKEN_RIGHT_PAREN);
		case '{': return makeToken(TOKEN_LEFT_BRACE);
		case '}': return makeToken(TOKEN_RIGHT_BRACE);
		case ';': return makeToken(TOKEN_SEMICOLON);
		case ',': return makeToken(TOKEN_COMMA);
		case '.': return makeToken(TOKEN_DOT);
		case '-': return makeToken(TOKEN_MINUS);
		case '+': return makeToken(TOKEN_PLUS);
		case '/': return makeToken(TOKEN_SLASH);
		case '*': return makeToken(TOKEN_STAR);
		case '!':
		  return makeToken(
			  match('=') ? TOKEN_BANG_EQUAL : TOKEN_BANG);
		case '=':
		  return makeToken(
			  match('=') ? TOKEN_EQUAL_EQUAL : TOKEN_EQUAL);
		case '<':
		  return makeToken(
			  match('=') ? TOKEN_LESS_EQUAL : TOKEN_LESS);
		case '>':
		  return makeToken(
			  match('=') ? TOKEN_GREATER_EQUAL : TOKEN_GREATER);
		default:
		  return makeToken(TOKEN_ERROR);
	}
}

void tokenize(const char* source, TokenArray* tokens) {
	initTokenArray(tokens);
	initScanner(source);
	Token token;

	do {
		token = scanToken();
		addTokenArray(tokens, token);
	} while (token.type != TOKEN_EOF);

#ifdef DEBUG_LEXER_EXECUTION 

	disassembleTokenArray(tokens);

#endif
}
