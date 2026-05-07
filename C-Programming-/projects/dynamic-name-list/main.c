//COP3223 Fall 2023 Assignment 3.1
//Copyright 2023 Kirkpatrick Jade

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void expandArray(char ***arr, int *size) {
    *size *= 2;
    *arr = realloc(*arr, *size * sizeof(char *));
}

void printList(char **arr, int count) {
    printf("List has %d name(s):\n", count);
    for (int i = 0; i < count; i++) {
        printf("%s\n", arr[i]);
    }
}

int findName(char **arr, int count, char *target) {
    for (int i = 0; i < count; i++) {
        if (strcmp(arr[i], target) == 0) {
            return i;
        }
    }
    return -1;
}

void removeName(char ***arr, int *count, char *target) {
    int index = findName(*arr, *count, target);
    if (index != -1) {
        free((*arr)[index]);
        for (int i = index; i < *count - 1; i++) {
            (*arr)[i] = (*arr)[i + 1];
        }
        (*count)--;
        *arr = realloc(*arr, *count * sizeof(char *));
    }
}

int main() {
    char **names = NULL;
    int size = 4;
    int count = 0;

    names = (char **)malloc(size * sizeof(char *));

    while (1) {
        char input[50];
        printf("Please enter the name: ");
        scanf("%s", input);

        if (strcmp(input, "done") == 0) {
            break;
        }

        names[count] = strdup(input);
        count++;

        if (count == size) {
            expandArray(&names, &size);
        }

        printList(names, count);
    }

    char search_name[50];
    printf("Please enter the name you want to find: ");
    scanf("%s", search_name);
    int index = findName(names, count, search_name);
    if (index != -1) {
        printf("Find name at index %d\n", index);
    } else {
        printf("Name not found in the list.\n");
    }

    char remove_name[50];
    printf("What name do you want to remove: ");
    scanf("%s", remove_name);
    removeName(&names, &count, remove_name);

    printList(names, count);

    // Free allocated memory
    for (int i = 0; i < count; i++) {
        free(names[i]);
    }
    free(names);

    return 0;
}
