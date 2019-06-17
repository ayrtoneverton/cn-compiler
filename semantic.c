#include <string.h>
#include "syntactic.tab.h"
#include "symbol-table.c"
#include "converter.c"

void freeAllExp(int count, ...) {
	Exp* exp;
	va_list ap;
	va_start(ap, count);
		exp = va_arg(ap, Exp*);
		if (exp->token != IDENTIFIER)
			freeExp(exp);
	va_end(ap);
}

/* Procurar nos escopos acima */
void checkDef(Exp* exp) {
	if (exp->token == IDENTIFIER && exp->type == NULL)
		yyerror(concat(3, "error: '", exp->value, "' undeclared"));
}

void checkNotDef(Exp* exp){	
	if (exp->type != NULL)
		yyerror(concat(3, "error: '", exp->value, "' already declared"));
}

void primary_exp2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	checkDef(exp2);
	*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), exp2->token, NULL);
	freeAllExp(3, exp1, exp2, exp3);
}

void binary_exp2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	checkDef(exp1);
	checkDef(exp3);
	*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), exp2->token, NULL);
	freeAllExp(3, exp1, exp2, exp3);
	/* printf("%s\n", (*exp)->value); */
}

void declaration(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	Exp* declarator = exp2->next;
	Exp* identifier;
	Exp* tmp;

	while(declarator != NULL){
		identifier = declarator->type;
		if(get(identifier->value) != NULL)
			yyerror(concat(3, "error: '", identifier->value, "' already declared"));

		identifier->type = exp1->type; 
		add(identifier);		
		tmp = declarator;
		declarator = declarator->next;
		tmp->type = NULL;
		tmp->next = NULL;
		freeExp(tmp);
	}
	*exp = newExp(concat(4, exp1->value, " ", exp2->value, exp3->value), EXP_OTHER, NULL);
	exp1->type = NULL;
	freeAllExp(2, exp1, exp2);	
	/* printf("D: %s\n", (*exp)->value); */
}

void declaration_specifiers3(Exp** exp, Exp* exp1){
	*exp = newExp(exp1->value, EXP_OTHER, getAll(exp1->value));
	exp1->value = NULL;
	freeExp(exp1);
	/* printf("DE3: %s\n", (*exp)->type->value); */
}

void init_declarator_list1(Exp** exp, Exp* exp1){
	*exp = newExp(concat(1, exp1->value), EXP_OTHER, NULL);
	(*exp)->next = exp1;
	/* printf("IDL1: %s\n", (*exp)->value); */
}

void init_declarator_list2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	char* tmp;
	Exp* last;

	*exp = exp1;
	tmp = (*exp)->value;	
	(*exp)->value = concat(3, exp1->value, exp2->value, exp3->value);
	free(tmp);

	last = (*exp)->next;
	while(last->next != NULL){
		last = last->next;
	}
	last->next = exp3;
	/* printf("IDL2: %s\n", (*exp)->value); */
}

void direct_declarator1(Exp** exp, Exp* exp1){
	checkNotDef(exp1);
	*exp = newExp(strdup(exp1->value), IDENTIFIER, exp1);
	/* printf("DD1: %s\n", (*exp)->value); */
}

void direct_declarator3(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3, Exp* exp4){
	if (exp1->token == IDENTIFIER || exp1->token == DEC_ARRAY || exp1->token == DEC_POINTER){
		int exp_token;
		if(strcmp(exp3->type->value, "int"))
			yyerror(concat(4, "error: size of array has no integer type '", exp3->type->value, "' = ", exp3->value));
		
		exp_token = exp1->token == DEC_POINTER ? DEC_POINTER_ARRAY : DEC_ARRAY;

		*exp = newExp(concat(4, exp1->value, exp2->value, exp3->value, exp4->value), exp_token, exp1->type);

		freeAllExp(2, exp1, exp3);
	}
	else{
		yyerror(concat(6, "error: '", exp1->value, exp2->value, exp3->value, exp4->value, "'is not a valid declaration"));
	}
}

void direct_declarator4(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	if (exp1->token == IDENTIFIER)
		*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), DEC_POINTER, exp1->type);
	else
		yyerror(concat(5, "error: '", exp1->value, exp2->value, exp3->value, "'is not a valid declaration"));
}

void direct_declarator5(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp4, Exp* exp5){
	if (exp1->token == IDENTIFIER){	
		*exp = newExp(concat(4, exp1->value, exp2->value, exp4->value, exp5->value), DEC_FUNCTION, exp1->type);
		(*exp)->next = exp4;
		/* printf("DD5: %s\n", (*exp)->value); */
	}
	else
		yyerror(concat(6, "error: '", exp1->value, exp2->value, exp4->value, exp5->value, "'is not a valid declaration"));
}

void checkScope(){
	if(scopeControl->scope != 0)
		yyerror("error: Function definition is not allowed here");
	inScope();
}

void direct_declarator7(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	checkScope();
	if (exp1->token == IDENTIFIER){	
		*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), DEC_FUNCTION, exp1->type);
	}
	else
		yyerror(concat(6, "error: '", exp1->value, exp2->value, exp3->value, "'is not a valid declaration"));
}

void parameter_list1(Exp** exp, Exp* exp1){
	*exp = newExp(concat(3, exp1->type->value, " " , exp1->value), EXP_OTHER, NULL);
	(*exp)->next = exp1;
	/* printf("PL1: %s\n", (*exp)->value); */
}

void parameter_list2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	Exp* last;
	char* tmp;

	*exp = exp1;
	tmp = (*exp)->value;
	(*exp)->value = concat(5, exp1->value, exp2->value, exp3->type->value, " ", exp3->value);
	free(tmp);

	last = (*exp)->next;
	while(last->next != NULL){
		last = last->next;
	}
	last->next = exp3;
}

void paramater_declaration(Exp** exp, Exp* exp1, Exp* exp2){
	*exp = exp2;
	(*exp)->type = exp1->type;

	exp1->type = NULL;
	freeExp(exp1);
	/* printf("PD1: %s\n", (*exp)->value); */
}

void function_def_declaration(Exp* exp1, Exp* exp2){
	Exp* par;
	par = exp2->next;
	if (exp2->token != DEC_FUNCTION)
		yyerror(concat(5, "error: '", exp1->value, " ", exp2->value, "'is not a valid function declaration"));
	if(par != NULL){
		par = par->next;
		while(par != NULL){
			Exp* new = newExp(strdup(par->value), IDENTIFIER, par->type);
			add(new);
			par = par->next;			
		}
	}
}

void function_def(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp4){
	Exp* identifier;
	outScope();
	identifier = exp2->type;
	identifier->type = exp1->type;
	add(identifier);
	*exp = newExp(concat(3, exp1->value, exp2->value, exp4->value), EXP_OTHER, NULL);
}

void stm3(Exp** exp, Exp* exp1, Exp* exp2){
	char* tmp = exp1->value;
	exp1->value = concat(2, exp1->value, exp2->value);
	*exp = exp1;
	free(tmp);
	freeExp(exp2);
}

void iteration_stm1(Exp** exp, Exp* exp3, Exp* exp5){
	*exp = newExp(createWhile(exp3->value, exp5->value), EXP_OTHER, NULL);
	printf("%s\n", (*exp)->value);
	freeAllExp(2, exp3, exp5);
}
