#ifndef DEBUG_H
#define DEBUG_H

#include "common.h"
#include "lexer.h"
#include "parser.h"
#include "value.h"

void disassembleToken(Token);
void disassembleTokenArray(TokenArray*);

void disassembleNode(Value, int);
void disassembleRootNode(Node*, int);

#endif
