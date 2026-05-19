# Systems Programming in C

Small C programs written to practice systems programming fundamentals: memory management, file I/O, string parsing, arrays, bit operations, and command-line builds on Linux/Unix.

## Projects

- `airline_reservation.c` - menu-driven seat reservation system with passenger search, cancellation, and boarding-pass output.
- `bit_tool.c` - command-line bit utility for masks, shifts, and binary display.
- `word_count.c` - file parser that counts lines, words, characters, and the longest token.

## Build

```bash
make
```

## Run

```bash
./build/airline_reservation
./build/bit_tool 42
./build/word_count README.md
```

## What this demonstrates

- C structs, arrays, and string handling
- Input validation and defensive bounds checks
- Makefile-based compilation with warnings enabled
- Linux command-line workflow and Git version control
