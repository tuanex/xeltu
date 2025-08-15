#include <stdio.h>
#include <string.h>

#include "common.h"
#include "function.h"
#include "lexer.h"
#include "parser.h"

static void repl() {
	char line[1024];
	for (;;) {
		printf("> ");

		if (!fgets(line, sizeof(line), stdin)) {
			printf("\n");
			break;
		}

		line[strcspn(line, "\r\n")] = 0;

		TokenArray list;
		initTokenArray(&list);
		tokenize(line, &list);

		//Node* root = parse(&list);

		//Result result = compile(root);
		//printf(": ");
		//printResult(result);

		//freeNode(root);
		freeTokenArray(&list);
	}
}

int main() {
	repl();
}
