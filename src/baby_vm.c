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
    status = memory_init();
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

ErrorStatus process_instructions()
{
    // @TODO: process sections and labels in the instructions
    // stub
    ErrorStatus status = ERR_ATTN;
    status = memory_process_instructions();
    return status;
}

ErrorStatus run_instructions()
{
    // @TODO: runs the instructions
    // stub
    /*
        FETCH - fetch the next instruction from memory
        (DECODE)
        EXECUTE - execute the instruction
        (WRITE BACK)

        bool is_halt = FALSE;
        pc = 0x0000;
        do
        {
            instruction = memory_get_next_instruction(&pc);
            hardware_execute_instruction(&pc, &is_halt);
            increment pc;
        }
        while (not is_halt)

    */
    return ERR_NONE;
}

// process model
int main(int argc, char* argv[])
{
    char* buffer = NULL;
    Arguments args = {0};

    /*
     *  baby_vm should take (at least?) one argument:
     *  path and name of the assembly.txt file
     *  arguments supplied to the program to be executed (???)
     */

    if (argc < 2)
    {
        LOG("Please enter file to be executed, or press q to quit");
        wait_for_data(&buffer, &args);
    }
    else
    {
        if (strlen(argv[1]) >= MAX_BUFFER_LEN)
        {
            // for simplicity. do not support dynamically allocate long string
            LOG("argument too long. exceeds maximum argument length. exiting grafaully...");
            return 0;
        }

        buffer = argv[1];

        args.list = &argv[2];
        args.len = argc - 2;

    }

#if DEBUG_ON
    LOG("after assigning new value to buffer");
    LOG(buffer);
    LOG("arg.list: ");
    LOG(*(args.list));
    LOG_COMP(MAX(argc-2, 0), args.len);
#endif // DEBUG_ON

    // @TODO: to be implemented
    ErrorStatus status = ERR_NONE;
    status = baby_vm_init();
    if (ERR_NONE != status)
    {
        LOG("failed to initialize baby vm.");
    }
    status = load_instructions(&buffer, &args);
    if (ERR_NONE != status)
    {
        LOG("failed to load instructions.");
    }
    status = process_instructions();
    if (ERR_NONE != status)
    {
        LOG("failed to process instructions.");
    }
    status = run_instructions();
    if (ERR_NONE != status)
    {
        LOG("failed to run instructions.");
    }

    LOG("exiting...");
}