# baby_vm Documentation (under construction)

## TABLE OF CONTENTS
- [baby_vm Documentation (under construction)](#baby_vm-documentation-under-construction)
	- [TABLE OF CONTENTS](#table-of-contents)
	- [HARDWARE](#hardware)
	- [MEMORY](#memory)
		- [TEXT REGION](#text-region)
		- [DATA REGION](#data-region)
		- [HEAP REGION (to be implemented)](#heap-region-to-be-implemented)
		- [STACK REGION (to be implemented)](#stack-region-to-be-implemented)
		- [RESERVED REGION](#reserved-region)
			- [MEMORY HASHMAP MODEL](#memory-hashmap-model)
	- [BABY_VM ARCHITECTURE (draft)](#baby_vm-architecture-draft)
	- [PROGRAM](#program)
	- [OTHER THOUGHTS](#other-thoughts)

## HARDWARE

baby_vm's hardware is modeled as single threaded CPU that only executes one instruction `SUBLEQ` (reference: [ons-instruction set computer (OISC)](https://en.wikipedia.org/wiki/One-instruction_set_computer)).

```
    Instruction subleq a, b, c
        Mem[b] = Mem[b] - Mem[a]
        if (Mem[b] ≤ 0)
            goto c
```
Registers are not required for this hardware model, the instruction operates directly on memory.

## MEMORY

Memory is modeled using an `uint16_t` array. Index of the array is interpreted as baby_vm's memory address. For example, address 0x0010 maps to `_memory[0x0010]`.

```
// src/memory.c

static uint16_t _memory[UINT16_MAX]; // 65535 (2^16) addressable location
```

Referenced by the C program memory layout, baby_vm's memory layout is inllusrated below (NOT TO SCALE)
```
// src/memory.h

    0xffff   +----------+
             | Reserved |   1024 bytes
             |          |
    0xfbff   +----------+
             |          |
             |  Stack   |
             |          |
             +----+-----+
             |    |     |
             |    v     |
             |          |
             |    ^     |
             |    |     |
             +----+-----+
             |   Heap   |
             |          |
    0x1400   +----------+
             |   Data   |   1024 bytes
             |          |
    0x1000   +----------+
             |          |
             |   Text   |
             |          |
    0x0000   +----------+
```

The `Memory` structure exposed to program to interact with memory is defined as:
```
// src/memory.h

typedef struct
{
	Data*           reserved;       // pointer to the reserved data region for hashmap
	uint16_t        addr_stack;     // addr to the start of stack region
	uint16_t        addr_heap;      // addr to the start of the heap region
	uint16_t        addr_data;      // addr to the user data region
	Instruction*    ptr_text;       // pointer to the text region
} Memory;

```

### TEXT REGION

Instructions from the assmebly files are loaded and stored in the text region of the memory map. The general format of the instruction is:
```
subleq a b c
```
where a, b , and c are memory address.

Since `subleq` is the only opcode, the structure of instructions can be defined as:
```
// src/memory.h

typedef struct
{
    //uint16_t      opcode; (omit)
    uint16_t        src;
    uint16_t        dest;
    uint16_t        brch;
} Instruction;
```
which adds up to be a 6-byte struct.

The text region in memory is interpreted as an array of `Instruction`. The first slot (`_memory[0x0000] - _memory[0x0003]`) in the text region is reserved.

### DATA REGION

Initialized user data from the assembly files is stored in the data region of the memory map.

The frist 4 slots (`_memory[0x1000] - _memory[0x1004]`) are reserved for special values that are used by the program. User data is stored in the rest of the data region.
```
_memory[0x1000] = 0x00 // ZERO
_memory[0x1001] = 0x01 // ONE
_memory[0x1002] = 0xff // RESERVED
_memory[0x1003] = 0xff // RESERVED

```

### HEAP REGION (to be implemented)
Memory region for dynamically allocated memory.

### STACK REGION (to be implemented)
Memory region for stack frame.

### RESERVED REGION
#### MEMORY HASHMAP MODEL

Functions and data can be assigned unique labels for memory address look up. To faciliate the look up, baby_vm stores the memory address and its associate label in a `{ label : addr }` key : value pair.
```
// src/memory.h

typedef struct
{
    const char*     label;
    uint16_t        reserved;   // for alignment
    uint16_t        addr;
} Data;
```

`{ label : addr}` pairs are stored in a Hashmap which is an internal data structure of baby_vm's memory.
```
// util/hahsmap.h

typedef struct _hashmap
{
    uint16_t        (*find)(struct _hashmap* self, const char* label);
    HashmapStatus   (*insert)(struct _hashmap* self, const char* label, uint16_t addr);
    uint16_t        (*delete)(struct _hashmap* self, const char* label);
    void            (*clear)(struct _hashmap* self);
    Data*           _data;
} Hashmap;
```
This internal Hashmap is stored in the reserved region of the memory map.

## BABY_VM ARCHITECTURE (draft)

`SUBLEQ`

* subtract AND branch if less then or equal to zero
	* example: a, b, c, are memory address
```
subleq a, b, c
# subtract mem[a] from value in mem[b], jump to c if mem[b] <= 0
```
* works directly on memory (no registers required)

each \ _ _ \ = 2 bytes

 \ _ _ \ _ _ \ _ _ \ _ _ \ _ _ \

 \ OP \ a \ b \ c \

opcode addr.a addr.b addr.c

address 0x0fff reserved for HALT -> bytes reserved 0x0fff

some special values for branching
+ 	0
+ 	1


## PROGRAM

1. load and process read file of assembly instructions (.txt file)
	- process labels (where branch can jump to)
	- save labels in memory
	- set the start point to the start of the instructions
	- mark the end of the instructions
2. process instructions


## OTHER THOUGHTS
