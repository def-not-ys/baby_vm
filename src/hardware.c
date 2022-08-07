#include "baby_vm.h"

// hardware model

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

/*  SUBLEQ implementation
    Instruction subleq a, b, c
        Mem[b] = Mem[b] - Mem[a]
        if (Mem[b] ≤ 0)
            goto c
*/
static ErrorStatus subleq(Instruction* ins, boolean* should_branch)
{
    ErrorStatus status = ERR_NONE;
    int16_t a, b, tmp;
    status |= memory_read(ins->src, &a);
    status |= memory_read(ins->dest, &b);

    tmp = b - a;
    status |= memory_write(ins->dest, tmp);

    if (tmp <= 0)
    {
        *should_branch = TRUE;
    }
    return status;
}

// decoded instruction
void hardware_decode_instruction(Instruction* pc)
{
    // offset data read memory address to data region
    pc->src += DATA_REGION_START;
    pc->dest += DATA_REGION_START;

    // brch address is in text region, no decode required.
}

// process instruction
ErrorStatus hardware_execute_instruction(Instruction* pc, boolean* is_halt, int16_t* rv)
{
    ErrorStatus status = ERR_NONE;
    boolean should_branch = FALSE;

    status |= subleq(pc, &should_branch);

    if (TRUE == should_branch)
    {
        // update memory.ptr_text to point to the branch address
        status |= memory_set_branch_addr(pc, &memory);

        if (HALT == pc->brch)
        {
            *is_halt = TRUE;
            // set return value
            status |= memory_read(DATA_RV, rv);
        }
    }
    return status;
}
