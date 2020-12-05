#include <string.h>
#include "converter.c"

void checkDef(Exp* exp) {
	if (exp->token == IDENTIFIER && !exp->type)
		yyerror(concat(3, "error: '", exp->value, "' undeclared"));
}

void checkNotDef(Exp* exp){
	if (getInThisScope(exp->value))
		yyerror(concat(3, "error: '", exp->value, "' already declared"));
}

void concatExp(Exp** exp, Exp* exp1, Exp* exp2) {
	*exp = newExp(concat(2, exp1->value, exp2->value), EXP_OTHER);
	freeAllExp(2, exp1, exp2);
}

void primary_exp2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	checkDef(exp2);
	*exp = newExp3(concat(3, exp1->value, exp2->value, exp3->value), EXP_OTHER, exp2->type);
	freeAllExp(3, exp1, exp2, exp3);
}

void primary_exp3(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3, Exp* exp4){
	if(exp1->token == EXP_FUNCTION || exp1->token == EXP_VAR)
		yyerror(concat(3, "error: '", exp1->value, " is not an array or a pointer"));

	if(exp3->type && strcmp(exp3->type->value, "int"))
		yyerror(concat(4, "error: index has no integer type '", exp3->type->value, "' = ", exp3->value));

	*exp = newExp3(concat(4, exp1->value, exp2->value, exp3->value, exp4->value), EXP_OTHER, exp1->type);
	freeAllExp(4, exp1, exp2, exp3, exp4);
}

void primary_exp4(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	checkDef(exp1);
	if (exp1->token != EXP_FUNCTION) {
		yyerror(concat(3, "error: '", exp1->value, "' is not a function"));
	}
	if (exp1->next) {
		yyerror(concat(3, "error: the '", exp1->value, "' function requires parameters"));
	}
	*exp = newExp3(concat(3, exp1->value, exp2->value, exp3->value), EXP_OTHER, exp1->type);
	freeAllExp(3, exp1, exp2, exp3);
}

void primary_exp5(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3, Exp* exp4){
	Exp* p = exp1->next;
	Exp* e = exp3;
	char* params = exp3->value;
	checkDef(exp1);
	if (exp1->token != EXP_FUNCTION) {
		yyerror(concat(3, "error: '", exp1->value, "' is not a function"));
	}
	if (!p) {
		yyerror(concat(3, "error: '", exp1->value, "' function does not receive parameters"));
	}
	while (p && e && p->token != ELLIPSIS) {
		if (e != exp3)
			params = concat(3, params, ",", e->value);
		e = e->next;
		p = p->next;
	}
	if (!p && e) {
		yyerror(concat(3, "error: more parameters than expected in function '", exp1->value, "'"));
	} else if (p && !e && p->token != ELLIPSIS) {
		yyerror(concat(3, "error: fewer parameters than expected in function '", exp1->value, "'"));
	}
	while (e) {
		params = concat(3, params, ",", e->value);
		e = e->next;
	}
	*exp = newExp3(concat(4, exp1->value, exp2->value, params, exp4->value), EXP_OTHER, exp1->type);
	freeAllExp(3, exp2, exp3, exp4);
}

void unary_exp67(Exp** exp, Exp* exp1, Exp* exp2){
	checkDef(exp2);
	*exp = newExp3(concat(2, exp1->value, exp2->value), EXP_OTHER, exp2->type);
	freeExp(exp1);
}

void unary_exp89(Exp** exp, Exp* exp1, Exp* exp2){
	checkDef(exp1);
	*exp = newExp3(concat(2, exp1->value, exp2->value), EXP_OTHER, exp1->type);
	freeExp(exp2);
}

void unary_exp10(Exp** exp, Exp* exp1, Exp* exp2){
	char* tmp = exp2->value;
	exp2->value = concat(2, exp1->value, tmp);
	*exp = exp2;
	free(tmp);
	freeExp(exp1);
}

void binary_exp2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	checkDef(exp1);
	checkDef(exp3);
	*exp = newExp(concat(5, exp1->value, " ", exp2->value, " ", exp3->value), EXP_OTHER);
	freeAllExp(3, exp1, exp2, exp3);
}

void complex_exp2(Exp** exp, Exp* exp1, Exp* exp2){
	checkDef(exp2);
	*exp = newExp(concat(2, exp1->value, exp2->value), INTEGER_LITERAL);
	freeAllExp(2, exp1, exp2);
}

void complex_exp6(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3, Exp* exp4, Exp* exp5){
	checkDef(exp2);
	*exp = newExp(concat(5, exp1->value, exp2->value, exp3->value, exp4->value, exp5->value), EXP_OTHER);
	freeAllExp(2, exp1, exp2, exp3, exp4, exp5);
}

void assignment_exp(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	checkDef(exp1);
	checkDef(exp3);
	*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), EXP_OTHER);
	freeAllExp(3, exp1, exp2, exp3);
}

void exp_list(Exp* exp1, Exp* exp3){
	Exp* e = exp1;
	while (e->next){
		e = e->next;
	}
	e->next = exp3;
}

void declaration(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	Exp* declarator = exp2->next;
	Exp* identifier;
	Exp* tmp;

	while(declarator){
		identifier = declarator->type;
		if(get(identifier->value))
			yyerror(concat(3, "error: '", identifier->value, "' already declared"));

		identifier->type = exp1->type;
		identifier->token = declarator->token;
		add(identifier);
		tmp = declarator;
		declarator = declarator->next;
		tmp->token = EXP_OTHER;
		/*freeExp(tmp);*/
	}
	*exp = newExp(concat(4, exp1->value, " ", exp2->value, exp3->value), EXP_OTHER);
	exp1->type = NULL;
	freeAllExp(3, exp1, exp2, exp3);
}

