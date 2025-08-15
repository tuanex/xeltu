#ifndef LEXER_H
#define LEXER_H

typedef enum {
	// Single-character tokens.
	TOKEN_LEFT_PAREN,
	TOKEN_RIGHT_PAREN,
	TOKEN_LEFT_BRACE,
	TOKEN_RIGHT_BRACE,
	TOKEN_COMMA,
	TOKEN_DOT,
	TOKEN_MINUS,
	TOKEN_PLUS,
	TOKEN_SEMICOLON,
	TOKEN_SLASH,
	TOKEN_STAR,
	// One or two character tokens.
	TOKEN_BANG,
	TOKEN_BANG_EQUAL,
	TOKEN_EQUAL,
	TOKEN_EQUAL_EQUAL,
	TOKEN_GREATER,
	TOKEN_GREATER_EQUAL,
	TOKEN_LESS,
	TOKEN_LESS_EQUAL,
	// Literals.
	TOKEN_IDENTIFIER,
	TOKEN_NUMBER,
	// Keywords.
	TOKEN_RETURN,

	TOKEN_ERROR,
	TOKEN_EOF
} TokenType;

typedef struct {
	TokenType type;
	const char *start;
	int length;
} Token;

typedef struct {
	int count; 
	int capacity;
	Token* first_token;
} TokenArray;

void initTokenArray(TokenArray*);
void freeTokenArray(TokenArray*);

void initScanner(const char *);
Token scanToken();

void tokenize(const char*, TokenArray*);

#endif
