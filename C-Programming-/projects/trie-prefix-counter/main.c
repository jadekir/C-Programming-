#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define LETTER_COUNT 26
#define INPUT_FILE_NAME "pa5_data.txt"

// COP 3502C Spring 2025
// Student Name: TODO_Jade_Kirkpatrick
// File Name: pa5_JadeKirkpatrick.c
// Note: Rename to use your actual surname. File name should be in lowercase.
// You must remove any whitespaces or dashes from your surname.
// Note: **do not** modify or remove TODO comments


typedef struct TrieNode_s {
    struct TrieNode_s *children[LETTER_COUNT];
    int is_terminal;
} TrieNode;



// DO NOT MODIFY THIS PORTION
// function prototypes defined here
int max_num_prefix_words(TrieNode *);
TrieNode * create_node();
void destroy_node(TrieNode *);
int get_char_index(char);
void insert(TrieNode *, char *);
void trim_string(char *);



// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION
int main(void) {
    FILE *input_file = fopen(INPUT_FILE_NAME, "r");

    // check if the file was opened
    if(input_file == NULL) {
        // if there was a problem, simply return a 1
        printf("There was a problem opening the file. Abort!\n");
        return 1;
    }

    // dynamically allocate the root node
    // of a trie
    TrieNode *root = create_node();

    char word[256];

    // read all the words from the file
    while( fgets(word, 256, input_file) ) {
        // trim the string
        trim_string(word);

        // insert the word to the trie
        insert(root, word);
    }

    // close the file
    fclose(input_file);

    // output the answer
    printf("%d", max_num_prefix_words(root));

    // deallocate trie
    destroy_node(root);

    return 0;
}
// DO NOT MODIFY THIS PORTION OF THE MAIN FUNCTION



int max_num_prefix_words(TrieNode *node) {
    // Base case: If the node is NULL, return 0.
    if (node == NULL) {
        return 0;
    }

    // Variable to track the maximum number of prefixes
    int max_prefixes = 0;

    // Variable to count the number of prefixes at this node
    int current_prefixes = 0;

    // Check each child node of the current node
    for (int i = 0; i < LETTER_COUNT; i++) {
        if (node->children[i] != NULL) {
            // Recursively get the number of prefixes in the child nodes
            current_prefixes = max_num_prefix_words(node->children[i]);
            
            // Update max_prefixes if the current child node has more prefixes
            if (current_prefixes > max_prefixes) {
                max_prefixes = current_prefixes;
            }
        }
    }

    // If this node is a terminal node (a word ends here), count it as a prefix
    if (node->is_terminal) {
        max_prefixes++;
    }

    return max_prefixes;
}


TrieNode * create_node() {
    // dynamically create a new node
    TrieNode *node = malloc(sizeof(TrieNode));

    // set all children pointers to NULL
    for(int i = 0; i < LETTER_COUNT; i++) {
        node->children[i] = NULL;
    }

    // set the flag to false
    // indicating that this is not
    // a terminal node for a certain string
    node->is_terminal = 0;

    return node;
}


void destroy_node(TrieNode *node) {
    // this is a postorder traversal
    // visit all the children first
    // then visit the current node
    // base case
    if(node == NULL)
        return;

    // postorder visit all the children
    // and individually destroy them
    for(int i = 0; i < LETTER_COUNT; i++) {
        destroy_node(node->children[i]);
    }

    // deallocate this current node
    free(node);
}


int get_char_index(char alpha) {
    // change to lowercase first
    alpha = tolower(alpha);

    return alpha-'a';
}


void insert(TrieNode *root, char *str) {
    // create a pointer to the root
    TrieNode *ptr = root;

    // traverse through each character
    // of the string
    int len = strlen(str);

    for(int i = 0; i < len; i++) {
        int idx = get_char_index(str[i]);

        // check if there is no existing
        // node for this letter
        if(ptr->children[idx] == NULL) {
            // create a new node for that
            // letter
            ptr->children[idx] = create_node();
        }

        // traverse to the next node
        ptr = ptr->children[idx];
    }

    // once we reached the last letter
    // ptr is pointing at the last node
    // therefore, set the flag of this
    // node to 1 or true
    ptr->is_terminal = 1;
}


void trim_string(char *str) {
    // helper function that will remove the extra new line
    // begin at location right before
    // the null terminator of the string
    int len = strlen(str) - 1;

    // from the right, keep moving to the left;
    // continue doing so until what you see right
    // now is not a newline character anymore
    while(len > 0) {
        // stop moving to the left once you don't
        // see a newline character anymore
        if(str[len] != '\n' && str[len] != '\r')
            break;

        len--;
    }

    // len is currently pointing at the
    // last character right before a newline
    // therefore, it should be immediately
    // followed by the null character
    str[len+1] = '\0';
}
