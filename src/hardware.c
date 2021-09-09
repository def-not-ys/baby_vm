#include "baby_vm.h"

// hardware model

/*  SUBLEQ implementation
    Instruction subleq a, b, c
        Mem[b] = Mem[b] - Mem[a]
        if (Mem[b] ≤ 0)
            goto c
*/

ErrorStatus hardware_init()
{
    // nothing to do
    return ERR_NONE;
}

void hardware_shutdown()
{
    // nothing to do
    return;
}

// process instruction
ErrorStatus hardware_execute_instruction(Instruction* pc, bool* is_halt)
{
    // stub
    *is_halt = TRUE;
    return ERR_NONE;
}
