#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>

#define MAX_BUFFER_LEN              250

#define LOG(msg)                    ( printf("%s\n", msg) )
#define LOG_COMP(expected, actual)  ( printf("expected: %d, actual %d\n", (expected), (actual)) )

#define MIN(x, y)                   ( x < y ? x : y)
#define MAX(x, y)                   ( x > y ? x : y)

#define DEBUG_ON                    0

/*  general */
typedef enum {
    FALSE,
    TRUE
} boolean;

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


/*  processor model */

#endif // _TYPES_H
