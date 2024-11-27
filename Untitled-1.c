#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

// Token types
typedef enum {
    TOKEN_ID,
    TOKEN_PLUS,
    TOKEN_MULTIPLY,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_END
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char value[50];
} Token;

// Global variables
Token currentToken;
char* input;
int inputIndex = 0;

// Function prototypes
void E();
void T();
void F();
void match(TokenType type);
Token getNextToken();
void parse();

// Lexical analyzer (tokenizer)
Token getNextToken() {
    Token token;
    
    // Skip whitespace
    while (input[inputIndex] == ' ') {
        inputIndex++;
    }
    
    // Check current character
    switch (input[inputIndex]) {
        case '+':
            token.type = TOKEN_PLUS;
            strcpy(token.value, "+");
            inputIndex++;
            break;
        case '*':
            token.type = TOKEN_MULTIPLY;
            strcpy(token.value, "*");
            inputIndex++;
            break;
        case '(':
            token.type = TOKEN_LPAREN;
            strcpy(token.value, "(");
            inputIndex++;
            break;
        case ')':
            token.type = TOKEN_RPAREN;
            strcpy(token.value, ")");
            inputIndex++;
            break;
        case '\0':
            token.type = TOKEN_END;
            strcpy(token.value, "");
            break;
        default:
            // Identifier or number
            if (isalnum(input[inputIndex])) {
                int j = 0;
                token.type = TOKEN_ID;
                while (isalnum(input[inputIndex])) {
                    token.value[j++] = input[inputIndex++];
                }
                token.value[j] = '\0';
            } else {
                printf("Error: Unexpected character %c\n", input[inputIndex]);
                exit(1);
            }
    }
    
    return token;
}

// Match current token with expected token type
void match(TokenType type) {
    if (currentToken.type == type) {
        currentToken = getNextToken();
    } else {
        printf("Error: Unexpected token %s\n", currentToken.value);
        exit(1);
    }
}

// Parsing functions for grammar rules
void F() {
    if (currentToken.type == TOKEN_ID) {
        match(TOKEN_ID);
    } else if (currentToken.type == TOKEN_LPAREN) {
        match(TOKEN_LPAREN);
        E();
        match(TOKEN_RPAREN);
    } else {
        printf("Error: Invalid token in F\n");
        exit(1);
    }
}

void T() {
    F();
    while (currentToken.type == TOKEN_MULTIPLY) {
        match(TOKEN_MULTIPLY);
        F();
    }
}

void E() {
    T();
    while (currentToken.type == TOKEN_PLUS) {
        match(TOKEN_PLUS);
        T();
    }
}

// Main parsing function
void parse() {
    // Initialize first token
    currentToken = getNextToken();
    
    // Start parsing from the start symbol (E)
    E();
    
    // Check if we've reached the end of input
    if (currentToken.type != TOKEN_END) {
        printf("Error: Unexpected tokens at end of input\n");
        exit(1);
    }
    
    printf("Parsing successful!\n");
}

int main() {
    // Test cases
    char* testCases[] = {
        "id",
        "id+id",
        "id*id",
        "(id)",
        "id+id*id",
        "(id+id)*id"
    };
    
    int numTestCases = sizeof(testCases) / sizeof(testCases[0]);
    
    for (int i = 0; i < numTestCases; i++) {
        printf("Parsing input: %s\n", testCases[i]);
        input = testCases[i];
        inputIndex = 0;
        
        // Reset global variables
        currentToken.type = TOKEN_END;
        memset(currentToken.value, 0, sizeof(currentToken.value));
        
        // Attempt to parse
        parse();
        printf("\n");
    }
    
    return 0;
}