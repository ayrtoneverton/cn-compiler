#include "comum.c"
#include "symbol-table.c"

void s_identifier(){
	yylval.item = malloc(sizeof(Item));
	yylval.item->value = yytext;
	printf("#%s#", yylval.item->value);
}
