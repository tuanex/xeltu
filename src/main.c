#include <stdio.h>
#include <string.h>

#include "debug.h"

#include "common.h"
#include "evaluater.h"
#include "scanner.h"
#include "memory.h"
#include "parser.h"
#include "table.h"

void repl() {
	char line[128];
	HashMap map;
	initHashMap(&map);

	for (;;) {
		printf("> ");

		if (!fgets(line, sizeof(line), stdin)) {
			printf("\n");
			continue;
		}

		line[strcspn(line, "\r\n")] = 0;

		TokenArray tokens;
		ScanningResult scanResult = scan(&tokens, line);

		switch (scanResult ) {
case SCANNING_ERROR:
	printf ("\033[31;1mThere was an error.\033[0m\n");
	continue;
default:
	break;
		}

		Node* root = parse(&tokens);

		Result result = evaluate(root, &map);

		if (result.type == RESULT_CONST) 
			printf(": %f\n", result.value.constant);

		freeRootNode(root);
		freeTokenArray(&tokens);
	}
	freeHashMap(&map);
}

int main() {
	repl();
	return 0;
}
