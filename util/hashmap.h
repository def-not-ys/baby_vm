#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../src/memory.h"

#define IDX_BASE            STACK_REGION_START
#define IDX_LAST            0xffff
#define SIZE_OF_DATA        8                                           // this will only work with size_t = 4 byte
#define HASH_TABLE_SIZE     ( ( IDX_LAST - IDX_BASE ) / SIZE_OF_DATA )  // 128 slots
#define INVALID_INDEX       HASH_TABLE_SIZE                             // use table size as marker for invalid index
#define SLOT_UNUSED         0x0000
#define SLOT_DELETED        IDX_LAST                                    // use last index as marker for deleted slot

#define DEBUG_MODE          0

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
    void            (*clear)(struct _hashmap* self);
    Data*           _data;
} Hashmap;

void hashmap_init(Hashmap* self, const Memory* ptr_memory);

#if DEBUG_MODE
void hashmap_test(Hashmap* self);
#endif // DEBUG_MODE

#endif // HASHMAP_H