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

typedef enum
{
    UINITIALIZED,
    SECTION,
    TEXT,
    DATA,
    INVALID
} Section;

static uint16_t _memory[UINT16_MAX];
static int text_pos; // available text region slot
static int data_pos; // available data region slot

#include "../util/hashmap.c"
#include "memory_private_helper.c"

Memory memory = {};
Hashmap hashmap = {};

uint16_t pc; // program counter
uint16_t sp; // stack pointer

static void _memory_init_private()
{
    memory.reserved  = (Data*)&_memory[STACK_REGION_START];
    memory.ptr_stack = &_memory[STACK_REGION_START];
    memory.ptr_heap  = &_memory[HEAP_REGION_START];
    memory.ptr_data  = &_memory[DATA_REGION_START];
    memory.ptr_text  = (Instruction*)&_memory[TEXT_REGION_START];
}

ErrorStatus memory_init()
{
    pc = TEXT_REGION_START;
    sp = STACK_REGION_START;
    text_pos = TEXT_REGION_START;
    data_pos = DATA_REGION_START;
    _memory_init_private();
    hashmap_init(&hashmap, &memory);
    return ERR_NONE;
}

ErrorStatus memory_get_instruction(uint16_t* pc, Instruction* buf)
{
    // @TODO: get instruction at mem[pc] and return the read instruction via buf
    // stub
    return ERR_NONE;
}

/*
    split line by space token
    process line token array:
    {
        for each token:
            token handler()
    }
*/
static ErrorStatus _memory_process_line(char* line, Section* section)
{
    ErrorStatus status = ERR_NONE;
    LOG("processing instructions...");

    // process instruction
    char* remaining_line = NULL;
    char* token = strtok_r(line, " ", &remaining_line);
    printf("token: [ %s ]\n", token);

    if (NULL != token)
    {
        status |= _token_handler(token, section);
    }

    while (NULL != token)
    {
        token = strtok_r(NULL, " ", &remaining_line);
        printf("token: [ %s ]\n", token);
        if (NULL != token)
        {
            status |= _token_handler(token, section);
        }
    }

    return status;
}

/*
    loading instructions into memory
    while (not EOF)
    {
        read a line from the file()
        process line token array(char* line, state):
    }
*/
ErrorStatus memory_load_instructions(FILE* ptr_file, Arguments* ptr_arg)
{
    ErrorStatus status = ERR_NONE;
    char buf[MAX_BUFFER_LEN];
    memset(buf, 0, MAX_BUFFER_LEN);

    Section section = UINITIALIZED;

    while ((fgets(buf, MAX_BUFFER_LEN, ptr_file) != NULL) && ERR_NONE == status)
    {
        // omit comment start with #
        char* line = strtok(buf, "#");
        line = strtok(line, "\r\n");
        printf("after split by #: [ %s ]\n", line);
        if (NULL != line)
        {
            status |= _memory_process_line(line, &section);
        }
    }


#if DEBUG_MODE
    _examine_memory();
    hashmap_test(&hashmap);
#endif // DEBUGMODE

    return status;
}

void memory_shutdown()
{
    //@TODO: clear hashmap and free related memory
    hashmap.clear(&hashmap);
}