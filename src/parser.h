#ifndef XELTU_PARSER_H
#define XELTU_PARSER_H

#include "scanner.h"

typedef enum { UNOP_POS, UNOP_NEG } UnCode;
typedef enum { BINOP_ADD, BINOP_SUB, BINOP_MUL, BINOP_DIV, } OpCode;

typedef enum { NODE_CONST, NODE_UNARY, NODE_BINARY, NODE_ERROR, } NodeTag;

typedef struct Node {
	NodeTag type;

	union {
		float const_value;

		struct {
			UnCode un;
			struct Node* operand;
		} unop_value;

		struct {
			OpCode op;
			struct Node* left;
			struct Node* right;
		} binop_value;
	};
} Node;

void freeRootNode(Node*);
Node* parse(TokenArray*);

#endif
