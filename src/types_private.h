#include <stdint.h>
// #define UINT16_MAX 0xffff

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
}