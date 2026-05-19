# Checksum Calculator

This program calculates a checksum for a text file using 8-bit, 16-bit, or 32-bit blocks.

I included it because it connects C programming with security and networking concepts. The main idea is checking whether data has changed by creating a smaller value from the file contents.

Run:

```sh
gcc -o checksum main.c
./checksum input.txt 16
```

