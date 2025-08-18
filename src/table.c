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

static void freeEntry(Entry* entry) {
	if (entry->next != NULL) { freeEntry(entry->next); }
	free(entry->key);
	free(entry);
}

void freeHashMap(HashMap* map) {
	for (int i = 0; i < HASHMAP_SIZE; i++) {
		Entry* entry = map->bucket[i];
		if (entry != NULL) {
			printf("NOT NULL\n");
			freeEntry(entry);
		}
	}
}

static char* my_strdup(const char* string) {
	if (string == NULL) return NULL;
	int length = strlen(string) + 1;
	char* copy = calloc(length, sizeof(char));
	if (copy == NULL) return NULL;
	memcpy(copy, string, length);
	return copy;
}
        
bool insertHashMap(HashMap* map, const char* key, Value value) {
	uint32_t index = hashFunction(key) % HASHMAP_SIZE;

	Entry* entry = malloc(sizeof(Entry));
	if (entry == NULL) return false;
	entry->key = my_strdup(key);
	if (entry->key == NULL) return false;
	entry->value = value;

	if (map->bucket[index] == NULL) {
		map->bucket[index] = entry;
		map->bucket[index]->next = NULL;
		return entry;
	}
	entry->next = map->bucket[index];
	map->bucket[index] = entry;
	return true;
}

static bool cmpKeys(const char* key, const char* entryKey) {
	return (key != NULL && entryKey != NULL && strcmp(key, entryKey));
}

Entry* getHashMap(HashMap* map, const char* key) {
	uint32_t index = hashFunction(key) % HASHMAP_SIZE;
	Entry* entry = map->bucket[index];
	if (entry == NULL) return NULL;

	for (;entry != NULL && cmpKeys(key, entry->key); entry = entry->next) {}

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
