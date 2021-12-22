/*
Jeremy Galvan
COP3402-20Fall 0001
HW2 Scanner
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "function.h"

#define MAX_IDENT 11
#define MAX_DIGITS 5
#define MAX_INTEGER 32767
#define MAX_NEST 5
#define MAX_STRING 256
#define MAX_NAME_TABLE_SIZE 1000



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

const char *ReservWord[] =
{"const", "var", "procedure","call",
"begin", "end", "if", "then", "else",
"while", "do", "read", "write", "odd"};

const char SpecSymbols[] =
{'+','-','*','/','('
,')','=',',','.','<'
,'>',';',':'};

void lexical(int lex, char inputName[])
{

/*Declares table for structs and index*/
tokStruct lexer[MAX_NAME_TABLE_SIZE];
int ind;


FILE *input, *input2, *output, *output2;
/*index initialized*/
ind = 0;

/*Initializes Token*/
for(int i=0;i<MAX_NAME_TABLE_SIZE;i++)
{
    lexer[i].toKen=0;
}

void printOutput(FILE* input, FILE* output,FILE* output2 , tokStruct table[], int ind, int lex );
int ifInvisibleSpace(int m);

/*count is used for index, check checks for errors,
charc reads characters,endov looks for how many chars are ahead
and comm looks for comments*/
int count,  check,charc,comm,endov;
input = fopen(inputName, "r");
input2 = fopen(inputName, "r");
output = fopen("lexeme.txt","w");
output2 = fopen("lexemelist.txt","w");



/*reads next character*/
charc=fgetc(input);


