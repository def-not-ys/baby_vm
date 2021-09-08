#ifndef MEM_H
#define MEM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "types_private.h"

#define TEXT_REGION_START   0x0003 // first 3 slots are reserved
#define TEXT_REGION_LAST    0x0fff // @TODO: revisit this!
#define TEXT_REGION_SIZE    ( TEXT_REGION_LAST - TEXT_REGION_START )
#define DATA_REGION_START   0x1000
#define DATA_REGION_LAST    0x13ff
#define DATA_REGION_SIZE    ( DATA_REGION_LAST - DATA_REGION_START )

#define STACK_REGION_START  0xfbff
#define HEAP_REGION_START   0x1400

// memory model
/* 65535 (2^16) slot * 2 sizeof(uint16_t) = 131072 bytes ~ 131 KB
    illustration NOT TO SCALE
    0xffff   +----------+
             | Reserved |   1024 bytes
             |          |
    0xfbff   +----------+
             |          |
             |  Stack   |
             |          |
             +----+-----+
             |    |     |
             |    v     |
             |          |
             |    ^     |
             |    |     |
             +----+-----+
             |   Heap   |
             |          |
    0x1400   +----------+
             |   Data   |   1024 bytes
             |          |
    0x1000   +----------+
             |          |
             |   Text   |
             |          |
    0x0000   +----------+

    opcode is omiited as all instructions have the same opcode
    instruction structure:
                (OP)    addr.a  addr.b  addr.c
    in bytes:   0       2       2       2       -> 6 byte instruction

    reserve 0x0fff for HALT instruction

    total text region in bytes: (4095 (0xfff) - 3) * 2 byte/slot (uint16_t) = 8184 bytes = 6 * 1364
    maximum of 1364 instructions

*/

/*  memory data type */
typedef enum
{
    UINITIALIZED,
    SECTION,
    TEXT,
    DATA,
    INVALID
} Section;

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
	Data*           reserved;       // pointer to the reserved data region for hashmap
	uint16_t        addr_stack;     // addr to the start of stack region
	uint16_t        addr_heap;      // addr to the start of the heap region
	uint16_t        addr_data;      // addr to the user data region
	Instruction*    ptr_text;       // pointer to the text region
} Memory;

/* memeory functions */
ErrorStatus memory_init(Memory* memory);
ErrorStatus memory_load_instructions(FILE* ptr_file, Arguments* ptr_arg);
ErrorStatus memory_read(uint16_t addr, uint16_t* ptr_val);
ErrorStatus memory_write(uint16_t addr, uint16_t value);
void memory_shutdown(void);

#endif // MEM_H