#include "TwinBuffer.h"
#include "../symbolTable.h"
#include <ctype.h>

typedef struct {
    TwinBuffer *buffer;
    int lineNumber;
    SymbolTable symbolTable;
} Lexer;

extern Lexer *newLexer(char *fileName);
extern Token *getNextToken(Lexer *lexer);
extern void printToken(Token *token);