/*while loop will iterate until the the program finishes reading input*/
while(charc!=EOF)
{
    /*Invisible characters are ignored*/
    if(ifInvisibleSpace(charc))
    {
        charc=fgetc(input);
        endov=0;
        continue;
    }

    /* Reads if character is an alphabet*/
    if(isalpha(charc))
    {

     /* String [12] to hold string*/
     char String[MAX_IDENT+1];

    /*fills String to size*/
     memset(String, 0 , sizeof(String) );

     /*count*/
     int count = 0;



     String[count]=charc;

     count++;
     endov=1;

     while(isalpha(charc=fgetc(input)) || isdigit(charc))
            {

                /*Prints error if result is too long*/
                if(count > MAX_IDENT){
                    printf("error: identifier is too long.\n");
                    while (isalpha(charc=fgetc(input))||isdigit(charc)) { }
                    check =1;
                    break;
                }
                String[count]= charc;
                count++;
            }

        /*Ends iteration if error was present*/
        if(check==1)
        {
            check=0;
            continue;
        }



            int compare=-1;


            for(count=0; count< sizeof(ReservWord)/sizeof( ReservWord[0]);count++){
                 /*if count matches word*/
                if(strcmp(String, ReservWord[count])==0)
                    {

                    compare=count;

                    }
            }

            /*Determines values of Token type*/
            switch(compare)
            {


                /*Constant identifier*/
                case 0:
                    lexer[ind].toKen = constsym;
                    strcpy(lexer[ind].symbol, "const");
                    break;
                /*Variable identifier*/
                case 1:
                    lexer[ind].toKen = varsym;
                    strcpy(lexer[ind].symbol, "var");
                    break;
                /*Procedure identifier*/
                case 2:
                    lexer[ind].toKen = procsym;
                    strcpy(lexer[ind].symbol, "proc");
                    break;
                /*Call identifier*/
                case 3:
                    lexer[ind].toKen = callsym;
                    strcpy(lexer[ind].symbol, "call");
                    break;
                /*Begin identifier*/
                case 4:
                    lexer[ind].toKen = beginsym;
                    strcpy(lexer[ind].symbol, "begin");
                    break;
                /*End identifier*/
                case 5:
                    lexer[ind].toKen= endsym;
                    strcpy(lexer[ind].symbol, "end");
                    break;
                /*If identifier*/
                case 6:
                    lexer[ind].toKen = ifsym;
                    strcpy(lexer[ind].symbol, "if");
                    break;
                /*Then identifier*/
                case 7:
                    lexer[ind].toKen = thensym;
                    strcpy(lexer[ind].symbol, "then");
                    break;
                /*Else identifier*/
                case 8:
                    lexer[ind].toKen = elsesym;
                    strcpy(lexer[ind].symbol, "else");
                    break;
                /*While identifier*/
                case 9:
                    lexer[ind].toKen = whilesym;
                    strcpy(lexer[ind].symbol, "while");
                    break;
                /*Do identifier*/
                case 10:
                    lexer[ind].toKen = dosym;
                    strcpy(lexer[ind].symbol, "do");
                    break;
                /*Read identifier*/
                case 11:
                    lexer[ind].toKen = readsym;
                    strcpy(lexer[ind].symbol, "read");
                    break;
                /*Write identifier*/
                case 12:
                    lexer[ind].toKen = writesym;
                    strcpy(lexer[ind].symbol, "write");
                    break;
                /*Odd identifier*/
                case 13:
                    lexer[ind].toKen = oddsym;
                    strcpy(lexer[ind].symbol, "odd");
                    break;
                /*Switch defaults to identifier*/
                default:

                    lexer[ind].toKen = identsym;
                    strcpy(lexer[ind].ident,String);
                    break;
            }
            ind++;
        }

        /*Checks if character is a digit*/
        else if(isdigit(charc))
        {
            int num=charc-'0',d, p=1;

            endov=1;


            while(isdigit(charc=fgetc(input))){
                if(p>MAX_NAME_TABLE_SIZE){
                    /*Checks if number is more than a 1000*/
                    printf("error : This number is larger than 1000.\n");
                    while (isdigit(charc=fgetc(input))) {

                    }
                    check=1;

                    break;
                }
                d=charc-'0';
                num=10*num+d;
                p++;
            }

           if(isalpha(charc)){
                /*Prints if variable doesn't start with a letter*/
                printf("error : Variable does not start with letter.\n");
                while(isalpha(charc=fgetc(input))||isdigit(charc)){

                }
                continue;
             }


            if(check==1) {
                check=0;
                continue;
            }

            /*Assigns token and value with number*/
            lexer[ind].value = num;
            lexer[ind].toKen = numbersym;

            ind++;
        }

        else 
        {
            endov=0;

            /*Used for symbol*/
            int specialS=-1;

            for(int i=0;i< sizeof(SpecSymbols)/sizeof( SpecSymbols[0]);i++){
                /*If character matches char symbol index, index will be assigned
                to variable*/
                if(charc==SpecSymbols[i]){
                    specialS=i;
                }
            }

            /*checks symbol identifier*/
            switch(specialS){

                /*plus symbol*/
                case 0:
                    lexer[ind].toKen = plussym;
                    strcpy(lexer[ind].symbol, "+");
                    ind++;
                    break;

                /*minus symbol*/
                case 1:
                    lexer[ind].toKen = minussym;
                    strcpy(lexer[ind].symbol, "-");
                    ind++;
                    break;

                /*multiple symbol*/
                case 2:
                    lexer[ind].toKen = multsym;
                    strcpy(lexer[ind].symbol, "*");
                    ind++;
                    break;

                /* These statements makes sure frontslash is not a comment*/
                case 3:
                    charc=fgetc(input);
                    endov=1;
                    if(charc=='*'){
                        comm=1;
                       endov=0;
                        charc=fgetc(input);
                        while(comm==1){
                            if(charc=='*'){
                                charc=fgetc(input);
                                if(charc=='/'){
                                    comm=0;
                                }
                            }
                            else{
                                charc=fgetc(input);
                            }
                        }
                    }
                    else{
                        /*frontslash symbol*/
                        lexer[ind].toKen = slashsym;
                        strcpy(lexer[ind].symbol, "/");
                        ind++;
                    }
                    break;

                /*left parenthesis symbol*/
                case 4:
                    lexer[ind].toKen= lparentsym;
                    strcpy(lexer[ind].symbol, "(");
                    ind++;
                    break;

                /*right parenthesis symbol*/
                case 5:
                    lexer[ind].toKen = rparentsym;
                    strcpy(lexer[ind].symbol, ")");
                    ind++;
                    break;

                /*equal symbol*/
                case 6:
                    lexer[ind].toKen= eqsym;
                    strcpy(lexer[ind].symbol, "=");
                    ind++;
                    break;

                /*comma symbol*/
                case 7:
                    lexer[ind].toKen = commasym;
                    strcpy(lexer[ind].symbol, ",");
                    ind++;
                    break;

                /*period symbol*/
                case 8:
                    lexer[ind].toKen = periodsym;
                    strcpy(lexer[ind].symbol, ".");
                    ind++;
                    break;

                /* checks equal to*/
                case 9:
                    charc=fgetc(input);
                    endov=1;
                     /*not equal to symbol*/
                    if(charc=='>'){
                        lexer[ind].toKen = neqsym;
                        strcpy(lexer[ind].symbol, "!=");
                       endov=0;
                    }

                     /*less than or equal to symbol*/
                    else if(charc=='='){
                        lexer[ind].toKen = leqsym;
                        strcpy(lexer[ind].symbol, "<=");
                       endov=0;
                    }
                     /* less than symbol*/
                    else{
                        lexer[ind].toKen = lessym;
                        strcpy(lexer[ind].symbol, "<");
                    }
                    ind++;
                    break;


                case 10:
                    charc=fgetc(input);
                    endov=1;
                     /*greater than or equal to symbol*/
                    if(charc=='='){
                        lexer[ind].toKen= geqsym;
                        strcpy(lexer[ind].symbol, ">=");
                        endov=0;
                    }
                     /*greater than symbol*/
                    else{
                        lexer[ind].toKen = gtrsym;
                        strcpy(lexer[ind].symbol, ">");
                    }
                    ind++;
                    break;

                /*semi-colon symbol*/
                case 11:
                    lexer[ind].toKen = semicolonsym;
                    strcpy(lexer[ind].symbol, ";");
                    ind++;
                    break;


                case 12:
                    charc=fgetc(input);
                    if(charc == '='){
                        /*become symbol*/
                        lexer[ind].toKen = becomessym;
                        strcpy(lexer[ind].symbol, ":=");
                        ind++;
                        }

                        /*Prints if symbol is invalid*/
                     else{
                        printf("error: invalid symbols detected.\n");
                        }

                    break;

                default:
                    /*Prints if symbol is invalid*/
                    printf("error: invalid symbols detected.\n");
                    break;
            }
        }

        if(endov==0)
        {
            charc=fgetc(input);
        }
    }


        /*prints out symbols*/
        printOutput(input2, output, output2, lexer, ind, lex);

        

    /*closes files*/
    fclose(input);
    fclose(input2);
    fclose(output);
    fclose(output2);




}


