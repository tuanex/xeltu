#ifndef XELTU_DEBUG_H
#define XELTU_DEBUG_H

#include "common.h"
#include "evaluater.h"
#include "parser.h"
#include "scanner.h"

void disassembleToken(Token);
void disassembleTokenArray(TokenArray);

void disassembleOpCode(OpCode, int);
void disassembleRootNode(Node*, int);

void disassembleResult(Result);

#endif
