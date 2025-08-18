#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table.h"

static uint32_t hashFunction(const char* key) {
	uint32_t hash = 5381;
	int c;
	while ((c = *key++)) {
		hash = ((hash << 5) + hash) + c;
	}
	return hash % HASHMAP_SIZE;
}

void initHashMap(HashMap* map) {
	for (int i = 0; i < HASHMAP_SIZE; i++) {
		map->bucket[i] = NULL;
	}
}

void freeHashMap(HashMap* map) {
	for (int i = 0; i < HASHMAP_SIZE; i++) {
		free(map->bucket[i]);
	}
}
        
bool insertHashMap(HashMap* map, const char* key, Value value) {
	uint32_t index = hashFunction(key);
	if (map->bucket[index] != NULL) {
		Entry* oldEntry = map->bucket[index];
		map->bucket[index] = malloc(sizeof(Entry));
		map->bucket[index]->key = key;
		map->bucket[index]->value = value;
		map->bucket[index]->next = oldEntry;
	}
	else {
		map->bucket[index] = malloc(sizeof(Entry));
		map->bucket[index]->key = key;
		map->bucket[index]->value = value;
		map->bucket[index]->next = NULL;
	}
	return (map->bucket[index] != NULL);
}

Entry* getHashMap(HashMap* map, const char* key) {
	uint32_t index = hashFunction(key) % HASHMAP_SIZE;
	Entry* entry = map->bucket[index];
	for (; entry != NULL && strcmp(key, entry->key);) {
		entry = map->bucket[index]->next;
	}
	return entry;
}

void setHashMap(HashMap* map, const char* key, Value value) {
	uint32_t index = hashFunction(key) % HASHMAP_SIZE;
	printf("key %s\n", key);
	for (;;) {
		if (map->bucket[index]->key == key) {
			map->bucket[index]->value = value;
			return;
		}
		index = (index + 1) & HASHMAP_SIZE;
	}
	insertHashMap(map, key, value);
}
