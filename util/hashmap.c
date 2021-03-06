#include "hashmap.h"

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
#if DEBUG_MODE
            printf("collision for { %s\t0x%x } at [%d] \n", label, addr, try);
#endif // DEBUG_MODE
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
#if DEBUG_MODE
            printf("label %s not found \n", label);
#endif // DEBUG_MODE
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
#if DEBUG_MODE
            printf("label %s not found \n", label);
#endif // DEBUG_MODE
            break;
        }
        else
        {
            int not_match = strncmp(self->_data[try].label, label, strlen(label));
            if (0 == not_match)
            {
                // found
#if DEBUG_MODE
                printf("deleting { label: %s:\taddr: 0x%x }\n", label, self->_data[try].addr);
#endif // DEBUG_MODE
                const char* tmp = self->_data[try].label;
                free((void*)tmp);
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

static void _clear(Hashmap* self)
{
    assert(self != NULL && "invalid hashmap");
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (NULL != self->_data[i].label)
        {
            free((void*)self->_data[i].label);
            self->_data[i].label = NULL;
        }
    }
}

void _print_hashmap(Hashmap* self)
{
    printf("\n\n----------------- HASHMAP START --------------- \n\n");
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
    printf("\n\n----------------- HASHMAP END --------------- \n\n");
}

void hashmap_init(Hashmap* self, const Memory* ptr_memory)
{
    self->find = &_find;
    self->insert = &_insert;
    self->delete = &_delete;
    self->clear = &_clear;
    self->_data = ptr_memory->reserved;
}

#if DEBUG_MODE
void hashmap_test(Hashmap* self)
{
    /* for testing hashmap */
    printf("\n\n");
    _print_hashmap(self);
    printf("\n\n");
}
#endif // DEBUG_MODE