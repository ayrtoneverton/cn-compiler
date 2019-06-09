#include <string.h>
#include "comum.c"
#include "symbol-table.c"

void binary_exp(Exp* exp1, Exp* exp2, Exp* exp3){
	char** list = malloc(sizeof(char*)+3);
	list[0] = exp1->value;
	list[1] = exp2->value;
	list[2] = exp3->value;
	yylval.exp = newExp(NULL, STRING_LITERAL, concat(3, exp1->value,exp2->value,exp3->value));
	free(list);
	printf("%s\n", yylval.exp->value);
	freeExp(exp1);
	freeExp(exp2);
	freeExp(exp3);
}
