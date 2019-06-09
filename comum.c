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

Exp* newExp(char* name, const int type, char* value){
	Exp* exp = malloc(sizeof(Exp));
	exp->name = name;
	exp->type = type;
	exp->value = value;
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

void freeAllExp(int count, ...){
	va_list ap;
	va_start(ap, count);
		freeExp(va_arg(ap, Exp*));
	va_end(ap);
}

#endif
