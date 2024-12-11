// #include "symtable.h"
// #include <string.h>
// #include <stdlib.h>
// #include <stdio.h>

// Symbol* hashArray[SYMBOL_TABLE_SIZE];

// int hash(char *str) {
	// unsigned long hash = 5381;
	// int c;
	
	// while ((c = *str++)) {
		// hash = ((hash << 5) + hash) + c;
	// }
	
	// return hash % SYMBOL_TABLE_SIZE;
// }

// void symtable_insert(char* key, hack_addr addr) {
	// Symbol* item = (Symbol*) malloc(sizeof(Symbol));
	// item->name = strdup(key);
	// item->addr = addr;
	// int hashIndex = hash(key);
	// while(hashArray[hashIndex] != NULL) {
		// ++hashIndex;
		// hashIndex %= SYMBOL_TABLE_SIZE;
	// }
	
	// hashArray[hashIndex] = item;
// }

// Symbol* symtable_find(char *key) {
	// int hashIndex = hash(key);
	// while(hashArray[hashIndex] != NULL) {
		// if (strcmp(hashArray[hashIndex]->name, key) == 0) {
			// return hashArray[hashIndex];
		// }
		// ++hashIndex;
		// hashIndex %= SYMBOL_TABLE_SIZE;
	// }
	
	// return NULL;
// }

// void symtable_display_table() {
	// for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
		// if (hashArray[i] != NULL) {
			// printf("(%s, %d)\n", hashArray[i]->name, hashArray[i]->addr);
		// } else {
			// printf(" ~~ \n");
		// }
	// }
// }

// void symtable_print_labels() {
	// for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
		// if (hashArray[i] != NULL) {
			// printf("{%s,%d}\n", hashArray[i]->name,hashArray[i]->addr);
		// }
	// }
// }

#include "symtable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SYMBOL_TABLE_SIZE 1000

static symbol symbol_table[SYMBOL_TABLE_SIZE];
static int symbol_table_count = 0;

void symtable_insert(const char *name, uint16_t address) {
    if (symbol_table_count < SYMBOL_TABLE_SIZE) {
        symbol_table[symbol_table_count].name = strdup(name);
        symbol_table[symbol_table_count].address = address;
        symbol_table_count++;
    }
}

symbol *symtable_find(const char *name) {
    for (int i = 0; i < symbol_table_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return &symbol_table[i];
        }
    }
    return NULL;
}