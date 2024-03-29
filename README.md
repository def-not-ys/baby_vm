# baby_vm
baby_vm is a virtual machine (VM) that implements [one-instruction set computer (OISC)](https://en.wikipedia.org/wiki/One-instruction_set_computer).

**baby_vm is still under construction. The to-dos are listed below:**

## TO-DO
### Features 
#### Execution
- [x] Add return value address
- [x] add capability to display return value
#### Add compiler in racket
- [ ] define bnf of the input language
    - refer to simple math-lan in 311
- [ ] design intermediate languages
- [ ] design compiler
### General
- [x] add `Dockerfile` and complete the *How to run* section
- [x] abstraction for memory address - added decode layer to turn relative index to memory data region address
- [x] add support for flags 
- [ ] implement stack memory support
- [ ] implement heap memory support
- [ ] implement syscall assmebly support
- [ ] keep updating the documentation
### Testing 
- [x] supply simple assembly program file for testing
- [x] Add test suite to test machine functionality
- [ ] convert simple c program to one-line instruction assembly code
    - maybe simple additions 
- [ ] test converted assembly code
- [?] add testing input example to print "hello world" - ? this requires IO capacity? 


## General information
Inspired by [this tutorial](https://justinmeiners.github.io/lc3-vm/) for writing your own vm, I got the idea of writing a simple baby vm that only runs one instruction and see how far I can get.

The one instruction that runs on `baby_vm` is `SUBLEQ`
```
    Instruction subleq a, b, c
        Mem[b] = Mem[b] - Mem[a]
        if (Mem[b] ≤ 0)
            goto c
```

A simple ISA around `SUBLEQ` was also deigned for `baby_vm` to interpret the assmebly files.

`baby_vm` has its own memory and hardware models.

All design deatils are documented in `/doc/DOCUMENTATION.md`.

This project is for fun and educational purpose, not meant to be useful. :)

## How to run

By the time this is publish, I should have dockerize `baby_vm`. So the following steps should get `baby_vm` up and running:

1) make sure `docker` is installed.
* `docker --version` will show if docker is installed.

2) from the project directory, build docker
* `cd baby_vm/`
* `docker build --no-cache -t baby_vm .`

3) run docker
* `docker run -it --tty  baby_vm:latest`

4) `baby_vm` will propmt you to enter the assmebly files that you want to run, there are some example files supplied in `baby_vm/examples/`, enter the file name with path relative to the project directory. for example:
* `examples/test.txt`
* if you want to run your own assembly file (please refer to `doc/DOCUMENTATION.md` for ISA), make sure enter the file name **with the path relative to the project directory**.

5) watch `baby_vm` run :)

## Anything else?

contributions welcome :)
