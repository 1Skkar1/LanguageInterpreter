Trabalho 1:

Pretende-se escrever um interpretador para uma linguagem de programação definida por um conjunto de instruções relativamente pequeno.


- Expressões: expressões aritméticas com um único operador. (exemplos: 2 + 3 , 4*x , y/z ).

- Inserção dum valor para uma variável: para que o utilizador possa indicar um valor para a variável identificada pela string referida na instrução. Caso a variável já tenha valor, esse valor será substituído pelo novo.
ler(<variável>);

- Escrever o valor de uma variável:
escrever(<variável>);

- Atribuir o valor duma expressão a uma variável: todas as variáveis que ocorram na expressão deverão ter já um valor definido; o valor da expressão será atribuído à variável que se encontra do lado esquerdo.
<variável> = <expressão>;

- labels: label <string>

- instruções goto: goto label

- comandos condicionais da forma: if <variável> goto <label>

- Terminar a execução do programa:
quit;



Um exemplo de uso desta linguagem:


ler(k);

p= 6 + k;

if p goto L1;

p = p/2;

escrever(p);

goto L2;

label L1;

p = 2*p;

escrever(p);

label L2;

quit;


Sugestões: 
1) use uma lista de instruções como input do interpretador, para lidar com os ciclos. 
2) Mantenha os valores das variáveis numa tabela de hash. 
3) Uma sugestão possível para representar instruções são quádruplos (por exemplo p = 2*p seria representado por (MUL, p, 2, p) e label L1 por (LABEL, L1, EMPTY, EMPTY).). 
4) Esta estrutura pode ser representada em C da forma seguinte:


typedef enum {ATRIB, ADD, SUB, MUL, IF_I, PRINT, READ, GOTO_I, LABEL,...} OpKind;

typedef enum {EMPTY, INT_CONST, STRING} ElemKind;

typedef struct 
                {
                   ElemKind kind;
                   union
                    { 
                      int val;
                      char *name;
                     } contents;
                  } Elem;

typedef struct
                 {
                    OpKind op;
                    Elem first, second, third;
                  } Instr;




Prazo para Entrega: 8 de Abril

Os trabalhos deverão ser feitos em grupos de dois alunos. Deverão ser enviados por email para o respectivo docente das práticas e apresentados (numa apresentação breve) ao docente durante as aulas práticas da semana de 8 a 12 de Abril.
