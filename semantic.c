#include <string.h>
#include "syntactic.tab.h"
#include "symbol-table.c"
#include "converter.c"

void freeAllExp(int count, ...) {
	Exp* exp;
	va_list ap;
	va_start(ap, count);
		exp = va_arg(ap, Exp*);
			freeExp(exp);
	va_end(ap);
}

void concatExp(Exp** exp, Exp* exp1, Exp* exp2) {
	*exp = newExp(concat(2, exp1->value, exp2->value), EXP_OTHER, NULL);
	freeAllExp(2, exp1, exp2);
}
void nextExp(Exp** exp, Exp* exp1, Exp* exp2) {
	char* tmp = exp1->value;
	exp1->value = concat(2, exp1->value, exp2->value);
	exp1->next = exp2;
	*exp = exp1;
	free(tmp);
}

void checkDef(Exp* exp) {
	if (exp->token == IDENTIFIER && exp->type == NULL)
		yyerror(concat(3, "error: '", exp->value, "' undeclared"));
}

void checkNotDef(Exp* exp){		
	if (get(exp->value) != NULL)
		yyerror(concat(3, "error: '", exp->value, "' already declared"));
}

void primary_exp1(Exp** exp, Exp* exp1){
	Exp * id;
	if(exp1->type == NULL){
		Exp * id = getAll(exp1->value);
		if(id == NULL)
			yyerror(concat(3, "error: '", exp1->value, "' undeclared"));
		freeExp(exp1);
	}else{
		id = exp1;
	}
	*exp = newExp(strdup(id->value), id->token, id->type);
}

void primary_exp2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), exp2->token, exp2->type);
	exp2->type = NULL;
	freeAllExp(3, exp1, exp2, exp3);
}

void primary_exp3(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3, Exp* exp4){
	if(exp1->token == EXP_FUNCTION || exp1->token == EXP_VAR)
		yyerror(concat(3, "error: '", exp1->value, " is not an array or a pointer"));

	if(strcmp(exp3->type->value, "int"))
		yyerror(concat(4, "error: index has no integer type '", exp3->type->value, "' = ", exp3->value));

	*exp = newExp(concat(4, exp1->value, exp2->value, exp3->value, exp4->value), exp1->token, exp1->type);
	exp1->type = NULL;
	freeAllExp(4, exp1, exp2, exp3, exp4);
}

void primary_exp4(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	char* tmp = exp1->value;
	checkDef(exp1);
	exp1->value = concat(3, exp1->value, exp2->value, exp3->value);
	*exp = exp1;
	free(tmp);
	freeAllExp(2, exp2, exp3);
}

void primary_exp5(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3, Exp* exp4){
	char* tmp = exp1->value;
	checkDef(exp1);
	exp1->value = concat(4, exp1->value, exp2->value, exp3->value, exp4->value);
	*exp = exp1;
	free(tmp);
	freeAllExp(3, exp2, exp3, exp4);
}

void unary_exp67(Exp** exp, Exp* exp1, Exp* exp2){
	checkDef(exp2);
	free(exp2->value);
	exp2->value = concat(2, exp1->value, exp2->value);
	*exp = exp2;
	freeExp(exp1);
}

void unary_exp89(Exp** exp, Exp* exp1, Exp* exp2){
	checkDef(exp1);
	free(exp1->value);
	exp1->value = concat(2, exp1->value, exp2->value);
	*exp = exp1;
	freeExp(exp2);
}

void binary_exp2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	checkDef(exp1);
	checkDef(exp3);
	*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), exp2->token, NULL);
	freeAllExp(3, exp1, exp2, exp3);
}

void complex_exp2(Exp** exp, Exp* exp1, Exp* exp2){
	checkDef(exp2);
	*exp = newExp(concat(2, exp1->value, exp2->value), INTEGER_LITERAL, NULL);
	freeAllExp(2, exp1, exp2);
}

void assignment_exp(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	char* tmp = exp1->value;
	checkDef(exp1);
	checkDef(exp3);
	exp1->value = concat(3, exp1->value, exp2->value, exp3->value);
	*exp = exp1;
	free(tmp);
	freeAllExp(2, exp2, exp3);
}

void exp_list(Exp** exp, Exp* exp1, Exp* exp2){
	checkDef(exp1);
	checkDef(exp2);
	exp1-> next = exp2;
	*exp = exp1;
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
		identifier->token = declarator->token;
		add(identifier);
		tmp = declarator;
		declarator = declarator->next;
		tmp->token = EXP_OTHER;
		/*freeExp(tmp);*/
	}
	*exp = newExp(concat(4, exp1->value, " ", exp2->value, exp3->value), EXP_OTHER, NULL);
	exp1->type = NULL;
	freeAllExp(3, exp1, exp2, exp3);
}

void declaration_specifiers3(Exp** exp, Exp* exp1){
	*exp = newExp(strdup(exp1->value), EXP_OTHER, getAll(exp1->value));
	freeExp(exp1);
}

void init_declarator_list1(Exp** exp, Exp* exp1){
	*exp = newExp(strdup(exp1->value), EXP_OTHER, NULL);
	(*exp)->next = exp1;
}

void init_declarator_list2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	char* tmp;
	Exp* last;

	*exp = exp1;
	tmp = (*exp)->value;
	(*exp)->value = concat(3, exp1->value, exp2->value, exp3->value);
	free(tmp);

	last = *exp;
	while(last->next != NULL){
		last = last->next;
	}
	last->next = exp3;

	freeExp(exp2);
}

