#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define IDX_BASE            STACK_REGION_START
#define IDX_LAST            0xffff
#define SIZE_OF_DATA        8                                           // this will only work with size_t = 4 byte
#define HASH_TABLE_SIZE     ( ( IDX_LAST - IDX_BASE ) / SIZE_OF_DATA )  // 128 slots
#define INVALID_INDEX       HASH_TABLE_SIZE                             // use table size as marker for invalid index
#define SLOT_UNUSED         0x0000
#define SLOT_DELETED        IDX_LAST                                    // use last index as marker for deleted slot
#define PRIME_TABLE_SIZE    25

#define DEBUG_MODE          0

/* prime numbers from 1 - 100 */
static const uint16_t _PRIME_TABLE[PRIME_TABLE_SIZE] =
{
    2, 3, 5, 7, 11,
    13, 17, 19, 23, 29,
    31, 37, 41, 43, 47,
    53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};

typedef enum
{
    STATUS_OK,
    STATUS_ERR,
    STATUS_FULL
} HashmapStatus;

typedef struct _hashmap
{
    uint16_t        (*find)(struct _hashmap* self, const char* label);
    HashmapStatus   (*insert)(struct _hashmap* self, const char* label, uint16_t addr);
    uint16_t        (*delete)(struct _hashmap* self, const char* label);
    Data*           _data;
} Hashmap;

// static Data _hashmap[HASH_TABLE_SIZE] = {};

/*
 * return hash table index from label.
 * if label is NULL, return INVALID_INDEX
 */
static int _hash(const char* label)
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

    assert(index >= 0 && index < INVALID_INDEX && "index out of bound");

    return index;
}

/*
 * insert label:addr into hashmap
 * return int error status
 */
static HashmapStatus _insert(Hashmap* self, const char* label, uint16_t addr)
{
    int index = _hash(label);
    if (INVALID_INDEX == index)
    {
        return STATUS_ERR; // error
    }

    int try = index;

    do
    {
        if (SLOT_UNUSED != self->_data[try].addr && SLOT_DELETED != self->_data[try].addr)
        {
            // collision handling - open addressing (try next slot)
            printf("collision for { %s\t0x%x } at [%d] \n", label, addr, try);
            try = (try + 1) % HASH_TABLE_SIZE;
            continue;
        }
        else
        {
            // insert addr in hashmap at index
            self->_data[try].label = label;
            self->_data[try].addr = addr;
            return STATUS_OK; // status ok
        }
    }
    while(try != index);

    return STATUS_FULL; // error - table full
}

/*
 * return the address found.
 * if not found, return IDX_LAST
 */
static uint16_t _find(Hashmap* self, const char* label)
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
        if (NULL == self->_data[try].label || SLOT_UNUSED == self->_data[try].addr)
        {
            // label does not exist, stop looking
            printf("label %s not found \n", label);
            break;
        }
        else
        {
            not_match = strncmp(self->_data[try].label, label, strlen(label));
            if (0 == not_match)
            {
                // found
                printf("found label %s \n", label);
                return self->_data[try].addr;
            }
            else if (not_match && SLOT_DELETED == self->_data[try].addr)
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
static uint16_t _delete(Hashmap* self, const char* label)
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
        if (NULL == self->_data[try].label || SLOT_UNUSED == self->_data[try].addr)
        {
            // label does not exist, stop looking
            printf("label %s not found \n", label);
            break;
        }
        else
        {
            int not_match = strncmp(self->_data[try].label, label, strlen(label));
            if (0 == not_match)
            {
                // found
                printf("deleting { label: %s:\taddr: 0x%x }\n", label, self->_data[try].addr);
                addr = self->_data[try].addr;
                self->_data[try].label = NULL;
                self->_data[try].addr = SLOT_DELETED;
                break;
            }
            else if (not_match && SLOT_DELETED == self->_data[try].addr)
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

void _print_hashmap(Hashmap* self)
{
    for (uint16_t i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (self->_data[i].addr == SLOT_UNUSED)
        {
            printf("hashmap[%d]\t-----\n", i);
        }
        else if (self->_data[i].addr == SLOT_DELETED)
        {
            printf("hashmap[%d]\tdeleted\n", i);
        }
        else
        {
            printf("hashmap[%d]\t { label: %s \t addr: 0x%x }\n", i, self->_data[i].label, self->_data[i].addr);
        }
    }
}

void hashmap_init(Hashmap* self, const Memory* ptr_memory)
{
    self->find = &_find;
    self->insert = &_insert;
    self->delete = &_delete;
    self->_data = ptr_memory->reserved;
}

#if DEBUG_MODE
void hashmap_test(Hashmap* self)
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
        self->insert(self, labels[i], (uint16_t)i);
        printf("inserting... %s 0x%x \n", labels[i], i);
    }

    printf("\n\n");

    _print_hashmap(self);

    printf("\n\n");
    self->find(self, "main");
    self->find(self, "ainm");

    printf("\n\n");
    self->delete(self, "main");
    self->delete(self, "ainm");
    self->delete(self, "a");
    self->delete(self, "b");
    self->delete(self, "c");

    self->insert(self, "main", 0xff);

    printf("\n\n");
    _print_hashmap(self);

    printf("\n\n");
    printf("sizeof Hashmap = %lu\n", sizeof(Hashmap));
    int table_size = HASH_TABLE_SIZE;
    printf("sizeof table = %d\n", table_size);

}
#endif // DEBUG_MODE