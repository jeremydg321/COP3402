

/*

Jeremy Galvan
COP3402-20Fall 0001
HW4 VM
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"


#define MAX_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 500
#define REGISTER_COUNT 8
#define MAX_CHAC 12

typedef struct
{
    int OP;
    int R;
    int L;
    int M;
    char name[MAX_CHAC];

}instruction;





int base(int l, int base);
void fetch();
void execute();
void printCode();
void printsfirstExecution();
void printsExecution(int temp);


int SP = MAX_STACK_HEIGHT;
int BP = (MAX_STACK_HEIGHT - 1);
int PC = 0;
instruction IR = {0};
int RF[REGISTER_COUNT] = {0};
int stack[MAX_STACK_HEIGHT]={0};

instruction inputCode[MAX_CODE_LENGTH];

int halt = 1;

int actRecord;

FILE *input;




/*ISA Commands*/
const char *ISA[] =
{"no","lit", "rtn", "lod",
"sto", "cal", "inc",
"jmp", "jpc", "sys"};

/*base function helps level up some L*/
int base(int l, int base)
{
    int b1;
    b1 = base;
    while (l > 0)
    {
    b1 = stack[b1];
    l--;
    }

return b1;
}

//Fetch function increments input code in IR and increments PC.
void fetch()
{
  IR = inputCode[PC];
  PC++;
}





//Fetch function increments input code in IR and increments PC.
void execute()
{
    //Instruction based in switch function
    switch(IR.OP)
    {
        /*LIT*/
        case 1:
            RF[IR.R] = IR.M;
            break;

        /*RTN*/
        case 2:
            SP=BP + 1;
            BP=stack[SP-2];

            PC=stack[SP-3];
             break;

        /*LOD*/
        case 3:
            RF[IR.R]=stack[base(IR.L,BP)-IR.M];
             break;

        /*STO*/
        case 4:
            stack[base(IR.L,BP)-IR.M]=RF[IR.R];

             break;

        /*CAL*/
        case 5:

            actRecord = SP-1;
            stack[SP-1]=base(IR.L,BP);
            stack[SP-2]=BP;
            stack[SP-3]=PC;
            BP=SP-1;

            PC=IR.M;

            break;

        /*INC*/
        case 6:
            SP=SP-IR.M;

             break;

        /*JMP*/
        case 7:

            PC=IR.M;
             break;

        /*JPC*/
        case 8:
            if(RF[IR.R]==0)
            {

                PC=IR.M;
            }
             break;

        /*SYS or SIO*/



            
            case 9:

            switch(IR.M)
            {
            case 1:
            printf("%d\n", RF[IR.R]);
            break;

          
            case 2:
            printf("Enter a Register: \n");
            scanf("%d", &RF[IR.R]);
            break;

            
            case 3:
            halt = 0;
            break;


            }

        break;

        /*NEG*/
        case 10:


            (RF[IR.R]=-(RF[IR.R]));
             break;

        /*ADD*/
        case 11:
            (RF[IR.R]=RF[IR.L]+RF[IR.M]);
             break;

        /*SUB*/
        case 12:
            (RF[IR.R]=RF[IR.L]-RF[IR.M]);
             break;

        /*MUL*/
        case 13:
            (RF[IR.R]=RF[IR.L]*RF[IR.M]);
             break;

        /*DIV*/
        case 14:
            (RF[IR.R]=RF[IR.L]/RF[IR.M]);
             break;

        /*ODD*/
        case 15:
            (RF[IR.R]= RF[IR.R] % 2);
            break;


        /*MOD*/
        case 16:
            (RF[IR.R] = RF[IR.L] % RF[IR.M]);
             break;

        /*EQL*/
        case 17:
            (RF[IR.R] = RF[IR.L] == RF[IR.M]);
             break;

        /*NEQ*/
        case 18:
            (RF[IR.R] = RF[IR.L] != RF[IR.M]);
             break;

        /*LSS*/
        case 19:
            (RF[IR.R] = RF[IR.L] < RF[IR.M]);
             break;

        /*LEQ*/
        case 20:
            (RF[IR.R] = RF[IR.L] <= RF[IR.M]);
             break;

        /*GTR*/
        case 21:
            (RF[IR.R] = RF[IR.L] > RF[IR.M]);
             break;

        /*GEQ*/
        case 22:
            (RF[IR.R] = RF[IR.L] >= RF[IR.M]);
             break;

        /*OP = 0*/
        default:
            printf("Error: ");
            break;

    }
}

    //Prints code from file
    void printCode(int index)
    {
        int i;
        printf("\nLine\tOP\tR\tL\tM");

        for(i = 0; i < index; i++)
        {
            int OP = inputCode[i].OP, M = inputCode[i].M;
            
            //Prints Instruction symbol based on OP value
            switch(OP){
            case 1:
            strcpy(inputCode[i].name, "LIT");
            printf ("\n%d\t", i);
            printf("LIT\t");
            break;

            case 2:
            strcpy(inputCode[i].name, "RTN");
            printf("\n%d\t", i);
            printf("RTN\t");
            break;

            case 3:
            strcpy(inputCode[i].name, "LOD");
            printf("\n%d\t", i);
            printf("LOD\t");
            break;

            case 4:
            strcpy(inputCode[i].name, "STO");
            printf("\n%d\t", i);
            printf("STO\t");
            break;

            case 5:
            strcpy(inputCode[i].name, "CAL");
            printf("\n%d\t", i);
            printf("CAL\t");
            break;

            case 6:
            strcpy(inputCode[i].name, "INC");
            printf( "\n%d\t", i);
            printf("INC\t");
            break;

            case 7:
            strcpy(inputCode[i].name, "JMP");
            printf("\n%d\t", i);
            printf( "JMP\t");
            break;

            case 8:
            strcpy(inputCode[i].name, "JPC");
            printf("\n%d\t", i);
            printf("JPC\t");
            break;


            case 9:

            switch(M)
            {

            case 1:
            strcpy(inputCode[i].name, "SYS");
            printf( "\n%d\t", i);
            printf( "SYS\t");
            break;

            case 2:
            strcpy(inputCode[i].name, "SYS");
            printf("\n%d\t", i);
            printf("SYS\t");
            break;

            case 3:
            strcpy(inputCode[i].name, "SYS");//or SIO
            printf( "\n%d\t", i);
            printf( "SYS\t");
            break;

            }

            break;
            case 10:
            strcpy(inputCode[i].name, "NEG");
            printf("\n%d\t", i);
            printf("NEG\t");
            break;

            case 11:
            strcpy(inputCode[i].name, "ADD");
            printf("\n%d\t", i);
            printf("ADD\t");
            break;

            case 12:
            strcpy(inputCode[i].name, "SUB");
            printf("\n%d\t", i);
            printf( "SUB\t");
            break;

            case 13:
            strcpy(inputCode[i].name, "MUL");
            printf( "\n%d\t", i);
            printf( "MUL\t");
            break;

            case 14:
            strcpy(inputCode[i].name, "DIV");
            printf("\n%d\t", i);
            printf("DIV\t");
            break;

            case 15:
            strcpy(inputCode[i].name, "ODD");
            printf("\n%d\t", i);
            printf("ODD\t");
            break;

            case 16:
            strcpy(inputCode[i].name, "MOD");
            printf("\n%d\t", i);
            printf("MOD\t");
            break;

            case 17:
            strcpy(inputCode[i].name, "EQL");
            printf("\n%d\t", i);
            printf("EQL\t");
            break;

            case 18:
            strcpy(inputCode[i].name, "NEQ");
            printf("\n%d\t", i);
            printf("NEQ\t");
            break;

            case 19:
            strcpy(inputCode[i].name, "LSS");
            printf("\n%d\t", i);
            printf("LSS\t");
            break;

            case 20:
            strcpy(inputCode[i].name, "LEQ");
            printf("\n%d\t", i);
            printf("LEQ\t");
            break;

            case 21:
            strcpy(inputCode[i].name, "GTR");
            printf( "\n%d\t", i);
            printf( "GTR\t");
            break;

            case 22:
            strcpy(inputCode[i].name, "GEQ");
            printf( "\n%d\t", i);
            printf("GEQ\t");
            break;

            default:
                return;
            }
            printf("%d\t", inputCode[i].R);
            printf("%d\t", inputCode[i].L);
            printf("%d\t", inputCode[i].M);
        }
        return;
    }

