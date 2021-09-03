#include "types_private.h"

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


// process instruction

