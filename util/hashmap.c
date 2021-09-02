#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define IDX_BASE            0xfbff
#define IDX_LAST            0xffff
#define SIZE_OF_DATA        8 // this will only work with size_t = 4 byte
#define HASH_TABLE_SIZE     ( ( IDX_LAST - IDX_BASE ) / SIZE_OF_DATA ) // 128 slots
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


typedef struct
{
    const char*     label;
    uint16_t        reserved; // for alignment
    uint16_t        addr;
} Data;

Data hashmap[HASH_TABLE_SIZE] = {};

/*
 * return hash table index from label.
 * if label is NULL, return INVALID_INDEX
 */
int hash(const char* label)
{
    int index = INVALID_INDEX;
    if (NULL == label)
    {
        // assert(label != NULL && "invalid null label");
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

    do
    {
        if (SLOT_UNUSED != hashmap[try].addr && SLOT_DELETED != hashmap[try].addr)
        {
            // collision handling - open addressing (try next slot)
            printf("collision for { %s\t0x%x } at [%d] \n", label, addr, try);
            try = (try + 1) % HASH_TABLE_SIZE;
            continue;
        }
        else
        {
            // insert addr in hashmap at index
            hashmap[try].label = label;
            hashmap[try].addr = addr;
            return 0; // status ok
        }
    }
    while(try != index);

    return 2; // error - table full
}

// return the address found.
// if not found, return IDX_LAST
uint16_t find(const char* label)
{

    uint16_t addr = IDX_LAST;
    if (NULL == label)
    {
        assert(label != NULL && "invalid lable");
        return addr;
    }

    int index = hash(label);
    int try = index;
    int not_match = 0;

    do
    {
        if (NULL == hashmap[try].label || SLOT_UNUSED == hashmap[try].addr)
        {
            // label does not exist, stop looking
            printf("label %s not found \n", label);
            break;
        }
        else
        {
            not_match = strncmp(hashmap[try].label, label, strlen(label));
            if (0 == not_match)
            {
                // found
                printf("found label %s \n", label);
                return hashmap[try].addr;
            }
            else if (not_match && SLOT_DELETED == hashmap[try].addr)
            {
                // data may exit, continue
                try = (try + 1) % HASH_TABLE_SIZE;
            }
            else
            {
                assert(0 && "something is wrong");
            }
        }

    }
    while (try != index);

    return IDX_LAST;
}


uint16_t delete(const char* label)
{
    // return the deleted address
    return 0;
}


void print_hashmap()
{
    for (uint16_t i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (hashmap[i].addr == SLOT_UNUSED)
        {
            printf("hashmap[%d]\t-----\n", i);
        }
        else if (hashmap[i].addr == SLOT_DELETED)
        {
            printf("hashmap[%d]\tdeleted\n", i);
        }
        else
        {
            printf("hashmap[%d]\t { label: %s \t addr: 0x%x }\n", i, hashmap[i].label, hashmap[i].addr);
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
        "stranger_variable_name",
        "aabbcc",
        "abcabc",
        "ccbbaa"
    };


    for (int i = 1; i < 20; i++)
    {
        // Data tmp =
        // {
        //     .label = labels[i];
        //     .addr = labels[i];
        // };
        insert(labels[i], (uint16_t)i);
        // printf("inserting... %s 0x%x \n", labels[i], i);
    }

    printf("\n\n");

    print_hashmap();

    printf("\n\n");
    find("main");
    find("ainm");

    printf("\n\n");
    printf("sizeof Data = %d\n", sizeof(Data));
    int table_size = HASH_TABLE_SIZE;
    printf("sizeof table = %d\n", table_size);

}