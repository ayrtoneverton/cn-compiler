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
	if (exp->token == IDENTIFIER)
		yyerror("ERROR\n");
}

void primary_exp2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	checkDef(exp2);
	*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), exp2->token);
	freeAllExp(3, exp1, exp2, exp3);
}

void binary_exp2(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	*exp = newExp(concat(3, exp1->value, exp2->value, exp3->value), exp2->token);
	freeAllExp(3, exp1, exp2, exp3);
	printf("%s\n", (*exp)->value);
}
