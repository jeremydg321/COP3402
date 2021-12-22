
/*

Jeremy Galvan
COP3402-20Fall 0001
HW3 Compiler
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"

int main(int argc, char *argv[]) {


    int lex, parse, vm;
    lex=0;
    parse = 0;
    vm = 0;
    for(int i = 0; i < argc; i++) 
    {
        if((strcmp(argv[i], "-l") == 0))
            lex = 1;

        if((strcmp(argv[i], "-a") == 0))
            parse = 1;

        if((strcmp(argv[i], "-v") == 0))
            vm = 1;
    }
       

    lexical(lex, argv[1]);


    parser(parse);

    pm(vm);

    return 0;
}
