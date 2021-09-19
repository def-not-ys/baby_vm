#ifndef BABY_VM_H
#define BABY_VM_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "types_private.h"
#include "memory.h"

extern Memory memory;

// hardware model
ErrorStatus hardware_init();
ErrorStatus hardware_execute_instruction(Instruction* pc, bool* is_halt);
void hardware_shutdown();

// process model

#endif // BABY_VM_H