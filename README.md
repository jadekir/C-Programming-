# Systems Programming in C

A collection of C programs from systems programming and data structures practice. This repository shows command-line development, manual memory management, file I/O, string parsing, bit operations, recursion, linked data structures, trees, stacks, queues, and debugging with compiler warnings enabled.

## What This Demonstrates

- Writing C programs that compile and run from the command line
- Breaking larger problems into functions and smaller pieces of logic
- Practicing pointers, arrays, structs, linked data structures, recursion, and file input
- Using a Makefile and Git-based workflow
- Documenting projects so another developer can understand and run them

## Featured Projects

| Project | Focus |
| --- | --- |
| `src/airline_reservation.c` | Menu-driven seat reservation system with passenger search, cancellation, and boarding-pass style output |
| `src/bit_tool.c` | Bit manipulation utility for masks, shifts, and binary display |
| `src/word_count.c` | File parser that counts lines, words, characters, and longest token |
| `C-Programming-/projects/linked-list-card-game` | Larger linked-list program with game state and dynamic data handling |
| `C-Programming-/projects/mips-alu-simulator` | MIPS-style ALU/simulator practice tied to computer architecture concepts |
| `C-Programming-/projects/infix-postfix-stack` | Stack-based expression conversion and parsing |
| `C-Programming-/projects/trie-prefix-counter` | Trie data structure for prefix counting |
| `C-Programming-/projects/bst-level-sum` | Binary search tree traversal and level-based aggregation |

## Repository Structure

```text
.
|-- README.md
|-- Makefile
|-- src/
|   |-- airline_reservation.c
|   |-- bit_tool.c
|   `-- word_count.c
`-- C-Programming-/projects/
    |-- airline-seat-reservation/
    |-- linked-list-card-game/
    |-- mips-alu-simulator/
    |-- trie-prefix-counter/
    `-- ...
```

## Build

```bash
make
```

## Run Examples

```bash
./build/airline_reservation
./build/bit_tool 42
./build/word_count README.md
```

Individual project folders include their own `README.md` files and source files.

## Skills Practiced

- C programming fundamentals
- Pointers, structs, arrays, strings, and dynamic memory
- File I/O and command-line arguments
- Recursion and algorithmic problem solving
- Linked lists, stacks, queues, binary search trees, and tries
- Bit operations and low-level data representation
- Linux command-line compilation and debugging
- Makefile-based builds

## Notes

This repository is intended as a portfolio-friendly version of coursework and practice programs. I keep code organized by concept and add documentation so the work is readable beyond the original assignment context.
