#include <stdio.h>
#include <string.h>
#include "baby_vm.h"


#define MAX_BUFFER_LEN              250

#define LOG(msg)                    ( printf("%s\n", msg) )
#define LOG_COMP(expected, actual)  ( printf("expected: %d, actual %d\n") )


void wait_for_data(char** buffer)
{
    // wait for user to enter the file name and stored the input data in buffer
    // stub
}

// process model
int main(int argc, char* argv[])
{
    char input_buffer[MAX_BUFFER_LEN] = {0};
    Arguments args = {0};
    bool is_running = TRUE;

    /*
     *  baby_vm should take at least one arguments:
     *  path and name of the assembly.txt file
     *  arguments supplied to the program to be executed
     */

    if (argc < 2)
    {
        LOG("Please enter file to be executed, or press q to quit");
        wait_for_data(&input_buffer);
    }
    else
    {
        if (strlen(argv[1]) >= MAX_BUFFER_LEN)
        {
            // for simplicity. do not support dynamically allocate long string
        LOG("argument too long. exceeds maximum argument length. exiting grafaully...");
            return 0;
        }

        // concanate to existing input buffer
        strcpy(input_buffer, argv[1]);

         args.list = &argv[2];
         args.len = argc - 2;

    }

    LOG("exiting...");
}