/****************************************
 * C-ploration 6 for CS 271
 * 
 * [NAME] $Declan Bartel$
 * [TERM] FALL $2024$
 * 
 ****************************************/
#include "parser.h"
#include "symtable.h"
#include "error.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

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
			printf("%u: %c (%s)\n", instr_num, inst_type, label);
            continue;
        }
		
		if (is_Atype(line)) {
			inst_type = 'A';
		} else if (is_Ctype(line)) {
			inst_type = 'C';
		} else {
			inst_type = ' ';
		}
		
		if (inst_type == 'A' || inst_type == 'C') {
			instr_num++;
		}

        if (instr_num > MAX_INSTRUCTIONS) {
            exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
        }
        printf("%u: %c  %s\n", instr_num, inst_type, line);
    }
}
