#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>

/*  general */
typedef enum
{
    FALSE,
    TRUE
} bool;

typedef enum
{
    ERR_NONE,
    ERR_ATTN
} ErrorStatus;

typedef struct
{
    char** list;
    int len;
} Arguments;

/*  hardware model */
enum opcodes
{
    OP_HALT   = 0x0000,
    OP_SUBLEQ = 0x4000
};

enum condition_flag
{
    FL_POS = 1 << 0,
    FL_ZRO = 1 << 1,
    FL_NEG = 1 << 2
};

/*  processor model */

#endif // _TYPES_H