/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $Declan Bartel$
 * [TERM] FALL $2024$
 * 
 ****************************************/
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "symtable.h"
#include "error.h"

#define MAX_INSTRUCTION_COUNT 30000

int main(int argc, char **argv) {
    if (argc != 2) {
        exit_program(EXIT_INCORRECT_ARGUMENTS, argv[0]);
    }
    
    FILE *file = fopen(argv[1], "r");
    if (!file) {
        exit_program(EXIT_CANNOT_OPEN_FILE, argv[1]);
    }
	
	instruction *instructions = malloc(MAX_INSTRUCTION_COUNT * sizeof(instruction));
	int num_instructions = parse(file, instructions);
	
	// for (int i = 0; i < num_instructions; i++) {
		// if (instructions[i].itype == INST_A) {
			// if (instructions[i].instr.a.is_addr) {
				// printf("A: %d\n", instructions[i].instr.a.operand.address);
			// } else {
				// printf("A: %s\n", instructions[i].instr.a.operand.label);
			// }
		// } else if (instructions[i].itype == INST_C) {
			// printf("C: d=%d, c=%d, j=%d\n", instructions[i].instr.c.dest, instructions[i].instr.c.comp, instructions[i].instr.c.jump);
		// }
	// }

    //parse(file);
	
	//symtable_print_labels();

    fclose(file);
	
	assemble(argv[1], instructions, num_instructions);
	
	free(instructions);
    return 0;
}