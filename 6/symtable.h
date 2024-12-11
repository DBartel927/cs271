#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <stdint.h>

#define SYMBOL_TABLE_SIZE 1000

typedef int16_t hack_addr;

typedef struct {
	char *name;
	//hack_addr address;
	uint16_t address;
} symbol;

//int hash(char *str);
//symbol *symtable_find(char *key);
//void symtable_insert(char *key, hack_addr addr);
//void symtable_display_table();
//void symtable_print_labels();
void symtable_insert(const char *name, uint16_t address);
symbol *symtable_find(const char *name);

#endif