void direct_declarator1(Exp** exp, Exp* exp1){
	checkNotDef(exp1);
	*exp = newExp(strdup(exp1->value), EXP_VAR, exp1);
}

void direct_declarator3(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3, Exp* exp4){
	if (exp1->token == EXP_VAR || exp1->token == EXP_ARRAY || exp1->token == EXP_POINTER){
		int exp_token;
		if(strcmp(exp3->type->value, "int"))
			yyerror(concat(4, "error: size of array has no integer type '", exp3->type->value, "' = ", exp3->value));

		exp_token = exp1->token == EXP_POINTER ? EXP_POINTER : EXP_ARRAY;
		*exp = newExp(concat(4, exp1->value, exp2->value, exp3->value, exp4->value), exp_token, exp1->type);
		exp1->token = EXP_OTHER;
		freeAllExp(4, exp1, exp2, exp3, exp4);
	}
	else{
		yyerror(concat(6, "error: '", exp1->value, exp2->value, exp3->value, exp4->value, "'is not a valid declaration"));
	}
}

void direct_declarator4(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	if (exp1->token == EXP_VAR)
		*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), EXP_POINTER, exp1->type);
	else
		yyerror(concat(5, "error: '", exp1->value, exp2->value, exp3->value, "'is not a valid declaration"));
	
	exp1->token = EXP_OTHER;
	freeAllExp(3, exp1, exp2, exp3);
}

void direct_declarator5(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp4, Exp* exp5){
	if (exp1->token == EXP_VAR){
		*exp = newExp(concat(4, exp1->value, exp2->value, exp4->value, exp5->value), EXP_FUNCTION, exp1->type);
		(*exp)->next = exp4;
		exp1->token = EXP_OTHER;
		freeAllExp(3, exp1, exp2, exp5);
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
	if (exp1->token == EXP_VAR){
		*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), EXP_FUNCTION, exp1->type);
		exp1->token = EXP_OTHER;
		freeAllExp(3, exp1, exp2, exp3);
	}
	else
		yyerror(concat(6, "error: '", exp1->value, exp2->value, exp3->value, "'is not a valid declaration"));
}

void parameter_list1(Exp** exp, Exp* exp1){
	*exp = newExp(concat(3, exp1->type->value, " " , exp1->value), EXP_OTHER, NULL);
	(*exp)->next = exp1;
}

void parameter_list2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	Exp* last;
	char* tmp;

	*exp = exp1;
	tmp = (*exp)->value;
	(*exp)->value = concat(5, exp1->value, exp2->value, exp3->type->value, " ", exp3->value);
	free(tmp);

	last = *exp;
	while(last->next != NULL){
		last = last->next;
	}
	last->next = exp3;
}

void paramater_declaration(Exp** exp, Exp* exp1, Exp* exp2){
	*exp = exp2;
	(*exp)->type->type = exp1->type;
}

void function_def_declaration(Exp* exp1, Exp* exp2){
	Exp* par;
	par = exp2->next;
	if (exp2->token != EXP_FUNCTION)
		yyerror(concat(5, "error: '", exp1->value, " ", exp2->value, "'is not a valid function declaration"));
	if(par != NULL){
		par = par->next;
		while(par != NULL){
			Exp* local_var = par->type;
			local_var->token = par->token;
			par->type = local_var->type;
			add(local_var);
			par = par->next;
		}
	}
}

void function_def(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp4){
	Exp* function;
	outScope();
	function = exp2->type;
	function->type = exp1->type;
	function->token = EXP_FUNCTION;
	add(function);
	*exp = newExp(concat(4, exp1->value, " ", exp2->value, exp4->value), EXP_OTHER, NULL);
	exp1->type = NULL;
	exp2->type = NULL;
	freeAllExp(2, exp1, exp2);
}

void stm3(Exp** exp, Exp* exp1, Exp* exp2){
	char* tmp = exp1->value;
	exp1->value = concat(2, exp1->value, exp2->value);
	*exp = exp1;
	free(tmp);
	freeExp(exp2);
}

void selection_stm1(Exp** exp, Exp* exp3, Exp* exp5){
	*exp = newExp(getIf(exp3->value, exp5->value), EXP_OTHER, NULL);
	freeAllExp(2, exp3, exp5);
}

void selection_stm2(Exp** exp, Exp* exp3, Exp* exp5, Exp* exp7){
	*exp = newExp(getIfElse(exp3->value, exp5->value, exp7->value), EXP_OTHER, NULL);
	freeAllExp(3, exp3, exp5, exp7);
}

void iteration_stm1(Exp** exp, Exp* exp3, Exp* exp5){
	*exp = newExp(getWhile(exp3->value, exp5->value), EXP_OTHER, NULL);
	freeAllExp(2, exp3, exp5);
}

void iteration_stm2(Exp** exp, Exp* exp2, Exp* exp5){
	*exp = newExp(getDoWhile(exp5->value, exp2->value), EXP_OTHER, NULL);
	freeAllExp(2, exp2, exp5);
}

void iteration_stm34(Exp** exp, Exp* exp3, Exp* exp4, Exp* exp5, Exp* exp6){
	*exp = newExp(getFor(exp3->value, exp4->value, exp5 ? exp5->value : NULL, exp6->value), EXP_OTHER, NULL);
	freeAllExp(2, exp3, exp4, exp5, exp6);
}
