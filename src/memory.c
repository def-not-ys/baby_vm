#include "memory.h"
#include "../util/hashmap.h"

Memory memory;

static uint16_t _memory[UINT16_MAX];
static int text_pos; // available text region slot
static int data_pos; // available data region slot

static Hashmap hashmap = {};

#include "memory_private_helper.c"

static void _memory_init_private(Memory* memory)
{
    memory->reserved  = (Data*)&_memory[STACK_REGION_START];
    memory->addr_stack = STACK_REGION_START;
    memory->addr_heap  = HEAP_REGION_START;
    memory->addr_data  = DATA_REGION_START;
    memory->ptr_text  = (Instruction*)&_memory[TEXT_REGION_START];
}

ErrorStatus memory_init(Memory* memory)
{
    text_pos = TEXT_REGION_START;
    data_pos = DATA_REGION_USER;
    _memory_init_private(memory);
    hashmap_init(&hashmap, memory);

    // initialze special values
    _memory[DATA_ZERO] = 0;
    _memory[DATA_ONE] = 1;
    _memory[DATA_RESERVED_1] = 0xff;
    _memory[DATA_RESERVED_2] = 0xff;

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

/*
    read mem[addr] to *(p_val) and return the error status
*/
ErrorStatus memory_read(uint16_t addr, int16_t* ptr_val)
{
    assert(NULL != ptr_val);
    if (addr < 0x0000 || addr > UINT16_MAX)
    {
        // addr out of bound
        *ptr_val = 0;
        return ERR_ATTN;
    }
    else
    {
        *ptr_val = (int16_t)_memory[addr];
        return ERR_NONE;
    }
}

/*
    write value to mem[addr] and return the error status
*/
ErrorStatus memory_write(uint16_t addr, int16_t value)
{
    if (addr < DATA_REGION_START || addr > STACK_REGION_START)
    {
        // read only region or addr out of bound
        return ERR_ATTN;
    }
    else
    {
        _memory[addr] = (uint16_t)value;
        return ERR_NONE;
    }

}

/*
    update memory.ptr_text to point to the branch address
*/
ErrorStatus memory_set_branch_addr(Instruction* pc, Memory* memory)
{
    uint16_t addr = pc->brch;
    if (addr < TEXT_REGION_START || addr > TEXT_REGION_LAST)
    {
        LOG("invalid branching location");
        return ERR_ATTN;
    }
    memory->ptr_text = (Instruction*)&_memory[addr];
    return ERR_NONE;
}

void memory_shutdown()
{
    hashmap.clear(&hashmap);
}