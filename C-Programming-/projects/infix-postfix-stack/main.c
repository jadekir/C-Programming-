#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char *array;
    int top;
    int maxSize;
} Stack;

// Function prototypes
void initStack(Stack *stack, int size);
void push(Stack *stack, char item);
char pop(Stack *stack);
int isEmpty(Stack *stack);
int isParentheses(char ch);
int checkBalance(char exp[]);
int priority(char ch);
int isOperator(char ch);
char *infixToPostfix(char infix[]);

int main() {
    char expression[100];

    printf("Enter Expression: ");
    fgets(expression, sizeof(expression), stdin);
    expression[strcspn(expression, "\n")] = 0; // Remove newline character

    printf("Your input expression: %s\n", expression);

    printf("Checking balance...\n");
    if (checkBalance(expression)) {
        printf("VALID\n");
        char *postfix = infixToPostfix(expression);
        printf("The postfix is: %s\n", postfix);
        free(postfix);
    } else {
        printf("INVALID\n");
    }

    return 0;
}

// Stack functions
void initStack(Stack *stack, int size) {
    stack->maxSize = size;
    stack->array = (char *)malloc(size * sizeof(char));
    stack->top = -1;
}

void push(Stack *stack, char item) {
    stack->array[++stack->top] = item;
}

char pop(Stack *stack) {
    return stack->array[stack->top--];
}

int isEmpty(Stack *stack) {
    return stack->top == -1;
}

// Parentheses checker
int isParentheses(char ch) {
    return ch == '(' || ch == ')' || ch == '{' || ch == '}' || ch == '[' || ch == ']';
}

int checkBalance(char exp[]) {
    Stack stack;
    initStack(&stack, strlen(exp));

    for (int i = 0; exp[i]; i++) {
        if (exp[i] == '(' || exp[i] == '{' || exp[i] == '[') {
            push(&stack, exp[i]);
        } else if (exp[i] == ')' || exp[i] == '}' || exp[i] == ']') {
            if (isEmpty(&stack)) {
                free(stack.array);
                return 0; // Stack is empty
            }
            char top = pop(&stack);
            if ((exp[i] == ')' && top != '(') ||
                (exp[i] == '}' && top != '{') ||
                (exp[i] == ']' && top != '[')) {
                free(stack.array);
                return 0; // Mismatch found
            }
        }
    }

    int isBalanced = isEmpty(&stack); // If stack is empty, it's balanced
    free(stack.array);
    return isBalanced;
}

// Operator priority
int priority(char ch) {
    switch (ch) {
        case '+':
        case '-':
            return 1;
        case '*':
        case '/':
            return 2;
        case '^':
            return 3;
        default:
            return 0;
    }
}

// Check if character is an operator
int isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

// Infix to postfix conversion
char *infixToPostfix(char infix[]) {
    Stack stack;
    initStack(&stack, strlen(infix));

    char *postfix = (char *)malloc(2 * stack.maxSize * sizeof(char)); // space for postfix
    int j = 0;

    for (int i = 0; infix[i]; i++) {
        if (isalnum(infix[i])) {
            postfix[j++] = infix[i];
            postfix[j++] = ' '; // space between operands
        } else if (isOperator(infix[i])) {
            while (!isEmpty(&stack) && priority(stack.array[stack.top]) >= priority(infix[i])) {
                postfix[j++] = pop(&stack);
                postfix[j++] = ' ';
            }
            push(&stack, infix[i]);
        } else if (isParentheses(infix[i])) {
            if (infix[i] == '(') {
                push(&stack, infix[i]);
            } else {
                while (!isEmpty(&stack) && stack.array[stack.top] != '(') {
                    postfix[j++] = pop(&stack);
                    postfix[j++] = ' ';
                }
                pop(&stack); // pop '('
            }
        }
    }

    while (!isEmpty(&stack)) {
        postfix[j++] = pop(&stack);
        postfix[j++] = ' ';
    }
    postfix[j] = '\0'; // Null-terminate the string
    free(stack.array);
    return postfix;
}
