#ifndef symbol_table_c
#define symbol_table_c

#include <limits.h>
#include <string.h>
#include "comum.c"

typedef struct SymbolTable {
	unsigned int size;
	struct Exp** list;
} SymbolTable;

typedef struct {
	unsigned int size;
	unsigned int scope;
	struct SymbolTable** tables;
} ScopeControl;

ScopeControl* scopeControl = NULL;

void expandScopeControl() {
	unsigned int i;
	SymbolTable** tablesTmp = scopeControl->tables;
	scopeControl->size += 10;
	scopeControl->tables = malloc(sizeof(SymbolTable*) * scopeControl->size);
	if(scopeControl->scope > 0){
		for(i = 0; i < scopeControl->scope; i++) {
			scopeControl->tables[i] = tablesTmp[i];
		}
		free(tablesTmp);
	}
}

void inScope() {
	int i;
	SymbolTable* table;
	if(scopeControl->scope < scopeControl->size) 
		scopeControl->scope++;
	if(scopeControl->scope == scopeControl->size)
		expandScopeControl();

	table = malloc(sizeof(SymbolTable));
	table->size = 500;
	table->list = malloc(sizeof(Exp*) * table->size);
	for(i = 0; i < table->size; i++) {
		table->list[i] = NULL;
	}
	scopeControl->tables[scopeControl->scope] = table;
}

void outScope() {
	unsigned int i;
	Exp* exp;
	SymbolTable* table = scopeControl->tables[scopeControl->scope];
	for(i = 0; i < table->size; i++) {
		exp = table->list[i]->next;
		while(exp != NULL) {
			free(exp);
			exp = exp->next;
		}
		free(table->list[i]);
	}
	free(table->list);
	free(table);
	scopeControl->scope--;
}

void initSymbolTable() {
	scopeControl = malloc(sizeof(ScopeControl));
	scopeControl->size = 0;
	scopeControl->scope = 0;
	inScope();
}

int getHash(const char* key) {
	unsigned long int hashval = 0;
	unsigned int i = 0;
	while(hashval < ULONG_MAX && i < strlen(key)) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}
	return hashval % scopeControl->tables[scopeControl->scope]->size;
}

int add(Exp* exp) {
	int hash = getHash(exp->name);
	Exp* last = scopeControl->tables[scopeControl->scope]->list[ hash ];

	while (last != NULL && last->next != NULL && strcmp(exp->name, last->name)) {
		last = last->next;
	}
	if (last == NULL) {
		scopeControl->tables[scopeControl->scope]->list[ hash ] = exp;
	} else if (!strcmp(exp->name, last->name)) {
		return 1;
	} else {
		last->next = exp;
	}
	return 0;
}

Exp* get(const char* name) {
	Exp* exp = scopeControl->tables[scopeControl->scope]->list[ getHash(name) ];
	while (exp != NULL && strcmp(name, exp->name)) {
		exp = exp->next;
	}
	return exp;
}

#endif
