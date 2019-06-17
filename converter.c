#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comum.c"

unsigned int idLabel = 0;

char* getLabel(){
	return concat(2, "Z3u6E_9j_", itoa(++idLabel));
}

char* getWhile(const char* exp, const char* stm){
	char* pre = getLabel();
	char* pos = getLabel();
	char* result = concat(12, pre,":if(!(",exp,")) goto ",pos,";",stm," goto ",pre,";",pos,":;");
	freeAll(2, pre, pos);
	return result;
}

char* getDoWhile(const char* exp, const char* stm){
	char* pre = getLabel();
	char* result = concat(8 ,pre,":;",stm," if(",exp,") goto ",pre,";");
	free(pre);
	return result;
}

char* getFor(const char* exp1, const char* exp2, const char* exp3, const char* stm){
	char* pre = getLabel();
	char* pos = getLabel();
	char* result = concat(14, exp1, pre,":if(!(",exp2,")) goto ",pos,";",stm,exp3,"; goto ",pre,";",pos,":;");
	freeAll(2, pre, pos);
	return result;
}
