# Recursive Garage Pairing

This program pairs garages based on coordinate distance and searches for the lowest total distance.

The main idea is recursion/backtracking: try possible pairings, track the best result, and avoid reusing garages that are already paired.

Run:

```sh
gcc -o garage_pairing main.c -lm
./garage_pairing
```

