#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comum.c"

unsigned int idLabel = 0;

char* getLabel(){
	return concat(2, "Z3u6E_9j_", itoa(++idLabel));
}

char* createWhile(const char* exp, const char* stm){
	char* pre = getLabel();
	char* pos = getLabel();
	char* result = concat(12, pre,":if(!(",exp,")) goto ",pos,";",stm,"goto ",pre,";",pos,":;");
	free(pre);
	free(pos);
	return result;
}
