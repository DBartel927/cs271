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
 
void parse(FILE *file) {
    char line[MAX_LINE_LENGTH];
    unsigned int line_num = 0;
    unsigned int instr_num = 0;
	char inst_type;
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
			inst_type = 'L';
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
			inst_type = 'A';
			if (!parse_A_instruction(line, &instr.instr.a)) {
				exit_program(EXIT_INVALID_A_INSTR, line_num, line);
			}
			instr.itype = INST_A;
		} else if (is_Ctype(line)) {
			inst_type = 'C';
			instr.itype = INST_C;
		} else {
			inst_type = ' ';
		}

        if (instr_num > MAX_INSTRUCTIONS) {
            exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
        }
        // printf("%u: %c  %s\n", instr_num, inst_type, line);
		instr_num++;
    }
}
