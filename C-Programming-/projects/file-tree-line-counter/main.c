/* myfind.c
   COP-3402 fs project
   Author: Jade Kirkpatrick
*/

#include <fcntl.h>         // O_RDONLY, open
#include <errno.h>         // errno
#include <stdio.h>         // perror, fprintf, printf
#include <dirent.h>        // opendir, readdir, closedir
#include <stdlib.h>        // exit, malloc, free
#include <sys/stat.h>      // stat, struct stat
#include <string.h>        // strlen, strcmp, strcpy, strcat
#include <unistd.h>        // read, close
#include <stdbool.h>       // bool

// Function prototype to avoid implicit declaration
void traverse(const char *curpath, bool countLines);

int main(int argc, char *argv[]) {
    bool countLines = false;
    char *path = NULL;

    // TODO: parse command line arguments
    if (argc == 2) {
        path = argv[1];
    } else if (argc == 3) {
        if (strcmp(argv[1], "-l") != 0) {
            fprintf(stderr, "Invalid option\n");
            exit(EXIT_FAILURE);
        }
        countLines = true;
        path = argv[2];
    } else {
        fprintf(stderr, "Usage: myfind [-l] path\n");
        exit(EXIT_FAILURE);
    }

    traverse(path, countLines);

    return 0;
}

// Function to count the number of lines in a regular file
long count_file_lines(const char *filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        // Skip printing files that cannot be opened
        return -1;
    }

    char buffer[1024];
    ssize_t bytesRead;
    long lines = 0;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytesRead; i++) {
            if (buffer[i] == '\n') {
                lines++;
            }
        }
    }

    close(fd);
    return lines;
}

// Recursive function to traverse directories
void traverse(const char *curpath, bool countLines) {
    struct stat pathStat;
    if (stat(curpath, &pathStat) != 0) {
        // Skip files/directories that cannot be accessed
        return;
    }

    if (S_ISDIR(pathStat.st_mode)) {
        // Directory: print its path
        printf("%s\n", curpath);

        DIR *dir = opendir(curpath);
        if (!dir) return;

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            // Skip "." and ".."
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;

            // Build new path
            int concatlen = strlen(curpath) + 1 + strlen(entry->d_name) + 1;
            char *newpath = malloc(concatlen);
            if (!newpath) {
                perror("malloc");
                closedir(dir);
                exit(EXIT_FAILURE);
            }
            snprintf(newpath, concatlen, "%s/%s", curpath, entry->d_name);

            traverse(newpath, countLines);
            free(newpath);
        }

        closedir(dir);
    } else if (S_ISREG(pathStat.st_mode)) {
        // Regular file
        if (countLines) {
            long lines = count_file_lines(curpath);
            if (lines >= 0)
                printf("%s\t%ld\n", curpath, lines);
        } else {
            printf("%s\n", curpath);
        }
    } else {
        // Other file types
        printf("%s\n", curpath);
    }
}
