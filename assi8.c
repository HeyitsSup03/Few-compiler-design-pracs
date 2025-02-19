#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// Operator precedence table
int precedence(char op) {
    switch (op) {
        case '+':
        case '-':
            return 1; // Lowest precedence
        case '*':
        case '/':
            return 2;
        case '^':
            return 3; // Exponentiation (right-associative)
        case '(':
        case ')':
            return 4; // Highest precedence (parentheses)
        default:
            return -1; // Invalid operator
    }
}

// Stack structure for operators and operands
typedef struct {
    char data[MAX][MAX];
    int top;
} Stack;

void initStack(Stack *s) {
    s->top = -1;
}

int isEmpty(Stack *s) {
    return s->top == -1;
}

int isFull(Stack *s) {
    return s->top == MAX - 1;
}

void push(Stack *s, char *str) {
    if (!isFull(s)) {
        strcpy(s->data[++(s->top)], str);
    }
}

char* pop(Stack *s) {
    if (!isEmpty(s)) {
        return s->data[(s->top)--];
    }
    return NULL;
}

char* peek(Stack *s) {
    if (!isEmpty(s)) {
        return s->data[s->top];
    }
    return NULL;
}

// Function to check if the token is an operator
int isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

// Function to check if the token is an operand (id)
int isOperand(char *token) {
    return strcmp(token, "id") == 0;
}

// Function to print the current stack status
void printStack(Stack *s) {
    printf("Stack: [");
    for (int i = 0; i <= s->top; i++) {
        printf("%s", s->data[i]);
        if (i < s->top) {
            printf(", ");
        }
    }
    printf("]\n");
}

// Function to check precedence and perform shifts and reduces
void handleOperator(Stack *stack, char *operator) {
    printf("Handling operator '%s'\n", operator);

    while (!isEmpty(stack) && precedence(peek(stack)[0]) >= precedence(operator[0]) && operator[0] != '^') {
        char *op = pop(stack);
        printf("Pop operator '%s' from stack\n", op);
        printStack(stack);
    }

    push(stack, operator);
    printf("Push operator '%s' onto the stack\n", operator);
    printStack(stack);
}

// Function to tokenize the input expression and return a valid token
char* nextToken(char *expression, int *i) {
    static char token[MAX];
    int j = 0;

    // Skip spaces
    while (expression[*i] == ' ') (*i)++;

    if (isalpha(expression[*i])) {
        // If the token starts with a letter, it is treated as "id"
        strcpy(token, "id");
        (*i)++;
        while (isalnum(expression[*i])) {
            (*i)++; // Skip the rest of the identifier
        }
    } else {
        // Otherwise, it's a single character operator or parenthesis
        token[j++] = expression[*i];
        token[j] = '\0';
        (*i)++;
    }
    return token;
}

// Function to perform the operator precedence parsing
void parse(char *expression) {
    Stack stack;
    initStack(&stack);

    printf("Expression: %s\n", expression);
    printf("\n");

    int i = 0;
    while (i < strlen(expression)) {
        char *token = nextToken(expression, &i);

        if (isOperand(token)) {
            printf("Shift: Operand '%s' detected, pushing to stack\n", token);
            push(&stack, token); // Push operands to the stack
            printStack(&stack);
        } else if (strcmp(token, "(") == 0) {
            printf("Shift: Left parenthesis '(', pushing to stack\n");
            push(&stack, token); // Push left parenthesis to the stack
            printStack(&stack);
        } else if (strcmp(token, ")") == 0) {
            printf("Right parenthesis ')' detected, reducing until matching '('\n");
            // Pop until we find a left parenthesis
            while (!isEmpty(&stack) && strcmp(peek(&stack), "(") != 0) {
                char *op = pop(&stack);
                printf("Pop operator '%s' from stack\n", op);
                printStack(&stack);
            }
            pop(&stack); // Discard the left parenthesis
            printStack(&stack);
        } else if (isOperator(token[0])) {
            handleOperator(&stack, token); // Handle operator precedence
        } else {
            printf("Error: Invalid token '%s'\n", token);
            return;
        }

        printf("\n");
    }

    // Pop the remaining operators from the stack
    printf("No more input, popping remaining operators\n");
    while (!isEmpty(&stack)) {
        char *op = pop(&stack);
        printf("Pop operator '%s' from stack\n", op);
        printStack(&stack);
    }

    printf("\nParsing completed.\n");
}

int main() {
    char expression[MAX];

    printf("Enter an expression: ");
    fgets(expression, MAX, stdin);
    expression[strcspn(expression, "\n")] = 0; // Remove trailing newline

    parse(expression);

    return 0;
}


