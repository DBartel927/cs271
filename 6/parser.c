/****************************************
 * C-ploration 6 for CS 271
 * 
 * [NAME] $Declan Bartel$
 * [TERM] FALL $2024$
 * 
 ****************************************/
#include "parser.h"
#include "symtable.h"
#include "hack.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

predefined_symbol predefined_symbols[NUM_PREDEFINED_SYMBOLS] = {
    {"R0", SYM_R0}, 
    {"R1", SYM_R1},
    {"R2", SYM_R2},
    {"R3", SYM_R3},
    {"R4", SYM_R4},
    {"R5", SYM_R5},
    {"R6", SYM_R6},
    {"R7", SYM_R7},
    {"R8", SYM_R8},
    {"R9", SYM_R9},
    {"R10", SYM_R10},
    {"R11", SYM_R11},
    {"R12", SYM_R12},
    {"R13", SYM_R13},
    {"R14", SYM_R14},
    {"R15", SYM_R15},
    {"SCREEN", SYM_SCREEN},
    {"KBD", SYM_KBD},
	{"SP", SYM_SP},
	{"LCL", SYM_LCL},
	{"ARG", SYM_ARG},
	{"THIS", SYM_THIS},
	{"THAT", SYM_THAT},
};

uint16_t instruction_to_opcode(c_instruction instr);

