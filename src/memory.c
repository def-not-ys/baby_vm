#include "baby_vm.h"

#define TEXT_REGION_START   0x0000
#define TEXT_REGION_LAST    0x0fff // @TODO: revisit this!
#define DATA_REGION_START   0x1000
#define DATA_REGION_LAST    0x13ff

#define STACK_REGION_START  0xfeff
#define HEAP_REGION_START   0x1400

// memory model
/* 65535 (2^16) locations
    0xffff   +----------+
             | Reserved |
    0xfeff   +----------+
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

uint16_t memory[UINT16_MAX];

uint16_t pc; // program counter
uint16_t sp; // stack pointer

ErrorStatus memory_load_instructions(FILE* ptr_file, Arguments* ptr_arg)
{
    // @TODO: loads instructions from file into memor
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
    return ERR_NONE;
}