#ifndef __HACK_H__
#define __HACK_H__

#include "hack.h"
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 200

#define NUM_PREDEFINED_SYMBOLS 23

#define OPCODE_TO_BINARY(opcode) \
    ((opcode & 0x8000) ? '1' : '0'), \
    ((opcode & 0x4000) ? '1' : '0'), \
    ((opcode & 0x2000) ? '1' : '0'), \
    ((opcode & 0x1000) ? '1' : '0'), \
    ((opcode & 0x0800) ? '1' : '0'), \
    ((opcode & 0x0400) ? '1' : '0'), \
    ((opcode & 0x0200) ? '1' : '0'), \
    ((opcode & 0x0100) ? '1' : '0'), \
    ((opcode & 0x0080) ? '1' : '0'), \
    ((opcode & 0x0040) ? '1' : '0'), \
    ((opcode & 0x0020) ? '1' : '0'), \
    ((opcode & 0x0010) ? '1' : '0'), \
    ((opcode & 0x0008) ? '1' : '0'), \
    ((opcode & 0x0004) ? '1' : '0'), \
    ((opcode & 0x0002) ? '1' : '0'), \
    ((opcode & 0x0001) ? '1' : '0')


typedef enum {
    SYM_R0 = 0,
    SYM_R1 = 1,
    SYM_R2 = 2,
    SYM_R3 = 3,
    SYM_R4 = 4,
    SYM_R5 = 5,
    SYM_R6 = 6,
    SYM_R7 = 7,
    SYM_R8 = 8,
    SYM_R9 = 9,
    SYM_R10 = 10,
    SYM_R11 = 11,
    SYM_R12 = 12,
    SYM_R13 = 13,
    SYM_R14 = 14,
    SYM_R15 = 15,
    SYM_SCREEN = 16384,
    SYM_KBD = 24576,
	SYM_SP = 0,
	SYM_LCL = 1,
	SYM_ARG = 2,
	SYM_THIS = 3,
	SYM_THAT = 4,
} symbol_id;

typedef struct {
	char name[10];
	int16_t address;
} predefined_symbol;

extern predefined_symbol predefined_symbols[NUM_PREDEFINED_SYMBOLS];

typedef enum {
	JMP_INVALID = -1,
	JMP_NULL = 0,
	JMP_JGT = 1,
	JMP_JEQ = 2,
	JMP_JGE = 3,
	JMP_JLT = 4,
	JMP_JNE = 5,
	JMP_JLE = 6,
	JMP_JMP = 7
} jump_id;

typedef enum {
	DEST_INVALID = -1,
	DEST_NULL = 0,
	DEST_M = 1,
	DEST_D = 2,
	DEST_MD = 3,
	DEST_A = 4,
	DEST_AM = 5,
	DEST_AD = 6,
	DEST_AMD = 7
} dest_id;

typedef enum {
    COMP_INVALID = -1,
    COMP_0 = 42,
    COMP_1 = 63,
    COMP_NEG_1 = 58,
    COMP_D = 12,
    COMP_A = 48,
    COMP_M = 48,
    COMP_NOT_D = 13,
    COMP_NOT_A = 49,
    COMP_NOT_M = 49,
    COMP_NEG_D = 15,
    COMP_NEG_A = 51,
    COMP_NEG_M = 51,
    COMP_D_PLUS_1 = 31,
    COMP_A_PLUS_1 = 55,
    COMP_M_PLUS_1 = 55,
    COMP_D_MINUS_1 = 14,
    COMP_A_MINUS_1 = 50,
    COMP_M_MINUS_1 = 50,
    COMP_D_PLUS_A = 2,
    COMP_D_PLUS_M = 2,
    COMP_D_MINUS_A = 19,
    COMP_D_MINUS_M = 19,
    COMP_A_MINUS_D = 7,
    COMP_M_MINUS_D = 7,
    COMP_D_AND_A = 0,
    COMP_D_AND_M = 0,
    COMP_D_OR_A = 21,
    COMP_D_OR_M = 21
} comp_id;

static inline jump_id str_to_jumpid(const char *s) {
    jump_id id = JMP_INVALID;
	//printf("DEBUG: str_to_jumpid input = '%s'\n", s);

    if (s == NULL) {
        id = JMP_NULL;
    } else if (strcmp(s, "JGT") == 0) {
        id = JMP_JGT;
    } else if (strcmp(s, "JEQ") == 0) {
        id = JMP_JEQ;
    } else if (strcmp(s, "JGE") == 0) {
        id = JMP_JGE;
    } else if (strcmp(s, "JLT") == 0) {
        id = JMP_JLT;
    } else if (strcmp(s, "JNE") == 0) {
        id = JMP_JNE;
    } else if (strcmp(s, "JLE") == 0) {
        id = JMP_JLE;
    } else if (strcmp(s, "JMP") == 0) {
        id = JMP_JMP;
    }
	
	//printf("DEBUG: str_to_jumpid result = %d\n", id);

    return id;
}

