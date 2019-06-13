		
#include <string.h>
#include "syntactic.tab.h"
#include "symbol-table.c"

void freeAllExp(int count, ...) {
	Exp* exp;
	va_list ap;
	va_start(ap, count);
		exp = va_arg(ap, Exp*);
		if (exp->token != IDENTIFIER)
			freeExp(exp);
	va_end(ap);
}

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
	printf("%s\n", (*exp)->value);
}

void declaration_specifiers3(Exp** exp, Exp* exp1){
	*exp = newExp(exp1->value, EXP_OTHER, exp1->type);
}

void direct_declarator1(Exp** exp, Exp* exp1){
	checkNotDef(exp1);
	*exp = newExp(exp1->value, IDENTIFIER, NULL);
}

void direct_declarator3(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3, Exp* exp4){
	if (exp1->token == IDENTIFIER || exp1->token == DEC_ARRAY || exp1->token == DEC_POINTER){
		int exp_token;
		/* Adaptar */
		if(strcmp(exp3->type->value, "int"))
			yyerror(concat(4, "error: size of array has no integer type '", exp3->type->value, "' = ", exp3->value));
		
		exp_token = exp1->token == DEC_POINTER ? DEC_POINTER_ARRAY : DEC_ARRAY;

		*exp = newExp(concat(4, exp1->value, exp2->value, exp3->value, exp4->value), exp_token, NULL);

		freeAllExp(2, exp1, exp3);
	}
	else{
		yyerror(concat(6, "error: '", exp1->value, exp2->value, exp3->value, exp4->value, "'is not a valid declaration"));
	}
}

void direct_declarator4(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	if (exp1->token == IDENTIFIER)
		*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), DEC_POINTER, NULL);
	else
		yyerror(concat(5, "error: '", exp1->value, exp2->value, exp3->value, "'is not a valid declaration"));
}