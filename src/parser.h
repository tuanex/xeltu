#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "value.h"

typedef enum {
	BINOP_ADD, 
	BINOP_SUB, 
	BINOP_MUL, 
	BINOP_DIV
} OpCode;

typedef enum { CONSTANT, UNARY, BINARY, ERROR } NodeTag;

typedef struct Node {
	NodeTag tag;

	union {
		Value leaf_val;

		struct {
			enum {UNOP_POS, UNOP_MIN} op;
			struct Node *operand;
		} unop_val;

		struct {
			OpCode op;
            struct Node *left;
            struct Node *right;
        } binop_val;
	} value;
} Node;

Node* parse(TokenArray*);
Node* makeNode(NodeTag, Token);
void freeNode(Node*);

#endif
