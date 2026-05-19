#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static void print_binary(unsigned int value) {
    for (int i = 31; i >= 0; i--) {
        putchar((value & (1u << i)) ? '1' : '0');
        if (i % 4 == 0 && i != 0) {
            putchar(' ');
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <unsigned integer>\n", argv[0]);
        return 1;
    }

    errno = 0;
    char *end = NULL;
    unsigned long parsed = strtoul(argv[1], &end, 10);
    if (errno != 0 || *end != '\0' || parsed > UINT_MAX) {
        fprintf(stderr, "Invalid unsigned integer: %s\n", argv[1]);
        return 1;
    }

    unsigned int value = (unsigned int) parsed;
    printf("decimal:  %u\n", value);
    printf("hex:      0x%X\n", value);
    printf("binary:   ");
    print_binary(value);
    putchar('\n');
    printf("low byte: 0x%02X\n", value & 0xFFu);
    printf("shift << 1: %u\n", value << 1);
    printf("shift >> 1: %u\n", value >> 1);

    return 0;
}
