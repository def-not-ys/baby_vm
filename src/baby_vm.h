#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "types_private.h"

#define MAX_BUFFER_LEN              250

#define LOG(msg)                    ( printf("%s\n", msg) )
#define LOG_COMP(expected, actual)  ( printf("expected: %d, actual %d\n", (expected), (actual)) )

#define MIN(x, y)                   ( x < y ? x : y)
#define MAX(x, y)                   ( x > y ? x : y)

#define DEBUG_ON                    1

// memory model
ErrorStatus memory_load_instructions(FILE* ptr_file, Arguments* ptr_arg);
ErrorStatus memory_process_instructions(void);

// hardware model

// process model