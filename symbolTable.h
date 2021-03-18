#include <string.h>

#define SIZE_OF_HASH_TABLE 10
#define NO_OF_KEYWORDS 24

typedef enum {
    TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_RECORDID, TK_WITH, TK_PARAMETERS,
    TK_END, TK_WHILE, TK_INT, TK_REAL, TK_TYPE, TK_MAIN, TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT,
    TK_OUTPUT, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF, TK_THEN, TK_ENDIF, TK_READ,
    TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND,
    TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ, TK_GT, TK_GE, TK_NE
} TokenType;

typedef struct {
    TokenType token;
    char *lexeme;
    int lineNumber;
} Token;

// The structure containing all information stored in a symbol table
typedef struct {
    TokenType type;
    char *lexeme;
} SymbolTableElement;

// Internal Structure used to store the symbol table as a hash table
typedef struct {
    SymbolTableElement *element;
    SymbolTableElement *next;
} SymbolTableNode;

typedef SymbolTableNode *SymbolTable;

extern SymbolTable newSymbolTable();
extern SymbolTableElement *exists(char *lexeme, SymbolTable symbolTable);
extern SymbolTableElement *insertToken(char *lexeme, TokenType tokenType, SymbolTable symbolTable);
extern void printSymbolTable(SymbolTable SymbolTable);