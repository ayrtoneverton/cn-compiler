#include <string.h>
#include "comum.c"
#include "symbol-table.c"

void binary_exp(Exp** exp, Exp* exp1, Exp* exp2, Exp* exp3){
	*exp = newExp(NULL, STRING_LITERAL, concat(3, exp1->value, exp2->value, exp3->value));
	freeAllExp(3, exp1, exp2, exp3);
	printf("%s\n", (*exp)->value);
}
