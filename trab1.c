#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "trab1.h"

Elem mkInt(int n){
	Elem k = (Elem) malloc(sizeof(struct elem));
	k->kind = INT_CONST;
	k->content.val = n;
	return k;
}

Elem mkVar(char* s){
	Elem k = (Elem) malloc(sizeof(struct elem));
	k->content.name = (char*) malloc(sizeof(char)*15);
	k->kind = STRING;
	strcpy(k->content.name,s);
	return k;
}

Elem empty(){
	Elem k = (Elem) malloc(sizeof(struct elem));
	k->kind = EMPTY;
	return k;
}

Instr mkInstr(OpKind opr, Elem x, Elem y, Elem z){
	Instr a = (Instr) malloc(sizeof(struct instr));
	a->op = opr;
	a->first = x;
	a->second = y;
	a->third = z;
	return a;
}

INSTR_LIST mkList(Instr head, INSTR_LIST tail){
	INSTR_LIST l = (INSTR_LIST) malloc(sizeof(struct list));
	if(head == NULL && tail == NULL){
		l->instrk = NULL;
		l->next = NULL;
		return l;
	}
	else{
		l->instrk = head;
		l->next = tail;
		return l;
	}
}


void addLast(INSTR_LIST p, Instr n){
	if(p->instrk == NULL){
		p->instrk = n;
		p->next = NULL;
		return;
	}
    while(p->next != NULL){
        p = p->next;
    }
    p->next = mkList(NULL,NULL);
    p->next->instrk = n;
}

unsigned int hash(char *s){
	unsigned int h;
	unsigned char *p;
	h=0;

	for(p = (unsigned char *)s; *p != '\0'; p++)
		h =  MULTIPLIER *h + *p;

	return h % HASH_SIZE;
}

CELL lookup(char *s){
	int index;
	CELL p;

	index = hash(s);
	for(p = table[index]; p != NULL; p = p->next){
		if(strcmp(s,p->key) == 0)
			return p;
	}
	return NULL;
}

void testList(INSTR_LIST p) {
	
	INSTR_LIST temp = p;
	int i = 0;
	
	while (temp != NULL) {
		printf("Indice_No:%d\tEndereco_No:%p\tAponta_para:%p\tValor:%d\n",i,temp,temp->next,temp->instrk->op);
		i++;
		temp = temp->next;
	}
	return;
}

void insert(char *s, int value){
	int index;
	CELL p;
	p = (CELL)malloc(sizeof(struct hashlist));

	index = hash(s);
	p->key = s;
	p->value = value;

	p->next = table[index];
	table[index] = p; 		
}

void init_table(){
	for(int i=0; i<HASH_SIZE; i++){
		table[i] = NULL;
	}
}

void update(char *s, int newValue){
	CELL p = lookup(s);
	if(p == NULL)
		insert(s,newValue);
	else
		p->value = newValue;
	//printf("new val %d\n", newValue); 
}

int getValue(char *s){
	CELL p = lookup(s);
	if(p == NULL)
		printf("ERROR!\n");
	return p->value; 
}



