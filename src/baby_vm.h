#ifndef BABY_VM_H
#define BABY_VM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "types_private.h"

#define MAX_BUFFER_LEN              250

#define LOG(msg)                    ( printf("%s\n", msg) )
#define LOG_COMP(expected, actual)  ( printf("expected: %d, actual %d\n", (expected), (actual)) )

#define MIN(x, y)                   ( x < y ? x : y)
#define MAX(x, y)                   ( x > y ? x : y)

#define DEBUG_ON                    0

// memory model
ErrorStatus memory_init(void);
ErrorStatus memory_load_instructions(FILE* ptr_file, Arguments* ptr_arg);
ErrorStatus memory_get_instruction(uint16_t* pc, Instruction* buf);
void memory_shutdown(void);

// hardware model
ErrorStatus hardware_init();
void hardware_shutdown();

// process model

#endif // BABY_VM_H