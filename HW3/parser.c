
/*

Jeremy Galvan
COP3402-20Fall 0001
HW3 Parser
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "function.h"

#define MAX_IDENT 11
#define MAX_SYMBOL_TABLE_SIZE 1000
#define MAX_CODE 500
#define MAX_CHAC 12
#define MAX_TOKEN 3

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

 token_type toKen;
 int value;
 char ident[MAX_IDENT];
 char symbol[MAX_IDENT];
} tokStruct;

typedef struct
{
	int kind;
	char name[10];
	int val;
	int level;
	int addr;
} symbol;


void errorMessages(int error);
void addTable(int kind, char name[], int val, int addr);
void addCode(int OP, int R, int L, int M);
void get();
void printParser(FILE *output);
void program();
void block();
void constantDecl(char * name, int val);
void varDecl(char *name, int space);
void statement();
void condition();
int rel_op();
void expression();
void term();
void factor();

int ERROR;

tokStruct list[1000];
tokStruct tok;
instruction code[MAX_CODE];
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];

int iterate = 0, iterateToken= 0, rp, sp=1, clvl=1, lvl=1;

FILE* vmInput, *vmOutput;


void get()
{

if(fscanf(vmInput, "%s", tok.ident) != EOF ){

    if((strcmp(tok.ident, "2") == 0))
      fscanf(vmInput, "%s", tok.symbol);

    else if((strcmp(tok.ident, "3") == 0))
      fscanf(vmInput, "%s", tok.symbol);

    else
        tok.symbol[0] = '\0';
  }

  else{
    tok.symbol[0] = '\0';
    tok.symbol[0] = '\0';
  }
}

void addTable(int kind, char name[], int val, int addr){
  symbol_table[iterate].kind = kind;
  strcpy(symbol_table[iterate].name, name);
  symbol_table[iterate].val = val;
  symbol_table[iterate].addr = addr;
  iterate++;
}

void addCode(int OP, int R, int L, int M){

    code[iterateToken].OP = OP;
    code[iterateToken].R = R;
    code[iterateToken].L = L;
    code[iterateToken].M = M;
    iterateToken++;
}

//program function
void program()
{
    get();
    block();

  if(strcmp(tok.ident, "19") != 0)
    errorMessages(9);

   addCode(9,0,0,3);

}

//block function
void block(){
  char tmp[12] = {0};
  int v, jmp, gp = 4, procedureInd;
  jmp = iterateToken;
  sp = 4;
  addCode(7, 0, 0, 0);

  do {
   
      //constant
    if(strcmp(tok.ident, "28") == 0) {
      get();
      do{
        constantDecl(tmp, v);
        while(strcmp(tok.ident, "17") == 0) {
            get();
            constantDecl(tmp, v);
        }
        if(strcmp(tok.ident, "18") != 0) //semicolon
             errorMessages(5);
         get();
       }while(strcmp(tok.ident, "2") == 0); //comma
    }
    
  //variable
  if(strcmp(tok.ident, "29") == 0) {
    get();
     do{
      varDecl(tmp, gp);
      while(strcmp(tok.ident, "17") == 0) { //comma
            get();
            varDecl(tmp, gp);
        }
        if(strcmp(tok.ident, "18") != 0) //semicolon
             errorMessages(5);
         get();
      }while(strcmp(tok.ident, "2") == 0);
    }
    //-----------------------------------------------

  }while((strcmp(tok.ident, "28") == 0) || (strcmp(tok.ident, "29") == 0));



  code[jmp].M = iterateToken;
  addCode(6, 0, 0, gp);
  statement();
  clvl--;
}

//const declaration
void constantDecl(char * name, int val) {
      if(strcmp(tok.ident, "2") != 0)
        errorMessages(4);
      strcpy(name, tok.symbol);
      get();
      if((strcmp(tok.ident, "9") == 0) || (strcmp(tok.ident, "20") == 0)){
          if((strcmp(tok.ident, "20") == 0)) errorMessages(1);
        get();
        if((strcmp(tok.ident, "3") == 0)) {
            val = atoi(tok.symbol);
            addTable(1, name, val, 0);
            get();
        }
    }
}

//var declaration
void varDecl(char *name, int space) {
      if(strcmp(tok.ident, "2") != 0)
        errorMessages(4);
      strcpy(name, tok.symbol);
      addTable(2, name, 0, sp); 
      symbol_table[iterate - 1].level = lvl; //-1
	  space++;
      sp++;
     get();
}

void statement()
{
  int i, ax = 0, temp;

  if(strcmp(tok.ident, "2") == 0)
   {
    for(i = iterate - 1; i >= 0; i--) 
    {
        if(strcmp(tok.symbol, symbol_table[i].name) == 0)
        {
            if(symbol_table[i].kind == 1)
                errorMessages(12);
            else if(symbol_table[i].kind == 2){
                ax = 1;
                temp = i;
            }
        }
    }

    if(ax == 0)
        errorMessages(11);

    get();

    // becomesym
    if(strcmp(tok.ident, "20") != 0 )
        errorMessages(3);

    get();
    expression();
    //add to instruction arrau
    addCode(4, rp - 1, clvl - symbol_table[temp].level, symbol_table[temp].addr - 1);
    rp--;
  }


  //begin
  else if(strcmp(tok.ident, "21") == 0)
  {
    get();
    statement();
    //semicolon
    while(strcmp(tok.ident, "18") == 0)
    {
      get();
      statement();
    }
    get();
  }
  //if
  else if(strcmp(tok.ident, "23") == 0)
  {
    int in1;
    get();
    condition();
    //then
    if(strcmp(tok.ident, "24") != 0)
        errorMessages(16);

    get();

    in1 = iterateToken;

    addCode(8, rp - 1, 0, 0);

    statement();

    get();
    //else
     if(strcmp(tok.ident, "33") == 0)
     {
        int in2 = iterateToken;
        addCode(7, 0, 0, 0);//Jump
        code[in1].M= iterateToken;
        get();
        statement();
        code[in2].M = iterateToken;
        rp--;
    }
    else{
        code[in1].M = iterateToken;
        rp--;
    }
  }
  //while
  else if(strcmp(tok.ident, "25") == 0)
  {
    int fI = iterateToken;
    get();
    condition();
    int fS = iterateToken;
    addCode(8, rp - 1, 0, 0);

    //do
    if(strcmp(tok.ident, "26") != 0)
        errorMessages(18);

   get();
    statement();
    //jumps
    addCode(7, 0, 0, fI);
    code[fS].M = iterateToken;
    rp--;
  }

  //read
  else if(strcmp(tok.ident, "32") == 0)
  {
    get();
    //ident
    if(strcmp(tok.ident, "2") != 0)
         errorMessages(29);

    for(i = iterate - 1; i >= 0; i--)
        if(strcmp(tok.symbol, symbol_table[i].name) == 0){
            ax = 1;
            temp = i;
        }
    if(ax == 0)
        errorMessages(11);
     addCode(10, rp, 0, 2); //Read
     if(symbol_table[temp].kind == 2)
        addCode(4, rp, clvl - symbol_table[temp].level, symbol_table[temp].addr - 1);
     else if(symbol_table[temp].kind == 1)
        errorMessages(12);
     get();
  }

  //write
  else if(strcmp(tok.ident, "31") == 0)
  {
    get();
    //ident
    if(strcmp(tok.ident, "2") != 0)
        errorMessages(29);

    for(i = iterate - 1; i >= 0; i--) 
    {
        if(strcmp(tok.symbol, symbol_table[i].name) == 0)
        {
            if((symbol_table[i].kind == 1) || (symbol_table[i].kind == 2))
            {
                ax = 1;
                temp = i;
            }
        }
    }

      if(ax == 0)
         errorMessages(11);

      //add var to instruction code
     if(symbol_table[temp].kind == 2)
     {
        addCode(3, rp, clvl - symbol_table[temp].level, symbol_table[temp].addr - 1);
        addCode(9, rp, 0, 1); 
     }
     //add var to instruction code
     else if(symbol_table[temp].kind == 1)
     {
        addCode(1, rp, 0, symbol_table[temp].val); 
        addCode(9, rp, 0, 1);
     }
     get();
  }
}



//condition function
void condition() 
{
  //odd
  if(strcmp(tok.ident, "8") == 0)
  {
    
    get();
    expression();
    addCode(17, rp - 1, rp - 1, 0); 
  }
  else{
    
    int temp;
    expression();

    //relation
    temp = rel_op();

    if(temp == 0)
      errorMessages(20);

    get();
    expression();
    addCode(temp, rp - 2, rp - 2, rp - 1); 
    rp--;
  }
}

//relation operators
int rel_op()
{
    // =
  if(strcmp(tok.ident, "9") == 0)
    return 19;
    // <>
  else if(strcmp(tok.ident, "10") == 0)
    return 20;
    // <
  else if(strcmp(tok.ident, "11") == 0)
    return 21;
    // <=
  else if(strcmp(tok.ident, "12") == 0)
    return 22;
    // >
  else if(strcmp(tok.ident, "13") == 0)
    return 23;
    // >=
  else if(strcmp(tok.ident, "14") == 0)
    return 24;
  else
    return 0;
}

void expression()
{

  char temp[3];
//plus or minus
  if((strcmp(tok.ident, "4") == 0) || (strcmp(tok.ident, "5") == 0)) 
  {
    strcpy(temp, tok.ident);
    get();
    term();
    //plus
    if(strcmp(temp, "5") == 0)
        addCode(12, rp - 1, rp - 1, 0);
  }

  else term();

  //plus or minus
  while((strcmp(tok.ident, "4") == 0) || (strcmp(tok.ident, "5") == 0)) {
    strcpy(temp, tok.ident);
    get();
    term();

    if(strcmp(temp, "4") == 0){
        addCode(13, rp - 2, rp - 2, rp - 1);
        rp--;
    }

    else{
        addCode(14, rp - 2, rp - 2, rp - 1 );
        rp--;
    }
  }
}

void term()
{

  char tmp[3];
  factor();
  //multi or slash
  while((strcmp(tok.ident, "6") == 0) || (strcmp(tok.ident, "7") == 0))
  {
    strcpy(tmp, tok.ident);
    get();
    factor();

  //multi
    if(strcmp(tmp,"6") == 0) 
    {
       addCode(15, rp - 2, rp - 2, rp - 1);
       rp--;
    }

  //slash
    else
    {
        addCode(16, rp - 2, rp - 2, rp - 1);
        rp--;
    }
  }
}

void factor()
{
  int i, t = 0, j = 0;

//ident
  if(strcmp(tok.ident, "2") == 0)
  {

    for(i = iterate - 1; i >= 0; i--)
      if(strcmp(tok.symbol, symbol_table[i].name) == 0)
      {
        t = 1;
        j = i;
      }

    if(t == 0)
        errorMessages(11);

    if(symbol_table[j].kind == 2)
        addCode(3, rp, clvl - symbol_table[j].level, symbol_table[j].addr - 1);

    else if(symbol_table[j].kind == 1)
        addCode(1 , rp, 0, symbol_table[j].val);

    rp++;
    get();
  }
//number
  else if(strcmp(tok.ident, "3") == 0) 
  {
    addCode(1, rp, 0, atoi(tok.symbol));
    rp++;
    get();
  }

//left parenthesis
  else if(strcmp(tok.ident, "15") == 0)
  {
    get();
    expression();

  //right parenthesis
    if(strcmp(tok.ident, "16") != 0 )
        errorMessages(22);

    get();

  }

  else
    errorMessages(23);
}




void errorMessages(int error) 
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
     ERROR++;

    exit(1);
}

//prints parsed code in output file
void printParser(FILE *output)
{

  int i;
  for(i = 0; i < iterateToken; i++)
   {
    fprintf(output, "%d %d %d %d\n", code[i].OP, code[i].R, code[i].L, code[i].M);
  }
}


//parser function
void parser(int parse)
{

    vmInput = fopen("lexemelist.txt", "r");
    vmOutput = fopen("parseroutput.txt", "w");

    if(vmInput == NULL)
    {
        printf("Input File could not open\n");
     }


    program();
    printParser(vmOutput);
    fclose(vmInput);
    fclose(vmOutput);

     if(parse) 
     {
        char t;
        vmOutput = fopen("parseroutput.txt", "r");
        fseek(vmOutput, 0, SEEK_SET);
        printf("Parser Code:\n");
        t = fgetc(vmOutput);
        while(t != EOF) 
        {
             printf("%c", t);
             t = fgetc(vmOutput);
        }
        printf("\n\n");
        fclose(vmOutput);
    }

}


