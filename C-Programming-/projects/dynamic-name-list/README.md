# Dynamic Name List

This program stores names in a dynamically allocated list.

The list grows with `realloc`, supports searching for a name, removes a selected name, and frees the allocated memory before exiting. I added it because it shows dynamic memory in a smaller, easier-to-read program.

Run:

```sh
gcc -o dynamic_names main.c
./dynamic_names
```

