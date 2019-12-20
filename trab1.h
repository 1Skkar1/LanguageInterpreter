#define HASH_SIZE 300
#define MULTIPLIER 33 //127

typedef enum { ATRIB, ADD, SUB, MUL, DIV, IF_I, PRINT, READ, GOTO_I, LABEL, END, NULLIUM } OpKind;

typedef enum { EMPTY, INT_CONST, STRING } ElemKind;

typedef struct elem{
	ElemKind kind;
	union 
		{
		int val;
		char *name;
	} content;
} *Elem;

typedef struct instr{
	OpKind op;
	Elem first;
	Elem second;
	Elem third;
} *Instr;

typedef struct list{
  Instr instrk;
  struct list *next; 
} *INSTR_LIST;

typedef struct hashlist{
	char* key;
	int value;
	struct hashlist *next;
} *CELL;

//---------------------------------------------------------------------------------------//

Elem mkInt(int n);
Elem mkVar(char *s);
Elem empty();


Instr mkInstr(OpKind opr, Elem x, Elem y, Elem z);
/*
x = y + 2; -> mkInstr(ADD, mkVar("x"), mkVar("y"), mkInt(2));

x = 2;
y = x + 4;
INSTR_LIST p = mkList( mkInstr( ATRIB, mkVar("x"), mkInt(2), empty()), 
			   mkList( mkInstr( ADD, mkVar("y"), mkVar("x"), mkInt(4), NULL));
*/


INSTR_LIST mkList(Instr head, INSTR_LIST tail);
void addLast(INSTR_LIST p, Instr n);


CELL table[HASH_SIZE];
unsigned int hash(char *s);
CELL lookup(char *s);
void insert(char *s, int value);
void init_table();
void update(char *s, int newValue);
int getValue(char *s);


void getOpr(char *token);
void intoList(INSTR_LIST p);
INSTR_LIST runL(Instr, INSTR_LIST, INSTR_LIST);
void testList(INSTR_LIST);