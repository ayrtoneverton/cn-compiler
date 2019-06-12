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

void setType(Exp* exp) {
	switch (exp->token) {
		case CHAR_LITERAL:
			exp->type = get("char");
			break;
		case INTEGER_LITERAL:
			exp->type = get("int");
			break;
		case DECIMAL_LITERAL:
			exp->type = get("float");
			break;
		case STRING_LITERAL:
			exp->type = get("string");
	}
}

void checkDef(Exp* exp) {
	if (exp->token == IDENTIFIER && exp->type == NULL)
		yyerror(concat(3, "error: '", exp->value, "' undeclared"));
}

void unary_exp(Exp** exp, Exp* exp1){
	checkDef(exp1);
	setType(exp1);
	*exp = exp1;
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
