#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define IDX_BASE            0xfbff
#define IDX_LAST            0xffff
#define SIZE_OF_DATA        8                                           // this will only work with size_t = 4 byte
#define HASH_TABLE_SIZE     ( ( IDX_LAST - IDX_BASE ) / SIZE_OF_DATA )  // 128 slots
#define INVALID_INDEX       HASH_TABLE_SIZE                             // use table size as marker for invalid index
#define SLOT_UNUSED         0x0000
#define SLOT_DELETED        IDX_LAST                                    // use last index as marker for deleted slot
#define PRIME_TABLE_SIZE    25

#define DEBUG_MODE         1

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

Data _hashmap[HASH_TABLE_SIZE] = {};

/*
 * return hash table index from label.
 * if label is NULL, return INVALID_INDEX
 */
int _hash(const char* label)
{
    int index = INVALID_INDEX;
    if (NULL == label)
    {
        assert(label != NULL && "invalid null label");
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

/*
 * insert label:addr into hashmap
 * return int error status
 */
int _insert(const char* label, uint16_t addr)
{
    int index = _hash(label);
    if (INVALID_INDEX == index)
    {
        return 1; // error
    }

    int try = index;

    do
    {
        if (SLOT_UNUSED != _hashmap[try].addr && SLOT_DELETED != _hashmap[try].addr)
        {
            // collision handling - open addressing (try next slot)
            printf("collision for { %s\t0x%x } at [%d] \n", label, addr, try);
            try = (try + 1) % HASH_TABLE_SIZE;
            continue;
        }
        else
        {
            // insert addr in hashmap at index
            _hashmap[try].label = label;
            _hashmap[try].addr = addr;
            return 0; // status ok
        }
    }
    while(try != index);

    return 2; // error - table full
}

/*
 * return the address found.
 * if not found, return IDX_LAST
 */
uint16_t _find(const char* label)
{
    uint16_t addr = IDX_LAST;
    if (NULL == label)
    {
        assert(label != NULL && "invalid label");
        return addr;
    }

    int index = _hash(label);
    int try = index;
    int not_match = 0;

    do
    {
        if (NULL == _hashmap[try].label || SLOT_UNUSED == _hashmap[try].addr)
        {
            // label does not exist, stop looking
            printf("label %s not found \n", label);
            break;
        }
        else
        {
            not_match = strncmp(_hashmap[try].label, label, strlen(label));
            if (0 == not_match)
            {
                // found
                printf("found label %s \n", label);
                return _hashmap[try].addr;
            }
            else if (not_match && SLOT_DELETED == _hashmap[try].addr)
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

/*
 * delete the {label:addr} and marked slot as deleted
 * return the deleted addr
 * if label not found, return SLOT_DELETED
 */
uint16_t _delete(const char* label)
{
    uint16_t addr = SLOT_DELETED;
    if (NULL == label)
    {
        assert(label != NULL && "invalid label");
        return addr;
    }

    int index = _hash(label);
    int try = index;

    do
    {
        if (NULL == _hashmap[try].label || SLOT_UNUSED == _hashmap[try].addr)
        {
            // label does not exist, stop looking
            printf("label %s not found \n", label);
            break;
        }
        else
        {
            int not_match = strncmp(_hashmap[try].label, label, strlen(label));
            if (0 == not_match)
            {
                // found
                printf("deleting { label: %s:\taddr: 0x%x }\n", label, _hashmap[try].addr);
                addr = _hashmap[try].addr;
                _hashmap[try].label = NULL;
                _hashmap[try].addr = SLOT_DELETED;
                break;
            }
            else if (not_match && SLOT_DELETED == _hashmap[try].addr)
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

    return addr;
}

void _print_hashmap()
{
    for (uint16_t i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (_hashmap[i].addr == SLOT_UNUSED)
        {
            printf("hashmap[%d]\t-----\n", i);
        }
        else if (_hashmap[i].addr == SLOT_DELETED)
        {
            printf("hashmap[%d]\tdeleted\n", i);
        }
        else
        {
            printf("hashmap[%d]\t { label: %s \t addr: 0x%x }\n", i, _hashmap[i].label, _hashmap[i].addr);
        }
    }
}



/*
    typedef struct hashmap
    {
        void (*init)(const void* self);

        ..
        hashmap data;
    } Hashmap;
*/

#if DEBUG_MODE
int main()
{

    char* labels[18] =
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


    for (int i = 1; i < 18; i++)
    {
        _insert(labels[i], (uint16_t)i);
        // printf("inserting... %s 0x%x \n", labels[i], i);
    }

    printf("\n\n");

    _print_hashmap();

    printf("\n\n");
    _find("main");
    _find("ainm");

    printf("\n\n");
    _delete("main");
    _delete("ainm");
    _delete("a");
    _delete("b");
    _delete("c");

    _insert("main", 0xff);

    printf("\n\n");
    _print_hashmap();

    printf("\n\n");
    printf("sizeof Data = %d\n", sizeof(Data));
    int table_size = HASH_TABLE_SIZE;
    printf("sizeof table = %d\n", table_size);

}

#endif // DEBUG_MODE