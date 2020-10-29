#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

enum {
	EXP_FUNCTION = -20,
	EXP_VAR = -21,
	EXP_ARRAY = -22,
	EXP_POINTER = -23,
	EXP_STRUCT,
	EXP_TYPE = -10,
	EXP_OTHER
};

char* strdup(const char* src) {
	char* dist;
	if (src == NULL)
		return NULL;
	dist = malloc(strlen(src) + 1);
	strcpy(dist, src);
	return dist;
}

char* itoa(int val){
	static char buf[32] = {0};
	char* cr = "0123456789";
	int i = 30;
	for(; val && i ; --i, val /= 10)
		buf[i] = cr[val % 10];
	return &buf[i+1];
}

void freeAll(int count, ...){
	va_list ap;
	va_start(ap, count);
		free(va_arg(ap, char*));
	va_end(ap);
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
	char* value;
	int token;
	struct Exp* type;
	struct Exp* next;
} Exp;

Exp* newExp(char* value, const int token, Exp* type) {
	Exp* exp = malloc(sizeof(Exp));
	exp->value = value;
	exp->token = token;
	exp->type = type;
	exp->next = NULL;
	return exp;
}

void freeExp(Exp* exp) {
	if (exp != NULL) {
		if (exp->token != EXP_POINTER || exp->token != EXP_FUNCTION || exp->token != EXP_VAR || exp->token != EXP_TYPE){
			free(exp->value);
			freeExp(exp->type);
			freeExp(exp->next);
			free(exp);
		}
	}
}