void assemble(const char *file_name, instruction *instructions, int num_instructions) {
    char output_file_name[MAX_LINE_LENGTH];
    snprintf(output_file_name, sizeof(output_file_name), "%s.hack", file_name);

    FILE *output_file = fopen(output_file_name, "w");
    if (!output_file) {
        exit_program(EXIT_CANNOT_OPEN_FILE, output_file_name);
    }

    for (int i = 0; i < num_instructions; i++) {
        uint16_t opcode = 0;
        if (instructions[i].itype == INST_A) {
            if (instructions[i].instr.a.is_addr) {
                opcode = instructions[i].instr.a.operand.address;
            } else {
                const symbol *sym = symtable_find(instructions[i].instr.a.operand.label);
                if (sym) {
                    opcode = sym->address;
                } else {
					static int next_available_address = 16;
                    symtable_insert(instructions[i].instr.a.operand.label, next_available_address);
                    opcode = next_available_address;
					next_available_address++;
                }
                free(instructions[i].instr.a.operand.label);
            }
        } else if (instructions[i].itype == INST_C) {
            opcode = instruction_to_opcode(instructions[i].instr.c);
        }

        fprintf(output_file, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n", OPCODE_TO_BINARY(opcode));
    }

    fclose(output_file);
}

uint16_t instruction_to_opcode(c_instruction instr) {
    uint16_t op = 0;
    op |= (7 << 13);

    op |= (instr.a << 12);
    op |= (instr.comp << 6);
    op |= (instr.dest << 3);
    op |= instr.jump;

    return op;
}

 void add_predefined_symbols () {
	 for (int i = 0; i < NUM_PREDEFINED_SYMBOLS; i++) {
		 symtable_insert(predefined_symbols[i].name, predefined_symbols[i].address);
	 }
 }

bool is_Atype(const char *line) {
	return line[0] == '@';
}

bool is_label(const char *line) {
	int len = strlen(line);
	return len > 1 && line[0] == '(' && line[len - 1] ==  ')';
}

bool is_Ctype(const char *line) {
	return !is_Atype(line) && !is_label(line);
}

char *extract_label(const char *line, char *label) {
	int len = strlen(line);
	strncpy(label, line + 1, len - 2);
	label[len - 2] = '\0';
	return label;
}

bool parse_A_instruction(const char *line, a_instruction *instr) {
    char *s = (char *)malloc(strlen(line));
    strcpy(s, line + 1);

    char *s_end = NULL;
    long result = strtol(s, &s_end, 10);

    if (s == s_end) {
        instr->operand.label = (char *)malloc(strlen(s) + 1);
        strcpy(instr->operand.label, s);
        instr->is_addr = false;
    } else if (*s_end != 0) {
        free(s);
        return false;
    } else {
        instr->operand.address = result;
        instr->is_addr = true;
    }

    free(s);
    return true;
}

void parse_C_instruction(char *line, c_instruction *instr) {
    char *temp = strtok(line, ";");
    char *jump = strtok(NULL, ";");
    char *dest = strtok(temp, "=");
    char *comp = strtok(NULL, "=");
	int a = 0;

    if (comp == NULL) {
        comp = dest;
        dest = NULL;
    }
	
	char jump_trimmed[MAX_LINE_LENGTH];
	if (jump != NULL) {
		while (isspace((unsigned char)*jump)) jump++; // Skip leading whitespace
		strncpy(jump_trimmed, jump, MAX_LINE_LENGTH - 1);
		jump_trimmed[MAX_LINE_LENGTH - 1] = '\0';
		char *end = jump_trimmed + strlen(jump_trimmed) - 1;
		while (end > jump_trimmed && isspace((unsigned char)*end)) end--;
		end[1] = '\0'; // Null-terminate the trimmed string 
	} else {
		strcpy(jump_trimmed, "");
	}

    instr->jump = str_to_jumpid(jump);
    instr->dest = str_to_destid(dest);
    instr->comp = str_to_compid(comp, &a);
	instr->a = a;
	
	//printf("DEBUG: dest=%s, comp=%s, jump=%s\n", dest, comp, jump);
	//printf("DEBUG: dest_id=%d, comp_id=%d, jump_id=%d\n", instr->dest, instr->comp, instr->jump);
}

/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
void strip(char *line) {
    char *start = line;
    while (isspace((unsigned char)*start)) {
        start++;
    }
    memmove(line, start, strlen(start) + 1);
	
    char *end = line + strlen(line) - 1;
    while (end > line && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }

    char *comment_start = strstr(line, "//");
    if (comment_start != NULL) {
        *comment_start = '\0';
    }
}


// char *strip(char *s){	

	//char s_new[MAX_LINE_LENGTH];
	//int i = 0;
	//for (char *s2 = s; *s2; s2++) {
//		if (*s2 == '/' && *(s2 + 1) == '/') {
//			break;
//		}
//		else if (!isspace((unsigned char)*s2)) {
//			s_new[i++] = *s2;
//		}
//	}
//	s_new[i] = '\0';
//	strcpy(s, s_new);
  //  return s;	
//}

/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments. 
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */
 
//void parse(FILE *file) {
int parse(FILE *file, instruction *instructions) {
    char line[MAX_LINE_LENGTH];
    unsigned int line_num = 0;
    unsigned int instr_num = 0;
	//char inst_type;
	char label[MAX_LABEL_LENGTH];
	instruction instr;
	
	add_predefined_symbols();

    while (fgets(line, sizeof(line), file)) {
        line_num++;
        strip(line);
		if (strlen(line) == 0) {
			continue;
		}
        if (is_label(line)) {
			//inst_type = 'L';
            extract_label(line, label);
            strcpy(line, label);
            if (!isalpha(label[0])) {
                exit_program(EXIT_INVALID_LABEL, line_num, line);
            }
			if (symtable_find(label) != NULL) {
                exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, line);
            }
            symtable_insert(label, instr_num);
            continue;
        }
		
		if (is_Atype(line)) {
			//inst_type = 'A';
			if (!parse_A_instruction(line, &instr.instr.a)) {
				exit_program(EXIT_INVALID_A_INSTR, line_num, line);
			}
			instr.itype = INST_A;
		} else if (is_Ctype(line)) {
			//inst_type = 'C';
			char tmp_line[MAX_LINE_LENGTH];
			strcpy(tmp_line, line);
			parse_C_instruction(tmp_line, &instr.instr.c);
			
			//printf("DEBUG: Parsed C-instruction on line %d\n", line_num);
			
			if (instr.instr.c.dest == DEST_INVALID) {
				exit_program(EXIT_INVALID_C_DEST, line_num, line);
			}
			if (instr.instr.c.comp == COMP_INVALID) {
				exit_program(EXIT_INVALID_C_COMP, line_num, line);
			}
			if (instr.instr.c.jump == JMP_INVALID) {
				exit_program(EXIT_INVALID_C_JUMP, line_num, line);
			}
			instr.itype = INST_C;
		} //else {
			//inst_type = ' ';
		//}

        if (instr_num >= MAX_INSTRUCTIONS) {
            exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
        }
        //printf("%u: %c  %s\n", instr_num, inst_type, line);
		//instr_num++;
		instructions[instr_num++] = instr;
    }
	return instr_num;
}
