#include <limits.h>
#include <string.h>

typedef struct Item {
	struct Exp* exp;
	struct Item* next;
} Item;

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

Item* newItem(Exp* exp) {
	Item* item = malloc(sizeof(Item));
	item->exp = exp;
	item->next = NULL;
	return item;
}

void freeItem(Item* item) {
	if (item != NULL) {
		/*
		freeExp(item->exp);
		freeItem(item->next);
		free(item);
		*/
	}
}

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
	table->size = 100;
	table->list = malloc(sizeof(Item*) * table->size);
	for(i = 0; i < table->size; i++) {
		table->list[i] = NULL;
	}
	scopeControl->tables[scopeControl->scope] = table;
}

void outScope() {
	unsigned int i;
	SymbolTable* table = scopeControl->tables[scopeControl->scope];
	for(i = 0; i < table->size; i++) {
		freeItem(table->list[i]);
	}
	free(table->list);
	free(table);
	scopeControl->scope--;
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
	int hash = getHash(exp->value);
	Item* last = scopeControl->tables[scopeControl->scope]->list[ hash ];

	while (last != NULL && last->next != NULL && strcmp(exp->value, last->exp->value)) {
		last = last->next;
	}
	if (last == NULL) {
		scopeControl->tables[scopeControl->scope]->list[ hash ] = newItem(exp);
	} else if (!strcmp(exp->value, last->exp->value)) {
		return 1;
	} else {
		last->next = newItem(exp);
	}
	/*if(exp->type == NULL)
		printf("ADDED [%d] NULL %s in SCOPE %d\n", exp->token, exp->value, scopeControl->scope);
	else
		printf("ADDED [%d] %s %s in SCOPE %d\n", exp->token, exp->type->value, exp->value, scopeControl->scope); */
	return 0;
}

Exp* get(const char* name) {
	Item* item = scopeControl->tables[scopeControl->scope]->list[ getHash(name) ];
	while (item != NULL && item->next != NULL  && strcmp(name, item->exp->value)) {
		item = item->next;
	}
	if (item == NULL)
		return NULL;
	return item->exp;
}

Exp* getInScope(int scp, const char* name){
	Item* item = NULL;
	if(scp <= scopeControl->scope){
		item = scopeControl->tables[scp]->list[ getHash(name) ];
		while (item != NULL && item->next != NULL  && strcmp(name, item->exp->value)) {
			item = item->next;
		}
	}
	if (item == NULL)
		return NULL;
	return item->exp;
}

Exp* getAll(const char* name){
	int upScope = scopeControl->scope;
	while (upScope >= 0){
		Exp* exp = getInScope(upScope, name);
		if(exp != NULL)
			return exp;
		upScope --;
	}
	return NULL;
}


void initSymbolTable() {
	scopeControl = malloc(sizeof(ScopeControl));
	scopeControl->size = 0;
	scopeControl->scope = 0;
	inScope();
	add(newExp(strdup("short"), EXP_TYPE, NULL));
	add(newExp(strdup("int"), EXP_TYPE, NULL));
	add(newExp(strdup("long"), EXP_TYPE, NULL));
	add(newExp(strdup("float"), EXP_TYPE, NULL));
	add(newExp(strdup("double"), EXP_TYPE, NULL));
	add(newExp(strdup("void"), EXP_TYPE, NULL));
	add(newExp(strdup("char"), EXP_TYPE, NULL));
	add(newExp(strdup("printf"), EXP_FUNCTION, NULL));
	add(newExp(strdup("scanf"), EXP_FUNCTION, NULL));
}
