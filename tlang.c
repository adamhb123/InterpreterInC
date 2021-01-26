#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define ARBITRARYMAX 50
#define TOKENMAX 1000

typedef enum {
    SYNTAX_ERROR = 60,
} ERROR_TYPE;

typedef enum {
    NUMBER, VARIABLE, STRING, SEMICOLON, EQUALS, LEFT_BRACKET, RIGHT_BRACKET, UNDEFINED
} TOKEN_TYPE;

typedef struct TOKEN {
    TOKEN_TYPE type;
    char value[ARBITRARYMAX];
} TOKEN;

char* token_type_to_string(TOKEN_TYPE token_type) {
    char* ret = malloc(sizeof(char)*32);
    switch (token_type) {
    case NUMBER:
        strcpy(ret, "NUMBER");
        break;
    case VARIABLE:
        strcpy(ret, "VARIABLE");
        break;
    case STRING:
        strcpy(ret, "STRING");
        break;
    case SEMICOLON:
        strcpy(ret, "SEMICOLON");
        break;
    case EQUALS:
        strcpy(ret, "EQUALS");
        break;
    case LEFT_BRACKET:
        strcpy(ret, "LEFT_BRACKET");
        break;
    case RIGHT_BRACKET:
        strcpy(ret, "RIGHT_BRACKET");
        break;
    }
    return ret;
}



char* lgetline(size_t* bufflen) {
    char* line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if (line == NULL)
        return NULL;

    for (;;) {
        c = fgetc(stdin);
        if (c == EOF)
            break;

        if (--len == 0) {
            len = lenmax;
            char* linen = realloc(linep, lenmax *= 2);

            if (linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if ((*line++ = c) == '\n')
            break;
    }
    *bufflen = lenmax - len;
    *line = '\0';
    return linep;
}

TOKEN* new_token(TOKEN_TYPE, char*);

void add_token(TOKEN token_dst[], TOKEN* token, int* token_count) {
    token_dst[*token_count] = *token;
    printf("ADDED %s TOKEN\n", token_type_to_string(token_dst[*token_count].type));
    ++(*token_count);
}

void print_tokens(TOKEN tokens[], int token_count) {
    printf("Printing tokens with token count %d\n", token_count);
    for (int i = 0; i < token_count; i++) {
        printf("TOKEN:\n\tTYPE:%s\n\tVALUE:%s\n", token_type_to_string(tokens[i].type),tokens[i].value);
    }
}

int main() {
    char* read;
    char next;
    size_t bufflen;
    int token_count = 0;
    TOKEN token_array[TOKENMAX];
    TOKEN_TYPE cur_token_type;
    char* cur_token_value = malloc(ARBITRARYMAX*sizeof(char));
    //  REPL
    for (;;) {
        read = lgetline(&bufflen);
        for (int i = 0; i < bufflen; i++) {
            next = read[i + 1];
            printf("READ:%c\nNEXT:%c\n", read[i], next);
            printf("BUFFLEN:%ld\n", bufflen);

            if (isalpha(read[i])) {
                //  It's a variable
                int varnameln = 0;
                while (isalpha(read[i]) && i != bufflen) {
                    cur_token_value[varnameln] = read[i];
                    varnameln++;
                    i++;
                }
                cur_token_value = realloc(cur_token_value, varnameln);
                cur_token_value[varnameln] = '\0';
                cur_token_type = VARIABLE;
            }
            else if (isdigit(read[i])) {
                int numvallen = 0;
                while (isdigit(read[i]) && i != bufflen) {
                    cur_token_value[numvallen] = read[i];
                    printf("READ DIGIT: %c\n", read[i]);
                    numvallen++;
                    i++;
                }
                cur_token_value = realloc(cur_token_value, numvallen);
                cur_token_value[numvallen] = '\0';
                cur_token_type = NUMBER;
            }
            else {
                cur_token_value = realloc(cur_token_value, sizeof(char));
                cur_token_value = '\0';
                switch (read[i]) {
                case ';':
                    cur_token_type = SEMICOLON;
                    break;
                case '=':
                    cur_token_type = EQUALS;
                    break;
                case '{':
                    cur_token_type = LEFT_BRACKET;
                    break;
                
                default:
                    cur_token_type = UNDEFINED;
		}    
		}
            
            add_token(token_array, new_token(cur_token_type, '\0'), &token_count);
            printf("LAST%c\n", read[i]);
            print_tokens(token_array, token_count);
        }

    }
    
}
