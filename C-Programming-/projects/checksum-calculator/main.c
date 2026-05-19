/*I Jade Kirkpatrick 5470989 affirm that this program is entirely my own work and that I have neither developed my code with any
another person, nor copied any code from any other person, nor permitted my code to be copied
or otherwise used by any other person, nor have I copied, modified, or otherwise used programs
created by others. I acknowledge that any violation of the above terms will be treated as
academic dishonesty.”*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 80

unsigned long calculate_checksum(const char *buffer, size_t length, int bits, size_t *char_count) {
    unsigned long checksum = 0;
    *char_count = length;

    // Add each character's ASCII value to the checksum
    for (size_t i = 0; i < length; i++) {
        checksum += (unsigned char)buffer[i];
    }

    // Pad the checksum based on the bit size
    if (bits == 8 && length % 1 != 0) {
        checksum += 'X'; // Padding character for 8 bits
    } else if (bits == 16 && length % 2 != 0) {
        checksum += 'X'; // Padding character for 16 bits
    } else if (bits == 32 && length % 4 != 0) {
        checksum += 'X'; // Padding character for 32 bits
    }

    // Mask the checksum to the appropriate size
    if (bits == 8) {
        return checksum & 0xFF;
    } else if (bits == 16) {
        return checksum & 0xFFFF;
    } else {
        return checksum & 0xFFFFFFFF;
    }
}

void print_echoed_input(const char *buffer, size_t length) {
    for (size_t i = 0; i < length; i++) {
        putchar(buffer[i]);
        if ((i + 1) % MAX_LINE_LENGTH == 0) {
            putchar('\n');
        }
    }
    if (length % MAX_LINE_LENGTH != 0) {
        putchar('\n'); // Ensure to print a new line if needed
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <filename> <checksum size>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *filename = argv[1];
    int checkSumSize = atoi(argv[2]);
    if (checkSumSize != 8 && checkSumSize != 16 && checkSumSize != 32) {
        fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }

    // Read the file contents
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(fileSize + 1);
    if (!buffer) {
        perror("Memory allocation failed");
        fclose(file);
        return EXIT_FAILURE;
    }

    fread(buffer, 1, fileSize, file);
    buffer[fileSize] = '\0'; // Null-terminate the string

    // Print the echoed input
    print_echoed_input(buffer, fileSize);

    size_t charCount;
    unsigned long checksum = calculate_checksum(buffer, fileSize, checkSumSize, &charCount);

    // Print the checksum
    printf("%2d bit checksum is %08lx for all %4zu chars\n", checkSumSize, checksum, charCount);

    // Cleanup
    free(buffer);
    fclose(file);
    return EXIT_SUCCESS;
}
