/****************************************
 * C-ploration 5 for CS 271
 * 
 * [NAME] $Declan Bartel$
 * [TERM] FALL $2024$
 * 
 ****************************************/
#include "parser.h"

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
/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
char *strip(char *s){	

	char s_new[MAX_LINE_LENGTH];
	int i = 0;
	for (char *s2 = s; *s2; s2++) {
		if (*s2 == '/' && *(s2 + 1) == '/') {
			break;
		}
		else if (!isspace((unsigned char)*s2)) {
			s_new[i++] = *s2;
		}
	}
	s_new[i] = '\0';
	strcpy(s, s_new);
    return s;	
}

/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments. 
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */
void parse(FILE * file){
	char line[MAX_LINE_LENGTH];
	while(fgets(line, sizeof(line), file)) {
		strip(line);
		if(*line == '\0') {
			continue;
		}
		char inst_type = ' ';
		if (is_Atype(line)) {
			inst_type = 'A';
		}
		else if (is_label(line)) {
			inst_type = 'L';
		}
		else if (is_Ctype(line)) {
			inst_type = 'C';
		}
		printf("%c  %s\n", inst_type, line);
	}
	
	
}	