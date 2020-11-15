#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <mcheck.h>

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
	char inTable;
} Exp;

Exp* newExp(char* value, const int token) {
	Exp* exp = malloc(sizeof(Exp));
	exp->value = value;
	exp->token = token;
	exp->type = NULL;
	exp->next = NULL;
	exp->inTable = 0;
	return exp;
}
Exp* newExp3(char* value, const int token, Exp* type) {
	Exp* exp = newExp(value, token);
	exp->type = type;
	return exp;
}
Exp* newExp4(char* value, const int token, Exp* type, Exp* next) {
	Exp* exp = newExp(value, token);
	exp->type = type;
	exp->next = next;
	return exp;
}

void freeExp(Exp* exp) {
	if (exp && !mprobe(exp) && !exp->inTable) {
		if(exp->value && mprobe(exp->value)) free(exp->value);
		freeExp(exp->type);
		freeExp(exp->next);
		free(exp);
	}
}

void freeAllExp(int count, ...) {
	va_list ap;
	va_start(ap, count);
		freeExp(va_arg(ap, Exp*));
	va_end(ap);
}
