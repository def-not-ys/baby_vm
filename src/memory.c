#include "baby_vm.h"

#define TEXT_REGION_START   0x0000
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
    total text region in bytes: 255 (0xfff) * 2 byte/slot (uint16_t) = 510 bytes = 6 * 85
    maximum of 85 + 1(HALT) = 86 instructions

*/

// @TODO: implement HASHMAP to store label-address pair

typedef struct
{
    const char*     label;
    uint16_t        reserved; // for alignment
    uint16_t        addr;
} Data;

typedef struct
{
	Data*             reserved;   // pointer to the reserved data region for hashmap
	const uint16_t*   ptr_stack;  // pointer to the start of stack region
	const uint16_t*   ptr_heap;   // pointer to the start of the heap region
	const uint16_t*   ptr_data;   // pointer to the user data region
	const uint16_t*   ptr_text;   // pointer to the text region
} Memory;

#include "../util/hashmap.c"

uint16_t _memory[UINT16_MAX];

Memory memory = {};
Hashmap hashmap = {};

uint16_t pc; // program counter
uint16_t sp; // stack pointer

static void _init_private()
{
    // @TODO: init memory struct
    // void* p = (void*)STACK_REGION_START;
    memory.reserved  = (Data*)&_memory[STACK_REGION_START];
    memory.ptr_stack = &_memory[STACK_REGION_START];
    memory.ptr_heap  = &_memory[HEAP_REGION_START];
    memory.ptr_data  = &_memory[DATA_REGION_START];
    memory.ptr_text  = &_memory[TEXT_REGION_START];
}

ErrorStatus memory_init()
{
    _init_private();
    hashmap_init(&hashmap, &memory);
    return ERR_NONE;
}


ErrorStatus memory_load_instructions(FILE* ptr_file, Arguments* ptr_arg)
{
    // @TODO: loads instructions from file into memory
    // stub
    char str[25];
    while (fgets(str, 25, ptr_file) != NULL)
    {
        puts(str);
    }
    return ERR_NONE;
}

ErrorStatus memory_process_instructions(void)
{
    // @TODO: porcess sections in memory
    // @TODO: process labels in memory
    // stub
    LOG("processing instructions...");

#if DEBUG_MODE
    hashmap_test(&hashmap);
#endif // DEBUGMODE

    return ERR_NONE;
}