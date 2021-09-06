#ifndef MEM_H
#define MEM_H

#include "baby_vm.h"

#define TEXT_REGION_START   0x0003 // first 3 slots are reserved
#define TEXT_REGION_LAST    0x0fff // @TODO: revisit this!
#define TEXT_REGION_SIZE    ( TEXT_REGION_LAST - TEXT_REGION_START )
#define DATA_REGION_START   0x1000
#define DATA_REGION_LAST    0x13ff
#define DATA_REGION_SIZE    ( DATA_REGION_LAST - DATA_REGION_START )

#define STACK_REGION_START  0xfbff
#define HEAP_REGION_START   0x1400

// memory model
/* 65535 (2^16) bytes
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
             |   Data   |
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

    @TODO: revisit the text section, may need more space for text region

    reserve 0x0fff for HALT instruction
    total text region in bytes: (255 (0xfff) - 3) * 2 byte/slot (uint16_t) = 504 bytes = 6 * 84
    maximum of 84 + 1(HALT) = 85 instructions

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
	Data*           reserved;   // pointer to the reserved data region for hashmap
	uint16_t*       ptr_stack;  // pointer to the start of stack region
	uint16_t*       ptr_heap;   // pointer to the start of the heap region
	uint16_t*       ptr_data;   // pointer to the user data region
	Instruction*    ptr_text;   // pointer to the text region
} Memory;

/* memeory functions */
ErrorStatus memory_init(void);
ErrorStatus memory_load_instructions(FILE* ptr_file, Arguments* ptr_arg);
ErrorStatus memory_get_instruction(uint16_t* pc, Instruction* buf);
void memory_shutdown(void);

#endif // MEM_H