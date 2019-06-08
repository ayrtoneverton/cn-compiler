#ifndef comum_c
#define comum_c 0

#include <stdlib.h>
#include <string.h>

char* strdup(const char* src) {
	char* dst = malloc(strlen(src) + 1);
	if (dst == NULL) return NULL;
	strcpy(dst, src);
	return dst;
}

typedef struct Item {
	char* key;
	char* value;
	struct Item* next;
} Item;

#endif