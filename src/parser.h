#ifndef XELTU_PARSER_H
#define XELTU_PARSER_H

#include "common.h"
#include "scanner.h"
#include "value.h"

typedef enum { UNOP_POS, UNOP_NEG } UnCode;
typedef enum { BINOP_ADD, BINOP_SUB, BINOP_MUL, BINOP_DIV, BINOP_ASS} OpCode;

typedef enum { NODE_LEAF, NODE_UNARY, NODE_BINARY, NODE_ERROR, } NodeType;

typedef struct Node {
	NodeType type;

	union {
		Value value;

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