void intoList(INSTR_LIST p){
	int i=0;
	int num1, num3;
	char line[1024];
	char* token[5];
	for ( i = 0; i < 5 ; i++)
		token[i] = malloc(sizeof(char)*50);
	char *str = malloc(sizeof(char)*50);
	char *tok = malloc(sizeof(char)*50);

	while( (str = fgets(line,50,stdin)) != "quit ;" ){
		i = 0;
		tok = strtok(str, " ");
		while( tok != NULL){

			if(!strcmp(tok,"ler"))
				strcpy(token[i],"READ");
					
			else if(!strcmp(tok,"escrever"))
				strcpy(token[i],"PRINT");

			else if(!strcmp(tok,"if"))
				strcpy(token[i],"IF_I");

			else if(!strcmp(tok,"goto"))
				strcpy(token[i],"GOTO_I");

			else if(!strcmp(tok,"label"))
				strcpy(token[i],"LABEL");
					
			else if(!strcmp(tok,"quit"))
				strcpy(token[i],"END");

			else if(!strcmp(tok,"+"))
				strcpy(token[i],"ADD");	

			else if(!strcmp(tok,"-"))
				strcpy(token[i],"SUB");	

			else if(!strcmp(tok,"*"))
				strcpy(token[i],"MUL");

			else if(!strcmp(tok,"/"))
				strcpy(token[i],"DIV");	

			else if(!strcmp(tok,"=") || !strcmp(tok,";") || !strcmp(tok,"(") || !strcmp(tok,")"))
				i--;

			else{
				//printf("%d %s\n",i, tok);
				strcpy(token[i],tok);	
						
			}
			//printf("%s\n",tok);
			i++;	
			tok = strtok(NULL," ");
		}

		if(!strcmp(token[0],"READ")){
			addLast(p, mkInstr(READ, mkVar(token[1]), empty(), empty()));
		}

		else if(!strcmp(token[0],"PRINT")){
			addLast(p, mkInstr(PRINT, mkVar(token[1]), empty(), empty()));
		}

		else if(!strcmp(token[0],"IF_I")){
			addLast(p, mkInstr(IF_I, mkVar(token[1]), empty(), empty()));
			addLast(p, mkInstr(GOTO_I, mkVar(token[3]), empty(), empty()));
			//printf("banana %s\n", token[3]);
			//addLast(p, mkInstr(token[0], mkVar(token[1]), mkInstr(token[2], mkInstr(LABEL, mkVar(token[3]), empty(), empty()), empty(), empty())));
		}

		else if(!strcmp(token[0],"GOTO_I")){
			addLast(p, mkInstr(GOTO_I, mkVar(token[1]), empty(), empty()));
		}

		else if(!strcmp(token[0],"LABEL")){
			addLast(p, mkInstr(LABEL, mkVar(token[1]), empty(), empty()));
		}

		else if(!strcmp(token[0],"END")){
			addLast(p, mkInstr(END, empty(), empty(), empty()));
			return;
		}

		else if(!strcmp(token[2],"ADD")){
			if((isdigit(token[1][0])) && (!isdigit(token[3][0]))){ //1 is int 3 is var
				num1 = atoi(token[1]);
				addLast(p, mkInstr(ADD, mkVar(token[0]), mkInt(num1), mkVar(token[3])));
			}
			else if((isdigit(token[1][0])) && (isdigit(token[3][0]))){ //1 is int 3 is int
				num1 = atoi(token[1]);
				num3 = atoi(token[3]);
				addLast(p, mkInstr(ADD, mkVar(token[0]), mkInt(num1), mkInt(num3)));
			}
			else if((!isdigit(token[1][0])) && (!isdigit(token[3][0]))){ //1 is var 3 var
				addLast(p, mkInstr(ADD, mkVar(token[0]), mkVar(token[1]), mkVar(token[3])));
			}
			else{ //1 is var 3 is int
				num3 = atoi(token[3]);
				addLast(p, mkInstr(ADD, mkVar(token[0]), mkVar(token[1]), mkInt(num3)));
			}
		}

		else if(!strcmp(token[2],"SUB")){
			if((isdigit(token[1][0])) && (!isdigit(token[3][0]))){ //1 is int 3 is var
				num1 = atoi(token[1]);
				addLast(p, mkInstr(SUB, mkVar(token[0]), mkInt(num1), mkVar(token[3])));
			}
			else if((isdigit(token[1][0])) && (isdigit(token[3][0]))){ //1 is int 3 is int
				num1 = atoi(token[1]);
				num3 = atoi(token[3]);
				addLast(p, mkInstr(SUB, mkVar(token[0]), mkInt(num1), mkInt(num3)));
			}
			else if((!isdigit(token[1][0])) && (!isdigit(token[3][0]))){ //1 is var 3 var
				addLast(p, mkInstr(SUB, mkVar(token[0]), mkVar(token[1]), mkVar(token[3])));
			}
			else{ //1 is var 3 is int
				num3 = atoi(token[3]);
				addLast(p, mkInstr(SUB, mkVar(token[0]), mkVar(token[1]), mkInt(num3)));
			}
		}

		else if(!strcmp(token[2],"MUL")){
			if((isdigit(token[1][0])) && (!isdigit(token[3][0]))){ //1 is int 3 is var
				//printf("1kkk\n");
				num1 = atoi(token[1]);
				addLast(p, mkInstr(MUL, mkVar(token[0]), mkInt(num1), mkVar(token[3])));
			}
			else if((isdigit(token[1][0])) && (isdigit(token[3][0]))){ //1 is int 3 is int
				//printf("2kkk\n");
				num1 = atoi(token[1]);
				num3 = atoi(token[3]);
				addLast(p, mkInstr(MUL, mkVar(token[0]), mkInt(num1), mkInt(num3)));
			}
			else if((!isdigit(token[1][0])) && (!isdigit(token[3][0]))){ //1 is var 3 var
				//printf("3kkk\n");
				addLast(p, mkInstr(MUL, mkVar(token[0]), mkVar(token[1]), mkVar(token[3])));
			}
			else{ //1 is var 3 is int
				//printf("4kkk\n");
				num3 = atoi(token[3]);
				addLast(p, mkInstr(MUL, mkVar(token[0]), mkVar(token[1]), mkInt(num3)));
			}
		}

		else if(!strcmp(token[2],"DIV")){
			if((isdigit(token[1][0])) && (!isdigit(token[3][0]))){ //1 is int 3 is var
				//printf("1kkk\n");
				num1 = atoi(token[1]);
				addLast(p, mkInstr(DIV, mkVar(token[0]), mkInt(num1), mkVar(token[3])));
			}
			else if((isdigit(token[1][0])) && (isdigit(token[3][0]))){ //1 is int 3 is int
				//printf("2kkk\n");
				num1 = atoi(token[1]);
				num3 = atoi(token[3]);
				addLast(p, mkInstr(DIV, mkVar(token[0]), mkInt(num1), mkInt(num3)));
			}
			else if((!isdigit(token[1][0])) && (!isdigit(token[3][0]))){ //1 is var 3 var
				//printf("3kkk\n");
				addLast(p, mkInstr(DIV, mkVar(token[0]), mkVar(token[1]), mkVar(token[3])));
			}
			else{ //1 is var 3 is int
				//printf("4kkk\n");
				num3 = atoi(token[3]);
				addLast(p, mkInstr(DIV, mkVar(token[0]), mkVar(token[1]), mkInt(num3)));
			}
		}

		else{ //ATRIB
			if(isdigit(token[1][0])){
				num1 = atoi(token[1]);
				addLast(p, mkInstr(ATRIB, mkVar(token[0]), mkInt(num1), empty()));
			}
			else
				addLast(p, mkInstr(ATRIB, mkVar(token[0]), mkVar(token[1]), empty()));
		}
	}
}

	

