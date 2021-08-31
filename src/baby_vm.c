#include <stdio.h>
#include <string.h>
#include "baby_vm.h"

#define MAX_BUFFER_LEN              250

#define LOG(msg)                    ( printf("%s\n", msg) )
#define LOG_COMP(expected, actual)  ( printf("expected: %d, actual %d\n", (expected), (actual)) )

#define MIN(x, y)                   ( x < y? x : y)
#define MAX(x, y)                   ( x > y? x : y)

#define DEBUG_ON                    1

static char* fake_path = "examples/example_input.txt";
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

ErrorStatus load_instructions(char** prt_buf, Arguments* ptr_arg)
{
    // @TODO: loads instructions from file into memory
    // stub
    return ERR_NONE;
}

ErrorStatus process_instructions()
{
    // @TODO: process labels in the instructions
    // stub
    return ERR_NONE;
}

ErrorStatus run_instructions()
{
    // @TODO: runs the instructions
    // stub
    return ERR_NONE;
}

// process model
int main(int argc, char* argv[])
{
    char* buffer = NULL;
    Arguments args = {0};
    bool is_running = TRUE;

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