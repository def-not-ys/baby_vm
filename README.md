## HARDWARE

### MEMORY

16 bit (for simplicity) - maybe 8 bit if pi cannot handle 16 bit

### RESGITERS

The LC-3 has 10 total registers, each of which is 16 bits. Most of them are general purpose, but a few have designated roles.

~~8 general purpose registers (R0-R7)~~

1 program counter (PC) register

1 condition flags (COND) register

### INSTRUCTION SET

typical basic instructions includes:

- load
- store
- add
- bitwise and
- bitwise not
- branch
- jump
- ...

~~need to implement `HALT` as well ??~~

#### try one-instruction set computer

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

~~HALT 	0x0000 - 2-byte instruction~~

SUBLEQ 	0x4000 - 4-byte instruction

**@TODO: memory need to be revisited may need more text space!**

address 0x0fff reserved for HALT -> bytes reserved 0x0fff / 2 = 510 = 170 * 3 -> (OP) a b c 0 * 2 + 6 * 2 = 12 bytes instruction can be aligned



##### some special values for branching
+ 	0
+ 	1
### CONDITIONAL FLAGS


Each CPU has a variety of condition flags to signal various situations. The LC-3 uses only 3 condition flags which indicate the sign of the previous calculation.


```
{Includes, 12}

{Registers, 3}
{Opcodes, 3}
{Condition Flags, 3}
```


## MODELLING

### MEMORY


* text region for loading the instructions
* user region for stack and data
* reserved region for special values

* program stack...?

use HASHMAP to store label-address pair

turn HASHMAP into an object???? ~~Polymorphism in C~~

~~compile hashmap as so to link at rt ???~~


originally memory is modelled as `uint16_t memory[UINT16_MAX]`. may want to turn the array into struct/union for better data type access.
as data is stored as 8-byte { label:addr } hashmap data and text is stored as plain 6 byte instruction (3 of 2-byte address).

using struct to define pointer pointing to different area of memory array???


potential memory struct
```
uint16_t _memory[UINT16_MAX]

typedef struct
{
	Data* reserved[HASH_TABLE_SIZE]; // pointer to the reserved data region for hashmap
	uint16_t* ptr_stack; // pointer to the start of stack region
	uint16_t* ptr_heap; // pointer to the start of the heap region
	uint16_t* ptr_data; // pointer to the user data region
	Instruction* ptr_text; // pointer to the text region

} memory;

```

### PROGRAM

1. load and process read file of assembly instructions (.txt file)
	- process labels (where branch can jump to)
	- save labels in memory
	- set the start point to the start of the instructions
	- mark the end of the instructions
2. process instructions (while `running`)


## OTHER THOUGHTS
* consider running automate end to end test .