INSTR_LIST runL(Instr a, INSTR_LIST p, INSTR_LIST init){
	INSTR_LIST temp = init;
	int readVar, strVal1=0, strVal2=0, sumTotal; 

	switch(a->op){
		case ATRIB: // like x = 2
			//printf("I'm in ATRIB\n");
			update(a->first->content.name, a->second->content.val);
			break;

		case ADD: // like y = x + 4
			//printf("I'm in ADD\n");
			if(a->second->kind == STRING)
				strVal1 = getValue(a->second->content.name);
			else
				strVal1 = a->second->content.val;

			if(a->third->kind == STRING)
				strVal2 = getValue(a->third->content.name);
			else
				strVal2 = a->third->content.val;
			sumTotal = strVal1 + strVal2;
			update(a->first->content.name, sumTotal);
			break;

		case SUB: // like y = x - 4
			//int strVal1, strVal2; 
			//printf("I'm in SUB\n");
			if(a->second->kind == STRING)
				strVal1 = getValue(a->second->content.name);
			else
				strVal1 = a->second->content.val;

			if(a->third->kind == STRING)
				strVal2 = getValue(a->third->content.name);
			else
				strVal2 = a->third->content.val;
			sumTotal = strVal1 - strVal2;
			update(a->first->content.name, sumTotal);
			break;

		case MUL: // like y = x * 4
			//int strVal1, strVal2; 
			//printf("I'm in MUL\n");
			if(a->second->kind == STRING)
				strVal1 = getValue(a->second->content.name);
			else
				strVal1 = a->second->content.val;

			if(a->third->kind == STRING)
				strVal2 = getValue(a->third->content.name);
			else
				strVal2 = a->third->content.val;
			sumTotal = strVal1 * strVal2;
			update(a->first->content.name, sumTotal);
			break;

		case DIV: // like y = x / 4
			//int strVal1, strVal2; 
			//printf("I'm in DIV\n");
			if(a->second->kind == STRING)
				strVal1 = getValue(a->second->content.name);
			else
				strVal1 = a->second->content.val;

			if(a->third->kind == STRING)
				strVal2 = getValue(a->third->content.name);
			else
				strVal2 = a->third->content.val;

			if(strVal1 == 0 || strVal2 == 0)
				update(a->first->content.name, 0);
			else{
				sumTotal = strVal1 / strVal2;
				update(a->first->content.name, sumTotal);
			}
			break;
			
		case IF_I: // like if p goto l1(if p==0 false else true)
			//printf("I'm in IF\n");
			if(getValue(a->first->content.name) == 0)
				p = p->next;
			break;

		case PRINT: // printf
			//printf("I'm in PRINT\n");
			printf("%s = %d\n", a->first->content.name, getValue(a->first->content.name));
			break;

		case READ: // scanf
			//printf("I'm in READ\n");
			fflush(stdin);
			printf("---------------------\n");
			printf("Insert value for %s: \n", a->first->content.name);
			scanf("%d", &readVar);

			//printf("%s = %d\n", a->first->content.name, readVar);
			//printf("%d\n", readVar);

			update(a->first->content.name,readVar);
			printf("%d\n", getValue(a->first->content.name));
			printf("---------------------\n");
			break;

		case GOTO_I: //like goto L2
			//printf("I'm in GOTO\n");
			temp = init;
			while(temp != NULL){
				//printf("yo\n" );
				if((temp->instrk->op == LABEL) && (!strcmp(temp->instrk->first->content.name,a->first->content.name))){
					p = temp;
					return temp;
				}
				temp = temp->next;	
			}
			//p = temp;
			break;

		case LABEL: //like label L2
			//printf("I'm in LABEL\n");
			break;

		default:
			printf("Process finished successfully\n");
			break;
	}
	return p;
}


/*
Elem var1 = mkVar(token);
					token = strtok(NULL," ");
					if(strcmp(token,"+")
						strcpy(token,"ADD");
					else if(strcmp(token,"-")
						strcpy(token,"SUB");
					else if(strcmp(token,"*")
						strcpy(token,"MUL");
*/