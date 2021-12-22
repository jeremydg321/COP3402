/*

Jeremy Galvan
COP3402-20Fall 0001
HW4 Parser
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "function.h"



#define MAX_CODE 500
#define MAX_CHAC 11
#define MAX_DIGITS 5



typedef struct
{
    int OP;
    int R;
    int L;
    int M;
    char name[MAX_CHAC];

}instruction;

typedef enum
{

nulsym =1, identsym =2, numbersym =3, plussym = 4,minussym = 5,
multsym = 6, slashsym = 7, oddsym = 8, eqsym = 9, neqsym = 10, lessym = 11, leqsym = 12,
gtrsym = 13, geqsym = 14, lparentsym = 15, rparentsym = 16, commasym = 17,
semicolonsym = 18, periodsym = 19, becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23,
thensym = 24,whilesym = 25, dosym = 26, callsym = 27, constsym =28, varsym =29, procsym = 30,
writesym = 31, readsym = 32, elsesym = 33
} token_type;


typedef struct
{


 int value;
 char ident[MAX_CHAC];
 
} tokStruct;

typedef struct
{
	int kind;
	char name[10];
  char ident[10];
	int val;
	int level;
	int addr;
  int mark;
} symbol;

//declaration of functions
void error(int error);
void emit(int OP, int R, int L, int M);
void emitSym(char *ident, int val, int kind, int addr);
void get();
void printParser(FILE *output);
void program();
void block();
void constantDecl();
int varDecl();
void procDecl(int index);
void statement();
void condition();
int rel();
int rel_op();
void expression();
void term();
void factor();
void readLexFile();
symbol* findIdent(char* ident);

//declarations of tables
tokStruct *token[1000];
tokStruct *tok;
symbol *symTable[1000];
instruction *cd[MAX_CODE];
char id[3][4];


//initial global variables for functions
int  level = 0, codei=0,  tokeni = 0, tokenc=0, symboli=0; 


//parse function
void parse()
{

    readLexFile();

    program();

    printf("No errors, program is syntactically correct\n\n");

    FILE *output = fopen("parseoutput.txt", "w");

    printParser(output);
}
void program()
{
  //first tokem
  get();
  
    //looks through const, var, and proc
    block();

    //nor period
    if(tok->value!=periodsym)
    {
        
        error(9);
    }

//this last line would end program
emit(9,0,0,3);
}

void block()
{
    //creates spa
    int s=4;
    int i=codei;

//first jump
emit(7,0,0,0);

if(tok->value ==constsym)
{
constantDecl();
}
if(tok->value ==varsym)
{
s+=varDecl();
}

while(tok->value ==procsym)
{
procDecl(i);
}

cd[i]->M= codei;
emit(6,0,0,s);
statement();

}


//analyzes constant
void constantDecl(){

do{
get();



tokStruct* t = tok;

    if(tok->value !=identsym)
    {
    error(4);
    }

    get();
   
    
    if(tok->value !=eqsym)
    {
    error(3);

    }
    get();
    
    if(tok->value !=numbersym)
    {
    error(2);
    }

    emitSym(t->ident, atoi(tok->ident),1, 0);
    get();
  
}while(tok->value==commasym);

if(tok->value!=semicolonsym)
{
    error(10);
}

get();


}



//analyzes varialbe
int varDecl()
{

    int numVars=0;

    do
    {
        get();
        
      

        if(tok->value!=identsym)
        {
            error(4);
        }


        emitSym(tok->ident,-1,2,numVars+4);
        get();
  
        
        numVars+=1;
    }while(tok->value==commasym);

    if(tok->value!= semicolonsym)
    {
        error(10);
    }

    get();

 
    return numVars;

}


//analyzes procedure
void procDecl(int index)
{
    get();


    if(tok->value != identsym)
    {
        error(4);
    }

    strcpy(id[level], tok->ident);

    emitSym(tok->ident,-1,3,index+1);

    symTable[symboli-1]->level = level;

    ++level;

    get();


    if(tok->value!=semicolonsym)
    {
        error(6);
    }

    get();
  

    block();

    if(tok->value!=semicolonsym)
    {
        error(10);
    }

    emit(2,0,0,0);

    get();

    --level;


}

//identifies statement
void statement()
 {
  
  if (tok->value == identsym) {
   
    symbol* table = findIdent(tok->ident);
    if (!table) {
      error(11);
    }


    if (table->kind != 2) {
      error(12);
    }

    get();
   
    if (tok->value != becomessym) {

  
      if (tok->value == eqsym) {
        error(1);
      }

      error(19);
    }

 
    get();
  
    expression();


    emit(4,0,  level - table->level, table->addr);
  }


  else if (tok->value == callsym) {


    get();
   
    symbol* table = findIdent(tok->ident);
    if (!table) {
      error(11);
    }


    if (tok->value != identsym) {
      error(14);
    }

    if (table->kind != 3) {
      error(15);
    }


    emit(5,0,  level - table->level, table->addr);

    get();

  }
  else if (tok->value == beginsym) {
    get();
   
    statement();

   
    if (tok->value != semicolonsym && tok->value!= endsym) {
      error(10);
    }


    while (tok->value == semicolonsym) {
      get();
    
      statement();


      if (tok->value != semicolonsym && tok->value != endsym) {
        error(10);
      }
    }


    if (tok->value != endsym) {
      error(8);
    }

    get();
 
  }
  else if (tok->value == ifsym) {

  
    condition();


    if (tok->value != thensym) {
      error(16);
    }


    int temp = codei;

    emit(8, 0, 0,0);

    get();
 
    statement();

  
    int temp2 = codei;

  
    emit(7, 0, 0,0);

    cd[temp]->M = codei;


    if (tok->value == elsesym) {
      get();
   
      statement();
    }



    cd[temp2]->M = codei;

  }

  else if (tok->value == whilesym) {
   
    int temp3 = codei;

   
    condition();

    int temp4 = codei;


    emit(8, 0,0, 0);


    if (tok->value != dosym) {
      error(18);
    }

  
    get();
 
    statement();


    emit(7,0, 0, temp3);

    cd[temp4]->M = codei;
  }

  else if (tok->value == readsym) {
    get();
  

    symbol *table = findIdent(tok->ident);
    if (!table) {
     
      error(11);
    }

   
    emit(9,0, 0, 2);

  
    if (table->kind != 2) {
      error(12);
    }

  
    emit(4,0, level - table->level, table->addr);

    get();
  
  }

  else if (tok->value == writesym) {
    get();
  
   
    if (tok->value != identsym) {
      error(26);
    }

 
    symbol *table = findIdent(tok->ident);
    if (!table) {
   
      error(11);
    }

    if (table->kind == 2) {
      emit(3, 0, level - table->level, table->addr);
    }

 
    if (table->kind == 1) {
      emit(1, 0, 0, table->val);
    }

   
    emit(9,0, 0, 1);

    get();
  
  }
}


//identifies odd
void condition()
{

      if (tok->value == oddsym) {
    
    emit(2,0, 0, 6);

    get();
   
    expression();
  }

 
  else {
    get();
  
    expression();

    int temp = rel_op();

   
    if (!rel()) {
      error(20);
    }

    get();
  
    expression();

   
    emit(2,0, 0, temp);

}
}


//arithmetic expressions
void expression() {
  int OP;
  
  if (tok->value == plussym || tok->value == minussym) {
    OP = tok->value;

    get();
    
    term();


    if (OP == minussym) {
      emit(2,0, 0,1);
    }
  }

  
  else {
    term();
  }

  while (tok->value == plussym || tok->value == minussym) {
    OP = tok->value;

    get();
   
    term();

    if (OP == plussym) {
      emit(6,0, 0, 2);
    }
    else {
      emit(6,0, 0, 3);
    }
  }
}


//multiply or divide
void term() {
  int OP;

 
  factor();

  
  while (tok->value == multsym || tok->value == slashsym) {
    OP = tok->value;

    get();
 
    factor();

    if (OP == multsym) {
      emit(6, 0,0, 4);
    }
    else {
      emit(6, 0,0, 5);
    }
  }
}


//parenthesis
void factor() {

  if (tok->value == identsym) {
    symbol *table = findIdent(tok->ident);
    if (!table) {
      error(11);
    }
    get();
 

    
    if (table->kind == 2) {
      emit(3, 0, level - table->level, table->addr);
    }

 
    else if (table->kind == 1) {
      emit(1,0, 0, table->val);
    }
  }


  else if (tok->value == numbersym) {
    
    emit(1,0, 0, atoi(tok->ident));

    get();

  }

  
  else if (tok->value == lparentsym) {
    get();

    expression();

   
    if (tok->value != rparentsym) {
      error(22);
    }

    get();

  }


  else {
    error(24);
  }
}


//finds identifier in default symbol table
symbol* findIdent(char* ident){
int count;

for(count= symboli-1;count>=0;count--)
{
    if(symTable[count]->level<=level)
    {
        if(strcmp(ident, symTable[count]->name)==0)
        {
            return symTable[count];
        }
    }
}

return NULL;

}


//reads output lexical file
void readLexFile()
{
FILE *input = fopen("lexemeList.txt", "r");

while(!feof(input))
{
    tokStruct *t = (tokStruct*)malloc(sizeof(tokStruct));

    strcpy(t->ident, "");

    fscanf(input, "%d ", &t->value);

   

      

    if( t->value==identsym || t->value==numbersym )
    {
       fscanf(input, "%[^ \n]s",t->ident);

      
    }

token[tokenc++] = t;
}

printf("\n\n");

printf("\n\n");
fclose(input);
}


//gets token
void get()
{
    tok=token[tokeni++];

    if(!tok)
    {
        tok=(tokStruct*)(malloc(sizeof(tokStruct)));
        strcpy(tok->ident,"");
        tok->value = nulsym;

    }

 
}

//inserts data to instruction table
void emit(int OP, int R, int L, int M)
{
    instruction *temp= (instruction*)(malloc(sizeof(instruction)));
    temp->OP=OP;
    temp->L=L;
    temp->M=M;
    temp->R=R;
    cd[codei++] = temp;

}


//inserts symbols in symbol table
void emitSym(char *ident, int val, int kind, int addr)
{
    if(kind!=2&& findIdent(ident))
    {
        return;
    }


    symbol * table =  (symbol*)(malloc(sizeof(symbol)));
    strcpy(table->name, ident);
    strcpy(table->ident, "");
    table->val=val;
    table->kind=kind;
    table->level=level;
    table->addr=addr;

    if(kind!=procsym)
    {
        strcpy(table->ident, id[level]);
    }
    symTable[symboli++] = table;

}


//checks whether relational operator is there
int rel()
{
   return (tok->value >= eqsym && tok->value <= geqsym); 
}

//added for emit
int rel_op() {
  return tok->value - 1; 
}


//prints errors
void error(int error) 
{


    printf("Error %d: ", error);

    switch (error) 
    {
        case 1:
            
            printf("Use = instead of :=.\n");
            break;
        case 2:
           
            printf("= must be followed by a number.\n");
            break;
        case 3:
            
            printf("Identifier must be followed by =.\n");
            break;
        case 4:
           
            printf("const, var, procedure must be followed by identifier.\n");
            break;
        case 5:
           
            printf("Semicolon or comma missing.\n");
            break;
        case 6:
           
            printf("Incorrect symbol after procedure declaration.\n");
            break;
        case 7:
            
            printf("Statement expected\n");
            break;
        case 8:
            
            printf("Incorrect symbol after statement part in block.\n");
            break;
        case 9:
            
            printf("Period expected.\n");
            break;
        case 10:
           
            printf("Semicolon between statements missing.\n");
            break;
        case 11:
           
            printf("Undeclared identifier.\n");
            break;
        case 12:
            
            printf("Assignment to constant or procedure is not allowed.\n");
            break;
        case 13:
           
            printf("Assignment operator expected.\n");
            break;
        case 14:
            
            printf("call must be followed by an identifier\n");
            break;
        case 15:
            
            printf("Call of a constant or variable is meaningless.\n");
            break;
        case 16:
            
            printf("then expected\n");
            break;
        case 17:
            
            printf("Semicolon or } expected\n");
            break;
        case 18:
            
            printf("do expected\n");
            break;
        case 19:
          
            printf("Incorrect symbol following statement.\n");
            break;
        case 20:
         
            printf("Relational operator expected.\n");
            break;
        case 21:
            
            printf("Expression must not contain a procedure identifier.\n");
            break;
        case 22:
            
            printf("Right parenthesis missing.\n");
            break;
        case 23:
           
            printf("The preceding factor cannot begin with this symbol.\n");
            break;
        case 24:
           
            printf("An expression cannot begin with this symbol.\n");
            break;
        case 25:
            
            printf("This number is too large.\n");
            break;
        default:
            break;
    }
     

    exit(1);
}


//prints output
void printParser(FILE *output)
{
    output= fopen("parseoutput.txt", "w");

  printf("Parser Output: \n\n");
    for(int i=0;i<codei;i++)
    {
        instruction *temp = cd[i];
        fprintf(output, "%d %d %d %d\n",temp->OP,temp->R,temp->L,temp->M);
        
        printf( "%d %d %d %d\n",temp->OP,temp->R,temp->L,temp->M);
    
    }
  printf("\n");
  fclose(output);
}