#include "baby_vm.h"

int main(int argc, char* argv[])
{
    char* buffer = NULL;
    Arguments args = {0};

    /*
     *  baby_vm should take (at least) one argument:
     *  path and name of the assembly.txt file
     *  arguments supplied to the program to be executed (OPTIONAL)
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
            LOG("argument too long. exceeds maximum argument length. exiting gracefully...");
            exit(EXIT_FAILURE);
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

    int16_t rv = process(&args, &buffer);

    // @TODO: need a better way to handle return value.
    // display return value
    printf("\n%d\n", rv);

    LOG("\nshutting down...\n");

    exit(EXIT_SUCCESS);
}
