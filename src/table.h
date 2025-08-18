#ifndef XELTU_TABLE_H
#define XELTU_TABLE_H

#include "common.h"
#include "value.h"

typedef struct Entry {
	char* key;
	Value value;
	struct Entry* next;
} Entry;

#define HASHMAP_SIZE 16
typedef struct HashMap {
	Entry* bucket[HASHMAP_SIZE];
} HashMap;

void initHashMap(HashMap*);
void freeHashMap(HashMap*);
bool insertHashMap(HashMap*, const char*, Value);
Entry* getHashMap(HashMap*, const char*);
void setHashMap(HashMap*, const char*, Value);

#endif
