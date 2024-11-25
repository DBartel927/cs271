/****************************************
 * C-ploration 4 for CS 271
 * 
 * [NAME] $Declan Bartel$
 * [TERM] FALL $2024$
 * 
 ****************************************/
#include "parser.h"
#include "symtable.h"

int main(int argc, const char *argv[])
{		

if (argc != 2) {
	printf("Usage: %s [filename]\n", argv[0]);
	exit(EXIT_FAILURE);
}
FILE *file = fopen(argv[1], "r");
if (file == NULL) {
	perror("Error opening file");
	exit(EXIT_FAILURE);
}
parse(file);
symtable_print_labels();
fclose(file);
}