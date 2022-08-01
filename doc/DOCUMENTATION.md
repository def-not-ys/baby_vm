# baby_vm Documentation (under construction)

## TABLE OF CONTENTS
- [baby_vm Documentation (under construction)](#baby_vm-documentation-under-construction)
  - [TABLE OF CONTENTS](#table-of-contents)
  - [BABY_VM ARCHITECTURE (draft)](#baby_vm-architecture-draft)
    - [HARDWARE](#hardware)
    - [MEMORY](#memory)
      - [TEXT REGION](#text-region)
      - [DATA REGION](#data-region)
      - [HEAP REGION (to be implemented)](#heap-region-to-be-implemented)
      - [STACK REGION (to be implemented)](#stack-region-to-be-implemented)
      - [RESERVED REGION](#reserved-region)
        - [MEMORY HASHMAP MODEL](#memory-hashmap-model)
  - [PROGRAM (draft)](#program-draft)
  - [OTHER THOUGHTS](#other-thoughts)

## BABY_VM ARCHITECTURE (draft)

baby_vm's simulates a [ons-instruction set computer (OISC)](https://en.wikipedia.org/wiki/One-instruction_set_computer)). The single instruction taht runs on baby_vm is `SUBLEQ`.

```
    Instruction subleq a, b, c
        Mem[b] = Mem[b] - Mem[a]
        if (Mem[b] ≤ 0)
            goto c
```
### HARDWARE

baby_vm's hardware is modeled as single threaded CPU. Each instruction is execurted in a serial fashion.
Registers are not required for this hardware model, the instruction operates directly on memory.

To faciliate the single instrustion program execution, sepcific values are reserved for special operations.
```
HALT : subleq ZERO ZERO 0x0fff
```
**address 0x0fff is resereved for `HALT`**

### MEMORY

Memory is modeled using an `uint16_t` array. Index of the array is interpreted as baby_vm's memory address. For example, address 0x0010 maps to `_memory[0x0010]`.

```
static uint16_t _memory[UINT16_MAX]; // 65535 (2^16) addressable location
```

Referenced by the C program memory layout, baby_vm's memory layout is inllusrated below (NOT TO SCALE)
```
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
typedef struct
{
	Data*           reserved;       // pointer to the reserved data region for hashmap
	uint16_t        addr_stack;     // addr to the start of stack region
	uint16_t        addr_heap;      // addr to the start of the heap region
	uint16_t        addr_data;      // addr to the user data region
	Instruction*    ptr_text;       // pointer to the text region
} Memory;

```

#### TEXT REGION

Instructions from the assmebly files are loaded and stored in the text region of the memory map. The general format of the instruction is:
```
subleq a b c
```
where a, b , and c are memory address.

Since `subleq` is the only opcode, the structure of instructions can be defined as:
```
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

#### DATA REGION

Initialized user data from the assembly files is stored in the data region of the memory map.

The frist 4 slots (`_memory[0x1000] - _memory[0x1004]`) are reserved for special values that are used by the program. User data is stored in the rest of the data region.
```
_memory[0x1000] = 0x00 // ZERO
_memory[0x1001] = 0x01 // ONE
_memory[0x1002] = 0xff // RESERVED
_memory[0x1003] = 0x00 // RETURN VALUE

```

#### HEAP REGION (to be implemented)
Memory region for dynamically allocated memory.

#### STACK REGION (to be implemented)
Memory region for stack frame.

#### RESERVED REGION
##### MEMORY HASHMAP MODEL

Functions and data can be assigned unique labels for memory address look up. To faciliate the look up, baby_vm stores the memory address and its associate label in a `{ label : addr }` key : value pair.
```
typedef struct
{
    const char*     label;
    uint16_t        reserved;   // for alignment
    uint16_t        addr;
} Data;
```

`{ label : addr}` pairs are stored in a Hashmap which is an internal data structure of baby_vm's memory.
```
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

## PROGRAM (draft)

baby_vm runs the instructions with the following steps:

1. Read assembly `txt` file and load instructions into memory
baby_vm takes assembly instructions in `.txt.` file as input, when reading the file from given file, internal parser will parse the assmebly instructions and load the instructions in memory. Instructions should obey rules below to be qualify as valid inpuy by the parser.

- The assembly file can have a text section and a data section. Each section should be indicated with the `section` tag followed by the section name.
  ```
  section .text
  (program text)
  section. data
  (program data)
  ```
- baby_vm supports labels in the assmebly file. Label can be used as an alias at certain program location. Labels must have `:` to indicate the end of a label.
  ```
  _start: (program text)
  loop:
    (program text)
 ```

2. Process instruction in memory
- each instruction is exucuted in a serial fashion (concurrent process is not supported)
- decode layer is added to convery relative index to the memory address in data region. e.g.
    ```
    subleq 5 10 4095
    ```

is decoded to:

    ```
    subleq mem[0x1005] mem[0x100a] 0x0fff # mem[0x100a] = mem[0x100a] - mem[0x1005] halt if mem[0x100a] <= 0 
    ```

3. Return value in specified memory address
- currently return value is stored in `0x1003`

## OTHER THOUGHTS
- Error handling and recovery
  - invalid instruction will trigger baby_vm to shutdown. (may want a more sophisticated error handling mechanism)
- baby_vm does not have state
