// #ifndef __PARSER_H__
// #define __PARSER_H__

// /****************************************
 // * C-ploration 6 for CS 271
 // * 
 // * [NAME] $Declan Bartel$
 // * [TERM] FALL $2024$
 // * 
 // ****************************************/
// #include <stdio.h>
// #include <string.h>
// #include <stdlib.h>
// #include <ctype.h>
// #include <stdbool.h>
// #include <stdint.h>
// #include "symtable.h"
// #include "hack.h"
// #include "error.h"

// #define MAX_HACK_ADDRESS INT16_MAX
// #define MAX_INSTRUCTIONS MAX_HACK_ADDRESS
// #define MAX_LINE_LENGTH  200
// #define MAX_LABEL_LENGTH (MAX_LINE_LENGTH - 2)

// typedef int16_t hack_addr;
// typedef int16_t opcode;

// typedef enum {
	// INVALID_INSTRUCTION = -1,
	// A_TYPE_INSTRUCTION,
	// C_TYPE_INSTRUCTION
// } instr_type;

// typedef struct {
	// opcode a:1;
	// opcode comp:7;
	// opcode dest:3;
	// opcode jump:3;
// } c_instruction;

// typedef struct {
	// union {
		// hack_addr address;
		// char *label;
	// };
	// bool is_addr;
// } a_instruction;

// typedef struct {
	// union {
		// a_instruction a_instr;
		// c_instruction c_instr;
	// };
	// instr_type type;
// } instruction;

// typedef struct  {
	// union {
		// int16_t address;
		// char *label;
	// } operand;
	// bool is_addr;
// } a_instruction;

// /** function prototypes **/
// void strip(char *line);

// void parse(FILE *file);

// bool is_Atype(const char *);

// bool is_label(const char *);

// bool is_Ctype(const char *);

// char *extract_label(const char *, char *);

// bool parse_A_instruction(const char *line, a_instruction *instr);

// #endif

#ifndef __PARSER_H__
#define __PARSER_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "symtable.h"
#include "hack.h"
#include "error.h"

#define MAX_HACK_ADDRESS INT16_MAX
#define MAX_INSTRUCTIONS MAX_HACK_ADDRESS
#define MAX_LINE_LENGTH  200
#define MAX_LABEL_LENGTH (MAX_LINE_LENGTH - 2)

typedef struct {
    union {
        int16_t address;
        char *label;
    } operand;
    bool is_addr;
} a_instruction;

typedef struct {
	int comp : 7;
	int dest : 4;
	int jump : 4;
	int a : 1;
} c_instruction;

typedef enum {
    INST_A,
    INST_C
} instruction_type;

typedef struct {
    instruction_type itype;
    union {
        a_instruction a;
		c_instruction c;
    } instr;
} instruction;

// Function declarations
bool is_Atype(const char *line);
bool is_label(const char *line);
bool is_Ctype(const char *line);
char *extract_label(const char *line, char *label);
void strip(char *line);
//void parse(FILE *file);
int parse(FILE *file, instruction *instructions);
void add_predefined_symbols();
bool parse_A_instruction(const char *line, a_instruction *instr);
void parse_C_instruction(char *line, c_instruction *instr);

#endif