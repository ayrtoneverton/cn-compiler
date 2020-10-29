#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int idLabel = 0;

char* getLabel(){
	return concat(2, "Z3u6E_9j_", itoa(++idLabel));
}

char* getIf(const char* exp, const char* stm){
	char* pos = getLabel();
	char* result = concat(7, "if(!(",exp,")) goto ",pos,";",stm,pos,":;");
	free(pos);
	return result;
}

char* getIfElse(const char* exp, const char* stm1, const char* stm2){
	char* pre = getLabel();
	char* med = getLabel();
	char* pos = getLabel();
	char* result = concat(14, "if(!(",exp,")) goto ",med,";",stm1," goto ",pos,";",med,":;",stm2,pos,":;");
	freeAll(3, pre, med, pos);
	return result;
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
	char* result = concat(14, exp1, pre,":if(!(",exp2,")) goto ",pos,";",stm,exp3?exp3:"","; goto ",pre,";",pos,":;");
	freeAll(2, pre, pos);
	return result;
}
