#include "baby_vm.h"

static char* fake_path = "examples/test.txt";
static char* fake_args = "fake args";

void wait_for_data(char** ptr_buf, Arguments* ptr_arg)
{
    // @TODO: wait for user to enter the file name and stored the input data in buffer
    // stub
    *ptr_buf = fake_path;
    LOG(*ptr_buf);
    int len = strlen(fake_path);
    ptr_arg->list = &fake_args;
    ptr_arg->len = 1;
}

ErrorStatus baby_vm_init()
{
    ErrorStatus status = ERR_ATTN;
    status = memory_init(&memory);
    status |= hardware_init();
    return status;
}

ErrorStatus load_instructions(char** ptr_buf, Arguments* ptr_arg)
{
    ErrorStatus status = ERR_ATTN;

    assert(*ptr_buf != NULL);

    // open file
    FILE* ptr_file = fopen(*ptr_buf, "r");

    if (ptr_file!=NULL)
    {
        // pass fd to memory to read instructions into memory
        status = memory_load_instructions(ptr_file, ptr_arg);
        fclose (ptr_file);
    }
    return status;
}

/*
    get the pointer to the current instruction to pc
    and increment the memory text pointer to point to the next instruction
*/
static void _get_instruction(Instruction** pc)
{
    *pc = memory.ptr_text++;
}


/*
    FETCH - fetch the next instruction from memory
    (DECODE)
    EXECUTE - execute the instruction
    (WRITE BACK)
*/
ErrorStatus run_instructions(int16_t* rv)
{
    ErrorStatus status = ERR_NONE;
    Instruction* pc = NULL; // program counter
    boolean is_halt = FALSE;

    do
    {
        /* FETCH */
        _get_instruction(&pc);
        assert(NULL != pc);

        /* DECODE - done by type inference */
        hardware_decode_instruction(pc);

        /* EXECUTE */
        status |= hardware_execute_instruction(pc, &is_halt, rv);

        /* WRITE BACK - done in EXECUTE */
    }
    while (FALSE == is_halt && ERR_NONE == status);

    return status;
}

void baby_vm_shutdown()
{
    memory_shutdown();
    hardware_shutdown();
}

// process model
int16_t process(Arguments* args, char** buffer)
{

    ErrorStatus status = ERR_ATTN;
    int16_t rv = 0;

    status = baby_vm_init();
    if (ERR_NONE != status)
        {
            LOG("failed to initialize baby vm. exiting gracefully...");
            exit(EXIT_FAILURE);
        }

    status = load_instructions(buffer, args);
    if (ERR_NONE != status)
        {
            LOG("failed to load instructions. exiting gracefully...");
            baby_vm_shutdown();
            exit(EXIT_FAILURE);
        }

    status = run_instructions(&rv);
    if (ERR_NONE != status)
        {
            LOG("failed to run instructions. exiting gracefully...");
            baby_vm_shutdown();
            exit(EXIT_FAILURE);
        }

    baby_vm_shutdown();

    return rv;
}
