#include <stdio.h>
#include <stdlib.h>
#define INPUT_FILE_NAME "pa4_data.txt"
// COP 3502C Spring 2025
// Student Name: Jade Kirkpatrick
// File Name: pa4_kirkpatrick.c
// Note: Rename to use your actual surname. File name should be in lowercase.
// You must remove any whitespaces or dashes from your surname.
// Note: **do not** modify or remove TODO comments
// DO NOT MODIFY THIS PORTION
// function prototypes defined here
unsigned long get_the_element(unsigned long *, unsigned long);
// You may define other helper functions if you want
int compare(const void *a, const void *b) {
    unsigned long val1 = *(const unsigned long *)a;
    unsigned long val2 = *(const unsigned long *)b;
    if (val1 < val2) return -1;
    if (val1 > val2) return 1;
    return 0;
}
// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION
int main(void) {
FILE *input_file = fopen(INPUT_FILE_NAME, "r");
// check if the file was opened
if(input_file == NULL) {
// if there was a problem, simply return a 1
printf("There was a problem opening the file. Abort!\n");
return 1;
}
// the first line of the input file indicates
// how many numbers to read from the file
// you can assume that this will be an odd
// number; note that this can be a very big
// number to represent a very big list
unsigned long count;
fscanf(input_file, "%lu", &count);
// dynamically allocate this array of this size
unsigned long *list = malloc(sizeof(unsigned long) * count);
unsigned long i = 0;
// read the remaining numbers and
// store them in the array
unsigned long num;
while( fscanf(input_file, "%lu", &num) != EOF ) {
list[i++] = num;
}
// close the file
fclose(input_file);
// output the element we are looking for
printf("%lu\n", get_the_element(list, count));
return 0;
}
// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION
unsigned long get_the_element(unsigned long *arr, unsigned long size) {
// TODO START
// TODO: Return the ((n+1)/2)-th highest element
// of an odd-length list. Assume that all the numbers
// in the list are distinct.

    qsort(arr, size, sizeof(unsigned long), compare);
    return arr[(size + 1) / 2 - 1];

// TODO END
}
