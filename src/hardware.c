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
    //stub
    return ERR_NONE;
}

void hardware_shutdown()
{
    // stub
    return;
}

// process instruction
ErrorStatus hardware_execute_instruction(Instruction* pc)
{
    // stub
    return ERR_NONE;
}
