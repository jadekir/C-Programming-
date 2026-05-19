# File Tree Line Counter

This is a small systems software project that walks through a file or directory path from the command line.

It can print file paths and, with the `-l` option, count lines in regular files. I added it because it shows UNIX-style file handling in C with directories, file descriptors, and system calls.

Run:

```sh
gcc -o findmy main.c
./findmy path
./findmy -l path
```