void init_declarator_list1(Exp** exp, Exp* exp1){
	*exp = newExp(strdup(exp1->value), EXP_OTHER);
	(*exp)->next = exp1;
}

void init_declarator_list2(Exp* exp1, Exp* exp2, Exp* exp3){
	char* tmp;
	Exp* last;

	tmp = exp1->value;
	exp1->value = concat(3, exp1->value, exp2->value, exp3->value);
	free(tmp);

	last = exp1;
	while(last->next) {
		last = last->next;
	}
	last->next = exp3;

	freeExp(exp2);
}

void init_declarator(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	(*exp)->value = concat(3, exp1->value, exp2->value, exp3->value);
}

void direct_declarator1(Exp** exp, Exp* exp1){
	checkNotDef(exp1);
	*exp = newExp3(strdup(exp1->value), EXP_VAR, exp1);
}

void direct_declarator3(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3, Exp* exp4){
	if (exp1->token == EXP_VAR || exp1->token == EXP_ARRAY || exp1->token == EXP_POINTER){
		int exp_token;
		if(exp3->type && strcmp(exp3->type->value, "int"))
			yyerror(concat(4, "error: size of array has no integer type '", exp3->type->value, "' = ", exp3->value));

		exp_token = exp1->token == EXP_POINTER ? EXP_POINTER : EXP_ARRAY;
		*exp = newExp3(concat(4, exp1->value, exp2->value, exp3->value, exp4->value), exp_token, exp1->type);
		exp1->token = EXP_OTHER;
		freeAllExp(4, exp1, exp2, exp3, exp4);
	}
	else{
		yyerror(concat(6, "error: '", exp1->value, exp2->value, exp3->value, exp4->value, "'is not a valid declaration"));
	}
}

void direct_declarator4(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	if (exp1->token == EXP_VAR)
		*exp = newExp3(concat(3, exp1->value, exp2->value, exp3->value), EXP_POINTER, exp1->type);
	else
		yyerror(concat(5, "error: '", exp1->value, exp2->value, exp3->value, "'is not a valid declaration"));

	exp1->token = EXP_OTHER;
	freeAllExp(3, exp1, exp2, exp3);
}

void direct_declarator5(Exp* exp1, Exp* exp2, Exp* exp3, Exp* exp4){
	if (exp1->token == EXP_VAR){
		exp1->next = exp3;
		exp1->token = EXP_FUNCTION;
		freeAllExp(2, exp2, exp4);
	}
	else
		yyerror(concat(6, "error: '", exp1->value, "' is not a valid declaration"));
}

void direct_declarator7(Exp* exp1, Exp* exp2, Exp* exp3){
	if (exp1->token == EXP_VAR){
		exp1->value = concat(3, exp1->value, exp2->value, exp3->value);
		exp1->token = EXP_FUNCTION;
		freeAllExp(2, exp2, exp3);
	}
	else
		yyerror(concat(6, "error: '", exp1->value, "' is not a valid declaration"));
}

void parameter_list2(Exp* exp1, Exp* exp2, Exp* exp3){
	Exp* last = exp1;
	while(last->next){
		last = last->next;
	}
	last->next = exp3;
	freeExp(exp2);
}

void paramater_declaration(Exp** exp, Exp* exp1, Exp* exp2){
	if(exp1->type)
		exp2->type = exp1->type;
	else
		exp2->type = get(exp1->value);
	*exp = exp2;
}

void function_def_declaration(Exp* exp1, Exp* exp2){
	Exp* p = exp2->next;
	if (exp2->token != EXP_FUNCTION)
		yyerror(concat(5, "error: '", exp1->value, " ", exp2->value, "'is not a valid function declaration"));
	if(scopeControl->scope != 0)
		yyerror("error: functions can only be defined in the global scope");
	exp2->type = get(exp1->value);
	add(exp2);
	inScope();
	while (p) {
		add(p);
		p = p->next;
	}
}

void function_def(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp4){
	Exp* p = exp2->next;
	char* value = NULL;
	char* tmp;
	outScope();
	while (p && p->type) {
		if (value) {
			tmp = value;
			value = concat(5, value, ",", p->type->value, " ", p->value);
			free(tmp);
		} else
			value = concat(3, p->type->value, " ", p->value);
		p = p->next;
	}
	value = value ? concat(3, "(", value, ")") : "";
	*exp = newExp(concat(5, exp1->value, " ", exp2->value, value, exp4->value), EXP_OTHER);
	freeAllExp(2, exp1, exp4);
}

void selection_stm1(Exp** exp, Exp* exp3, Exp* exp5){
	*exp = newExp(getIf(exp3->value, exp5->value), EXP_OTHER);
	freeAllExp(2, exp3, exp5);
}

void selection_stm2(Exp** exp, Exp* exp3, Exp* exp5, Exp* exp7){
	*exp = newExp(getIfElse(exp3->value, exp5->value, exp7->value), EXP_OTHER);
	freeAllExp(3, exp3, exp5, exp7);
}

void iteration_stm1(Exp** exp, Exp* exp3, Exp* exp5){
	*exp = newExp(getWhile(exp3->value, exp5->value), EXP_OTHER);
	freeAllExp(2, exp3, exp5);
}

void iteration_stm2(Exp** exp, Exp* exp2, Exp* exp5){
	*exp = newExp(getDoWhile(exp5->value, exp2->value), EXP_OTHER);
	freeAllExp(2, exp2, exp5);
}

void iteration_stm34(Exp** exp, Exp* exp3, Exp* exp4, Exp* exp5, Exp* exp7){
	*exp = newExp(getFor(exp3->value, exp4->value, exp5 ? exp5->value : NULL, exp7->value), EXP_OTHER);
	freeAllExp(2, exp3, exp4, exp5, exp7);
}
