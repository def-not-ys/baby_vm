#include "memory.h"
#include "../util/hashmap.h"

static uint16_t _memory[UINT16_MAX];
static int text_pos; // available text region slot
static int data_pos; // available data region slot

Memory memory = {};
Hashmap hashmap = {};

uint16_t pc; // program counter
uint16_t sp; // stack pointer

#include "memory_private_helper.c"

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
    split line by space token and process token
*/
static ErrorStatus _memory_process_line(char* line, Section* section)
{
    ErrorStatus status = ERR_NONE;

    // process instruction
    char* remaining_line = NULL;
    char* token = strtok_r(line, " ", &remaining_line);

#if DEBUG_ON
    printf("token: [ %s ]\n", token);
#endif // DEBUG_ON

    if (NULL != token)
    {
        status |= _token_handler(token, section);
    }

    while (NULL != token)
    {
        token = strtok_r(NULL, " ", &remaining_line);
#if DEBUG_ON
        printf("token: [ %s ]\n", token);
#endif // DEBUG_ON
        if (NULL != token)
        {
            status |= _token_handler(token, section);
        }
    }

    return status;
}

/*
    load instructions into memory
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
#if DEBUG_ON
        printf("after split by #: [ %s ]\n", line);
#endif // DEBUG_ON
        if (NULL != line)
        {
            status |= _memory_process_line(line, &section);
        }
    }

#if DEBUG_ON
    _examine_memory();
#if DEBUG_MODE
    hashmap_test(&hashmap);
#endif // DEBUGMODE
#endif // DEBUG_ON

    return status;
}

void memory_shutdown()
{
    hashmap.clear(&hashmap);
}