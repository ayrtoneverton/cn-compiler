#include <limits.h>
#include <string.h>
#include "comum.c"

typedef struct SymbolTable {
	unsigned int size;
	struct Item** list;
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
	if((scopeControl->tables = malloc(sizeof(SymbolTable*)* scopeControl->size)) == NULL) {
		exit(1);
	}
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

	if((table = malloc(sizeof(SymbolTable))) == NULL) {
		exit(1);
	}
	table->size = 500;
	if((table->list = malloc(sizeof(Item*)* table->size)) == NULL) {
		exit(1);
	}
	for(i = 0; i < table->size; i++) {
		table->list[i] = NULL;
	}
	scopeControl->tables[scopeControl->scope] = table;
}

void outScope() {
	unsigned int i;
	Item* item;
	SymbolTable* table = scopeControl->tables[scopeControl->scope];
	for(i = 0; i < table->size; i++) {
		item = table->list[i]->next;
		while(item != NULL) {
			free(item);
			item = item->next;
		}
		free(table->list[i]);
	}
	free(table->list);
	free(table);
	scopeControl->scope--;
}

void initSymbolTable() {
	if((scopeControl = malloc(sizeof(ScopeControl))) == NULL) {
		exit(1);
	}
	scopeControl->size = 0;
	scopeControl->scope = 0;
	inScope();
}

int getHash(char* key) {
	unsigned long int hashval = 0;
	unsigned int i = 0;
	while(hashval < ULONG_MAX && i < strlen(key)) {
		hashval = hashval << 8;
		hashval += key[ i ];
		i++;
	}
	return hashval % scopeControl->tables[scopeControl->scope]->size;
}

void add(char* key, char* value) {
	Item* newpair = NULL;
	Item* last = NULL;
	int hash = getHash(key);
	Item* next = scopeControl->tables[scopeControl->scope]->list[ hash ];

	while(next != NULL && next->key != NULL && strcmp(key, next->key) > 0) {
		last = next;
		next = next->next;
	}

	if(next != NULL && next->key != NULL && strcmp(key, next->key) == 0) {
		free(next->value);
		next->value = strdup(value);
	} else {
		if((newpair = malloc(sizeof(Item))) == NULL) {
			newpair = NULL;
		}else{
			if((newpair->key = strdup(key)) == NULL) {
				newpair = NULL;
			}
			if((newpair->value = strdup(value)) == NULL) {
				newpair = NULL;
			}
			newpair->next = NULL;
		}

		/* We're at the start of the linked list in this hash. */
		if(next == scopeControl->tables[scopeControl->scope]->list[ hash ]) {
			newpair->next = next;
			scopeControl->tables[scopeControl->scope]->list[ hash ] = newpair;

		/* We're at the end of the linked list in this hash. */
		} else if (next == NULL) {
			last->next = newpair;

		/* We're in the middle of the list. */
		} else {
			newpair->next = next;
			last->next = newpair;
		}
	}
}

char* get(char* key) {
	Item* pair = scopeControl->tables[scopeControl->scope]->list[ getHash(key) ];
	while(pair != NULL && pair->key != NULL && strcmp(key, pair->key) > 0) {
		pair = pair->next;
	}
	if(pair == NULL || pair->key == NULL || strcmp(key, pair->key) != 0) {
		return NULL;
	} else {
		return pair->value;
	}
}
