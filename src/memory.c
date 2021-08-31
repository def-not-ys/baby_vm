#include "baby_vm.h"

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
*/
uint16_t memory[UINT16_MAX];

ErrorStatus memory_load_instructions(FILE* ptr_file, Arguments* ptr_arg)
{
    // @TODO: loads instructions from file into memor
    // stub
    return ERR_NONE;
}
