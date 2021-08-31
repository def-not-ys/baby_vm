#include <stdint.h>
// #define UINT16_MAX 0xffff

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

enum opcodes
{
    OP_SUBLEQ = 0,
    OP_LAST
};

enum condition_flag
{
    FL_POS = 1 << 0,
    FL_ZRO = 1 << 1,
    FL_NEG = 1 << 2
};