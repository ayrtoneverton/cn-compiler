#ifndef comum_c
#define comum_c

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

enum {
	EXP_FUNCTION = -20,
	EXP_STRUCT
};

char* strdup(const char* src) {
	char* dist;
	if (src == NULL)
		return NULL;
	dist = malloc(strlen(src) + 1);
	strcpy(dist, src);
	return dist;
}

char* concat2(const int size, char** list) {
	int len = 1;
	char* dist;
	int i;
	for (i = 0; i < size; ++i) {
		len += strlen(list[i]);
	}
	dist = malloc(len);
	strcpy(dist, list[0]);
	for (i = 1; i < size; ++i) {
		strcat(dist, list[i]);
	}
	return dist;
}

char* concat(int count, ...){
	va_list ap;
	int i;
	char* merged;
	int null_pos = 0;
	int len = 1;
	va_start(ap, count);
	for (i = 0; i < count; i++)
		len += strlen(va_arg(ap, char*));
	va_end(ap);
	merged = calloc(sizeof(char), len);
	va_start(ap, count);
	for (i = 0; i < count; i++) {
		char* s = va_arg(ap, char*);
		strcpy(merged + null_pos, s);
		null_pos += strlen(s);
	}
	va_end(ap);
	return merged;
}

typedef struct Exp {
	char* name;
	int type;
	char* value;
	struct Exp* next;
} Exp;

Exp* newExp(const char* name, const int type, const char* value){
	Exp* exp = malloc(sizeof(Exp));
	exp->name = strdup(name);
	exp->type = type;
	exp->value = strdup(value);
	exp->next = NULL;
	return exp;
}

void freeExp(Exp* exp){
	if (exp != NULL) {
		free(exp->name);
		free(exp->value);
		freeExp(exp->next);
		free(exp);
	}
}

#endif