void printsfirstExecution() 
{
    int i;

    printf("\n\n\t\t\t\tpc\tbp\tsp\t\n");

    printf( "Initial Values:\t\t\t%d \t%d \t%d\n", PC, BP, SP);

    printf("Register: ");

    for(i = 0; i < REGISTER_COUNT; i++)//based on homework instruction, 8 indexes for stack
    {


        printf("%d ", RF[i]);
    }

    printf("\n");

     /*Iterates initial stacks*/
    printf("Stack: ");

    for(i = 0; i < 40; i++)
    {
       printf("%d ", stack[i]);
    }



}

void printsExecution(int t) 
{
     int i;

    printf("\n\n\t\t\t\tpc\tbp\tsp\n");

    printf("%d %s %d %d %d\t\t\t", t, IR.name , IR.R, IR.L, IR.M);
    printf("%d \t%d \t%d\n", PC, BP, SP);


    printf( "Register: ");
    for(i = 0; i < REGISTER_COUNT; i++)
    {

        printf( "%d ", RF[i]);
    }


    printf("\n");
    printf("Stack: ");
    for(i = MAX_STACK_HEIGHT-1; i>=SP; i--)

    {

        if(actRecord==i)
        {
            printf(" |");
        }


         printf("%d ", stack[i]);
    }


   printf("\n\n");
}

//VM
void pm()
{
     input =   fopen("parseoutput.txt", "r");
     

     if(input == NULL)
     {
        printf("input file does not exist\n");
      }

    int index = 0;

    //scans each line of parsed file
    while(!feof(input)) 
    {
        fscanf(input, "%d", &inputCode[index].OP);
        fscanf(input, "%d", &inputCode[index].R);
        fscanf(input, "%d", &inputCode[index].L);
        fscanf(input, "%d", &inputCode[index].M);
        index++;
    }
    printf(" Code: \n\n");
   
    printCode(index);

     printf("\n\nVM Output: \n\n");
    printsfirstExecution();
    index = 0;

     
    //Halt will become zero when OP is 9 and M is 3
    while(halt == 1)
    {
        int t = PC;

        fetch();
        execute();
        printsExecution(t);
    
        

    }

    //Close files.
    fclose(input);
   



}

