#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "trab1.h"

int main(){
	INSTR_LIST il = mkList(NULL,NULL);
	intoList(il);

	//testList(il);
	//printf("%d\n",il->instrk->first->content.val);

	INSTR_LIST init = il;
	while(il->next != NULL){
		il = runL(il->instrk, il, init);
		il = il->next;
	}
	il = runL(il->instrk, il, init);
	
	return 0;
}