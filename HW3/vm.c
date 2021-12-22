


/*

Jeremy Galvan
COP3402-20Fall 0001
HW1 VM
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

FILE *input, *output;




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
            fprintf(output, "%d\n", RF[IR.R]);
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
            fprintf(output,"Error: ");
            break;

    }
}

    //Prints code from file
    void printCode(int index)
    {
        int i;
        fprintf(output, "\nLine\tOP\tR\tL\tM");

        for(i = 0; i < index; i++)
        {
            int OP = inputCode[i].OP, M = inputCode[i].M;
            
            //Prints Instruction symbol based on OP value
            switch(OP){
            case 1:
            strcpy(inputCode[i].name, "LIT");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "LIT\t");
            break;

            case 2:
            strcpy(inputCode[i].name, "RTN");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "RTN\t");
            break;

            case 3:
            strcpy(inputCode[i].name, "LOD");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "LOD\t");
            break;

            case 4:
            strcpy(inputCode[i].name, "STO");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "STO\t");
            break;

            case 5:
            strcpy(inputCode[i].name, "CAL");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "CAL\t");
            break;

            case 6:
            strcpy(inputCode[i].name, "INC");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "INC\t");
            break;

            case 7:
            strcpy(inputCode[i].name, "JMP");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "JMP\t");
            break;

            case 8:
            strcpy(inputCode[i].name, "JPC");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "JPC\t");
            break;


            case 9:

            switch(M)
            {

            case 1:
            strcpy(inputCode[i].name, "SYS");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "SYS\t");
            break;

            case 2:
            strcpy(inputCode[i].name, "SYS");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "SYS\t");
            break;

            case 3:
            strcpy(inputCode[i].name, "SYS");//or SIO
            fprintf(output, "\n%d\t", i);
            fprintf(output, "SYS\t");
            break;

            }

            break;
            case 10:
            strcpy(inputCode[i].name, "NEG");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "NEG\t");
            break;

            case 11:
            strcpy(inputCode[i].name, "ADD");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "ADD\t");
            break;

            case 12:
            strcpy(inputCode[i].name, "SUB");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "SUB\t");
            break;

            case 13:
            strcpy(inputCode[i].name, "MUL");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "MUL\t");
            break;

            case 14:
            strcpy(inputCode[i].name, "DIV");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "DIV\t");
            break;

            case 15:
            strcpy(inputCode[i].name, "ODD");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "ODD\t");
            break;

            case 16:
            strcpy(inputCode[i].name, "MOD");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "MOD\t");
            break;

            case 17:
            strcpy(inputCode[i].name, "EQL");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "EQL\t");
            break;

            case 18:
            strcpy(inputCode[i].name, "NEQ");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "NEQ\t");
            break;

            case 19:
            strcpy(inputCode[i].name, "LSS");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "LSS\t");
            break;

            case 20:
            strcpy(inputCode[i].name, "LEQ");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "LEQ\t");
            break;

            case 21:
            strcpy(inputCode[i].name, "GTR");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "GTR\t");
            break;

            case 22:
            strcpy(inputCode[i].name, "GEQ");
            fprintf(output, "\n%d\t", i);
            fprintf(output, "GEQ\t");
            break;

            default:
                return;
            }
            fprintf(output, "%d\t", inputCode[i].R);
            fprintf(output, "%d\t", inputCode[i].L);
            fprintf(output, "%d\t", inputCode[i].M);
        }
        return;
    }

void printsfirstExecution() 
{
    int i;

    fprintf(output,"\n\n\t\t\t\tpc\tbp\tsp\t\n");

    fprintf(output, "Initial Values:\t\t\t%d \t%d \t%d\n", PC, BP, SP);

    fprintf(output, "Register: ");

    for(i = 0; i < REGISTER_COUNT; i++)//based on homework instruction, 8 indexes for stack
    {


        fprintf(output, "%d ", RF[i]);
    }

    fprintf(output,"\n");

     /*Iterates initial stacks*/
    fprintf(output,"Stack: ");

    for(i = 0; i < 40; i++)
    {
       fprintf(output, "%d ", stack[i]);
    }



}

void printsExecution(int t) 
{
     int i;

    fprintf(output, "\n\n\t\t\t\tpc\tbp\tsp\n");

    fprintf(output, "%d %s %d %d %d\t\t\t", t, IR.name , IR.R, IR.L, IR.M);
    fprintf(output, "%d \t%d \t%d\n", PC, BP, SP);


    fprintf(output, "Register: ");
    for(i = 0; i < REGISTER_COUNT; i++)
    {

        fprintf(output, "%d ", RF[i]);
    }


    fprintf(output,"\n");
    fprintf(output,"Stack: ");
    for(i = MAX_STACK_HEIGHT-1; i>=SP; i--)

    {

        if(actRecord==i)
        {
            fprintf(output, " |");
        }


         fprintf(output, "%d ", stack[i]);
    }


   fprintf(output, "\n\n");
}

//VM
void pm( int vm)
{
     input =   fopen("parseroutput.txt", "r");
     output =   fopen("finaloutput.txt", "w");

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
    printCode(index);
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
    fclose(output);

 if(vm) 
    {
     //echoes VM in output
      char t;
      output = fopen("finaloutput.txt", "r");
      fseek(output, 0, SEEK_SET);
      printf("Virtual Machine: \n");
      t = fgetc(output);
      while(t != EOF) {
         printf("%c", t);
         t = fgetc(output);
        }
        printf("\n\n");
        fclose(output);
    }


}