static inline dest_id str_to_destid(const char *s) {
    dest_id id = DEST_INVALID;

    if (s == NULL) {
        id = DEST_NULL;
    } else if (strcmp(s, "M") == 0) {
        id = DEST_M;
    } else if (strcmp(s, "D") == 0) {
        id = DEST_D;
    } else if (strcmp(s, "MD") == 0) {
        id = DEST_MD;
    } else if (strcmp(s, "A") == 0) {
        id = DEST_A;
    } else if (strcmp(s, "AM") == 0) {
        id = DEST_AM;
    } else if (strcmp(s, "AD") == 0) {
        id = DEST_AD;
    } else if (strcmp(s, "AMD") == 0) {
        id = DEST_AMD;
    }

    return id;
}

static inline comp_id str_to_compid(const char *s, int *a) {
    comp_id id = COMP_INVALID;
	char comp_trimmed[MAX_LINE_LENGTH];

    while (isspace((unsigned char)*s)) s++;
	strncpy(comp_trimmed, s, MAX_LINE_LENGTH - 1);
	comp_trimmed[MAX_LINE_LENGTH - 1] = '\0';
	
	char *end = comp_trimmed + strlen(comp_trimmed) - 1;
	while (end > comp_trimmed && isspace((unsigned char)*end)) end--;
	end[1] = '\0'; // Null-terminate the trimmed string
	
	if (strlen(comp_trimmed) == 0) {
		*a = 0;
		return id;
	}

    if (strcmp(comp_trimmed, "0") == 0) {
        id = COMP_0;
        *a = 0;
    } else if (strcmp(comp_trimmed, "1") == 0) {
        id = COMP_1;
        *a = 0;
    } else if (strcmp(comp_trimmed, "-1") == 0) {
        id = COMP_NEG_1;
        *a = 0;
    } else if (strcmp(comp_trimmed, "D") == 0) {
        id = COMP_D;
        *a = 0;
    } else if (strcmp(comp_trimmed, "A") == 0) {
        id = COMP_A;
        *a = 0;
    } else if (strcmp(comp_trimmed, "M") == 0) {
        id = COMP_M;
        *a = 1;
    } else if (strcmp(comp_trimmed, "!D") == 0) {
        id = COMP_NOT_D;
        *a = 0;
    } else if (strcmp(comp_trimmed, "!A") == 0) {
        id = COMP_NOT_A;
        *a = 0;
    } else if (strcmp(comp_trimmed, "!M") == 0) {
        id = COMP_NOT_M;
        *a = 1;
    } else if (strcmp(comp_trimmed, "-D") == 0) {
        id = COMP_NEG_D;
        *a = 0;
    } else if (strcmp(comp_trimmed, "-A") == 0) {
        id = COMP_NEG_A;
        *a = 0;
    } else if (strcmp(comp_trimmed, "-M") == 0) {
        id = COMP_NEG_M;
        *a = 1;
    } else if (strcmp(comp_trimmed, "D+1") == 0) {
        id = COMP_D_PLUS_1;
        *a = 0;
    } else if (strcmp(comp_trimmed, "A+1") == 0) {
        id = COMP_A_PLUS_1;
        *a = 0;
    } else if (strcmp(comp_trimmed, "M+1") == 0) {
        id = COMP_M_PLUS_1;
        *a = 1;
    } else if (strcmp(comp_trimmed, "D-1") == 0) {
        id = COMP_D_MINUS_1;
        *a = 0;
    } else if (strcmp(comp_trimmed, "A-1") == 0) {
        id = COMP_A_MINUS_1;
        *a = 0;
    } else if (strcmp(comp_trimmed, "M-1") == 0) {
        id = COMP_M_MINUS_1;
        *a = 1;
    } else if (strcmp(comp_trimmed, "D+A") == 0) {
        id = COMP_D_PLUS_A;
        *a = 0;
    } else if (strcmp(comp_trimmed, "D+M") == 0) {
        id = COMP_D_PLUS_M;
        *a = 1;
    } else if (strcmp(comp_trimmed, "D-A") == 0) {
        id = COMP_D_MINUS_A;
        *a = 0;
    } else if (strcmp(comp_trimmed, "D-M") == 0) {
        id = COMP_D_MINUS_M;
        *a = 1;
    } else if (strcmp(comp_trimmed, "A-D") == 0) {
        id = COMP_A_MINUS_D;
        *a = 0;
    } else if (strcmp(comp_trimmed, "M-D") == 0) {
        id = COMP_M_MINUS_D;
        *a = 1;
    } else if (strcmp(comp_trimmed, "D&A") == 0) {
        id = COMP_D_AND_A;
        *a = 0;
    } else if (strcmp(comp_trimmed, "D&M") == 0) {
        id = COMP_D_AND_M;
        *a = 1;
    } else if (strcmp(comp_trimmed, "D|A") == 0) {
        id = COMP_D_OR_A;
        *a = 0;
    } else if (strcmp(comp_trimmed, "D|M") == 0) {
        id = COMP_D_OR_M;
        *a = 1;
    }

    return id;
}

void add_predefined_symbols();

#endif