/*Prints output*/
void printOutput(FILE* input,FILE* output, FILE* output2, tokStruct lexer[], int ind , int lex)
{


    fprintf(output,"\n\nSample Program: \n");

    if(lex==1)
    printf("\n\nSample Program: \n");
    int c=fgetc(input);
    while(c!=EOF)
    {
        /*Prints sample text file*/
        fprintf(output,"%c",c );
        if(lex==1)
        printf("%c",c );
        c=fgetc(input);

    }

    fprintf(output,"\n--------------\n\n" );
    if(lex==1)
    printf("\n--------------\n\n" );

    fprintf(output,"\n\nLexeme Table: \n");
    if(lex==1)
    printf("\n\nLexeme Table: \n");
    for(int i=0;i<ind;i++)
    {
        /*Prints Identifier if token==2*/
        if(lexer[i].toKen==2)
        {
        fprintf(output,"%s\t",lexer[i].ident );
        if(lex==1)
        printf("%s\t",lexer[i].ident );
        }

        /*Prints Number if token==3*/
        else if(lexer[i].toKen==3)
        {
       fprintf(output,"%d\t",lexer[i].value );
       if(lex==1)
        printf("%d\t",lexer[i].value );
        }

        /*Prints symbol*/
        else
        {
        fprintf(output,"%s\t",lexer[i].symbol );
        if(lex==1)
        printf("%s\t",lexer[i].symbol );
        }

        /*Prints token*/
        fprintf(output,"%d\n", lexer[i].toKen );
        if(lex==1)
        printf("%d\n", lexer[i].toKen );
    }

    fprintf(output,"\n--------------\n\n" );
    if(lex==1)
    printf("\n--------------\n\n" );



    fprintf(output,"\n\nLexeme List: \n");
    if(lex==1)
    printf("\n\nLexeme List: \n");
    for(int i=0;i<ind;i++){

        /*Prints token*/
        fprintf(output," %d", lexer[i].toKen);
        fprintf(output2," %d", lexer[i].toKen);
        if(lex==1)
        printf(" %d", lexer[i].toKen);

        /*Prints identifier if token==2*/
        if(lexer[i].toKen==2){
           fprintf(output," %s", lexer[i].ident);
           fprintf(output2," %s", lexer[i].ident);
           if(lex==1)
           printf(" %s", lexer[i].ident);
        }

        /*Prints number if token==3*/
        else if(lexer[i].toKen==3){
            fprintf(output," %d",lexer[i].value);
            fprintf(output2," %d", lexer[i].toKen);
            if(lex==1)
            printf(" %d",lexer[i].value);
        }

}
    fprintf(output,"\n--------------\n\n" );
    if(lex==1)
    printf("\n--------------\n\n" );
}

/*Checks for tabs, spaces, and newlines*/
int ifInvisibleSpace(int m)
{
    if(m=='\t'||m==' '||m=='\n')
        return 1;
    else
        return 0;

}





