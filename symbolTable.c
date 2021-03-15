#include "symbolTable.h"
#include <stdlib.h>

Token keywords[] = {
    {.lexeme = "with", .token = TK_WITH},
    {.lexeme = "parameters", .token = TK_PARAMETERS},
    {.lexeme = "end", .token = TK_END},
    {.lexeme = "while", .token = TK_WHILE},
    {.lexeme = "int", .token = TK_INT},
    {.lexeme = "real", .token = TK_REAL},
    {.lexeme = "type", .token = TK_TYPE},
    {.lexeme = "_main", .token = TK_MAIN},
    {.lexeme = "global", .token = TK_GLOBAL},
    {.lexeme = "parameter", .token = TK_PARAMETER},
    {.lexeme = "list", .token = TK_LIST},
    {.lexeme = "input", .token = TK_INPUT},
    {.lexeme = "output", .token = TK_OUTPUT},
    {.lexeme = "endwhile", .token = TK_ENDWHILE},
    {.lexeme = "if", .token = TK_IF},
    {.lexeme = "then", .token = TK_THEN},
    {.lexeme = "endif", .token = TK_ENDIF},
    {.lexeme = "read", .token = TK_READ},
    {.lexeme = "write", .token = TK_WRITE},
    {.lexeme = "return", .token = TK_RETURN},
    {.lexeme = "call", .token = TK_CALL},
    {.lexeme = "record", .token = TK_RECORD},
    {.lexeme = "endrecord", .token = TK_ENDRECORD},
    {.lexeme = "else", .token = TK_ELSE}
};

int calculateHash(char *lexeme) {
    int hash = 0;
    for(int i = 0; i < strlen(lexeme); i++) {
        hash += (((int) lexeme[i]) * i);
        hash = hash % SIZE_OF_HASH_TABLE;
    }
    return hash;
}

SymbolTableElement *exists(char *lexeme, SymbolTable symbolTable) {
    int hash = calculateHash(lexeme);
    SymbolTableNode *temp = symbolTable + hash;
    if(temp == NULL || temp -> element == NULL) {
        return NULL;
    }
    while(temp != NULL) {
        if(strcmp(lexeme, temp -> element -> lexeme) == 0) {
            return temp -> element;
        }
        temp = temp -> next;
    }
    return NULL;
}

SymbolTableElement *insertToken(char *lexeme, TokenType tokenType, SymbolTable symbolTable) {
    if(exists(lexeme, symbolTable) == NULL) {
        int hash = calculateHash(lexeme);
        SymbolTableNode *temp = symbolTable + hash;
        if(temp == NULL) {
            temp = (SymbolTableNode *) malloc(sizeof(SymbolTableNode));
            temp -> element = (SymbolTableElement *) malloc(sizeof(SymbolTableElement));
            temp -> next = NULL;
            temp -> element -> lexeme = (char *) malloc(sizeof(char) * strlen(lexeme) + 1);
            temp -> element -> type = tokenType;
            strcpy(temp -> element -> lexeme, lexeme);
            return temp -> element;
        } else if(temp -> element == NULL) {
            temp -> next = NULL;
            temp -> element = (SymbolTableElement *) malloc(sizeof(SymbolTableElement));
            temp -> element -> lexeme = (char *) malloc(sizeof(char) * strlen(lexeme) + 1);
            temp -> element -> type = tokenType;
            strcpy(temp -> element -> lexeme, lexeme);
            return temp;
        } else {
            while(temp -> next != NULL) {
                temp = temp -> next;
            }   
            temp -> next = (SymbolTableElement *) malloc(sizeof(SymbolTableElement));
            temp = temp -> next;
            temp -> element = (SymbolTableElement *) malloc(sizeof(SymbolTableElement));
            temp -> element -> lexeme = (char *) malloc(sizeof(char) * strlen(lexeme) + 1);
            temp -> element -> type = tokenType;
            strcpy(temp -> element -> lexeme, lexeme);
            temp -> next = NULL;
            return temp;
        }
    } else {
        return exists(lexeme, symbolTable);
    }
}

SymbolTable newSymbolTable() {
    SymbolTable symbolTable = (SymbolTable) malloc(sizeof(SymbolTableNode) * SIZE_OF_HASH_TABLE);
    for(int i = 0; i < NO_OF_KEYWORDS; i++) {
        insertToken(keywords[i].lexeme, keywords[i].token, symbolTable);
    }
    return symbolTable;
}

void printSymbolTable(SymbolTable symbolTable) {
    if(symbolTable == NULL) {
        printf("Error in printing. Symbol Table doesn't exist\n");
        return;
    }
    for(int i = 0; i < SIZE_OF_HASH_TABLE; i++) {
        SymbolTableNode *temp = symbolTable + i;
        while(temp != NULL) {
            printf("Value = %s\n", temp -> element -> lexeme);
            temp = temp -> next;
        }
    }
}