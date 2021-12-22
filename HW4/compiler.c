
/*

Jeremy Galvan
COP3402-20Fall 0001
HW4 Compiler
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"

int main(int argc, char *argv[]) {



 

    lexical(argv[1]);


    parse();

    pm();

    return 0;
}
