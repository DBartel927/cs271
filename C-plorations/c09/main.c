/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $Declan Bartel$
 * [TERM] FALL $2024$
 * 
 ****************************************/
#include <stdio.h>
#include "parser.h"
#include "symtable.h"
#include "error.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        exit_program(EXIT_INCORRECT_ARGUMENTS, argv[0]);
    }
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        exit_program(EXIT_CANNOT_OPEN_FILE, argv[1]);
    }

    parse(file);
	
	symtable_print_labels();

    fclose(file);
    return 0;
}