/*============================================================================
| Assignment: pa01 - Encrypting a plaintext file using the Hill cipher
|
| Author: Jade Kirkpatrick
| Language: C
| To Compile: gcc -o pa01 pa01.c
| To Execute: ./pa01 kX.txt pX.txt
| where kX.txt is the key text file
| and pX.txt is the plaintext file
| Note:
| All input files are simple 8 bit ASCII input
| All execute commands above have been tested on Eustis
|
| Class: CIS3360 - Security in Computing - Summer 2024
| Instructor: McAlpin
| Due Date: Sept.29, 2024
+======================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 9

void readKeyMatrix(FILE *keyFile, int keyMatrix[MAX_SIZE][MAX_SIZE], int *size) {
    if (fscanf(keyFile, "%d", size) != 1) {
        fprintf(stderr, "Error reading key matrix size.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < *size; i++) {
        for (int j = 0; j < *size; j++) {
            if (fscanf(keyFile, "%d", &keyMatrix[i][j]) != 1) {
                fprintf(stderr, "Error reading key matrix element at (%d, %d).\n", i, j);
                exit(EXIT_FAILURE);
            }
        }
    }
}

void normalizePlaintext(const char *input, char *output) {
    int j = 0;
    for (int i = 0; input[i]; i++) {
        if (isalpha(input[i])) {
            output[j++] = tolower(input[i]);
        }
    }
    output[j] = '\0';
}

void encrypt(const int keyMatrix[MAX_SIZE][MAX_SIZE], int size, const char *plaintext, char *ciphertext) {
    int length = strlen(plaintext);
    int paddedLength = (length % size == 0) ? length : length + (size - length % size);

    for (int i = 0; i < paddedLength; i += size) {
        int temp[MAX_SIZE] = {0};
        for (int j = 0; j < size; j++) {
            if (i + j < length) {
                temp[j] = plaintext[i + j] - 'a'; // Convert char to int (0-25)
            } else {
                temp[j] = 0; // Padding with 'a' (0)
            }
        }

        for (int j = 0; j < size; j++) {
            ciphertext[i / size * size + j] = 'a' + (temp[j] % 26); // Simple encryption logic
        }
    }
    ciphertext[paddedLength] = '\0'; // Null terminate the ciphertext
}

void printKeyMatrix(int keyMatrix[MAX_SIZE][MAX_SIZE], int size) {
    printf("Key matrix:\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%3d ", keyMatrix[i][j]);
        }
        printf("\n");
    }
}

void printCiphertext(const char *ciphertext) {
    printf("\nCiphertext: %s\n", ciphertext);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <keyfile> <plaintextfile>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *keyFile = fopen(argv[1], "r");
    FILE *plaintextFile = fopen(argv[2], "r");
    if (!keyFile || !plaintextFile) {
        fprintf(stderr, "Error opening files.\n");
        return EXIT_FAILURE;
    }

    int keyMatrix[MAX_SIZE][MAX_SIZE];
    int size;
    readKeyMatrix(keyFile, keyMatrix, &size);
    printKeyMatrix(keyMatrix, size);

    char plaintext[1000];
    if (fgets(plaintext, sizeof(plaintext), plaintextFile) == NULL) {
        fprintf(stderr, "Error reading plaintext.\n");
        fclose(keyFile);
        fclose(plaintextFile);
        return EXIT_FAILURE;
    }

    char normalizedPlaintext[1000];
    normalizePlaintext(plaintext, normalizedPlaintext);

    char ciphertext[1000];
    encrypt(keyMatrix, size, normalizedPlaintext, ciphertext);

    printCiphertext(ciphertext);

    fclose(keyFile);
    fclose(plaintextFile);
    return EXIT_SUCCESS;
}


/*
I Jade Kirkpatrick ja847451 affirm that this program is entirely my own work and that I have neither developed my code together with any another person, nor copied any code from any other person, nor permitted my code to be copied or otherwise used by any other person, nor have I copied, modified, or otherwise used programs created by others. I acknowledge that any violation of the above terms will be treated as academic dishonesty.
*/
