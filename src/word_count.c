#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define TOKEN_LIMIT 128

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Could not open file");
        return 1;
    }

    long lines = 0;
    long words = 0;
    long chars = 0;
    int in_word = 0;
    char token[TOKEN_LIMIT] = "";
    char longest[TOKEN_LIMIT] = "";
    size_t token_len = 0;

    for (int ch = fgetc(file); ch != EOF; ch = fgetc(file)) {
        chars++;
        if (ch == '\n') {
            lines++;
        }

        if (isspace(ch)) {
            if (in_word) {
                words++;
                token[token_len] = '\0';
                if (strlen(token) > strlen(longest)) {
                    strcpy(longest, token);
                }
                token_len = 0;
            }
            in_word = 0;
        } else {
            in_word = 1;
            if (token_len + 1 < TOKEN_LIMIT) {
                token[token_len++] = (char) ch;
            }
        }
    }

    if (in_word) {
        words++;
        token[token_len] = '\0';
        if (strlen(token) > strlen(longest)) {
            strcpy(longest, token);
        }
    }

    fclose(file);

    printf("lines:   %ld\n", lines);
    printf("words:   %ld\n", words);
    printf("chars:   %ld\n", chars);
    printf("longest: %s\n", longest);

    return 0;
}
