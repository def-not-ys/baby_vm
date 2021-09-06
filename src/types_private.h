#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>

/*  general */
typedef enum
{
    FALSE,
    TRUE
} bool;

typedef enum
{
    ERR_NONE,
    ERR_ATTN
} ErrorStatus;

typedef struct
{
    char** list;
    int len;
} Arguments;

/*  memory model */
typedef struct
{
    const char*     label;
    uint16_t        reserved;   // for alignment
    uint16_t        addr;
} Data;

typedef struct
{
    //uint16_t      opcode; (omit)
    uint16_t        src;
    uint16_t        dest;
    uint16_t        brch;
} Instruction;

typedef struct
{
	Data*           reserved;   // pointer to the reserved data region for hashmap
	uint16_t*       ptr_stack;  // pointer to the start of stack region
	uint16_t*       ptr_heap;   // pointer to the start of the heap region
	uint16_t*       ptr_data;   // pointer to the user data region
	Instruction*    ptr_text;   // pointer to the text region
} Memory;

/*  hardware model */
enum opcodes
{
    OP_HALT   = 0x0000,
    OP_SUBLEQ = 0x4000
};

enum condition_flag
{
    FL_POS = 1 << 0,
    FL_ZRO = 1 << 1,
    FL_NEG = 1 << 2
};

/*  processor model */


#endif // _TYPES_H