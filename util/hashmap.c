#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/*
    hashmap structure:
    {
        label:  index = hash["label"] % TABLE_SIZE,
        addr:   "0x0000" 2 byte
    }

*/

#define IDX_BASE            0xfeff
#define IDX_LAST            0xffff
#define HASH_TABLE_SIZE     ( IDX_LAST - IDX_BASE ) // 256 loactions
#define INVALID_INDEX       HASH_TABLE_SIZE
#define SLOT_UNUSED         0x0000
#define SLOT_DELETED        IDX_LAST // use last marker for available slot
#define PRIME_TABLE_SIZE    25

/* prime numbers from 1 - 100 */
static const uint16_t _PRIME_TABLE[PRIME_TABLE_SIZE] =
{
    2, 3, 5, 7, 11,
    13, 17, 19, 23, 29,
    31, 37, 41, 43, 47,
    53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};

uint16_t hashmap[HASH_TABLE_SIZE] = { SLOT_UNUSED };

/*
 * return hash table index from label.
 * if label is NULL, return INVALID_INDEX
 */
int hash(const char* label)
{
    int index = INVALID_INDEX;
    if (NULL == label)
    {
        return index;
    }

    // calculate hash
    index = 0;
    int len = strlen(label);
    for (int i = 0; i < len; i++)
    {
        index += (label[i] * 256 + label[i] * _PRIME_TABLE[i % PRIME_TABLE_SIZE]);
    }

    index %= HASH_TABLE_SIZE;

    return index;
}

int insert(const char* label, uint16_t addr)
{
    int index = hash(label);
    if (INVALID_INDEX == index)
    {
        return 1; // error
    }

    int try = index;
    // for (int i = 0; i < HASH_TABLE_SIZE; i++)
    // {
    do
    {
        if (SLOT_UNUSED != hashmap[try] && SLOT_DELETED != hashmap[try])
        {
            // collision handling - open addressing (try next slot)
            printf("collision for 0x%x at [%d] \n", addr, try);
            try = (try + 1) % HASH_TABLE_SIZE;
            continue;
        }
        else
        {
            // insert addr in hashmap at index
            hashmap[try] = addr;
            // printf("inserted 0x%x at hashmap[%d]\n", addr, try);
            return 0; // status ok
        }
    }
    while(try != index);

    // }

    return 1; // error - table full
}


void print_hashmap()
{
    for (uint16_t i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (hashmap[i] == SLOT_UNUSED)
        {
            printf("hashmap[%d]\t-----\n", i);
        }
        else if (hashmap[i] == SLOT_DELETED)
        {
            printf("hashmap[%d]\tdeleted\n", i);
        }
        else
        {
            printf("hashmap[%d]\t0x%x\n", i, hashmap[i]);
        }
    }
}

int main()
{

    char* labels[20] =
    {
        "123456",
        "data",
        "start",
        "main",
        "loop",
        "end",
        "swap",
        "a",
        "b",
        "c",
        "x",
        "y",
        "z",
        "strange_var_name",
        "stranger_variable_name"
    };


    for (int i = 1; i < 20; i++)
    {
        insert(labels[i], (uint16_t)i);
        // printf("inserting... %s 0x%x \n", labels[i], i);
    }

    printf("\n\n");

    // print_hashmap();
}