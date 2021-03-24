#include "Lexer.h"

char *tokenTypeToString[] = {"TK_ASSIGNOP", "TK_COMMENT", "TK_FIELDID", "TK_ID", "TK_NUM", "TK_RNUM", "TK_FUNID", "TK_RECORDID", "TK_WITH", "TK_PARAMETERS",
    "TK_END", "TK_WHILE", "TK_INT", "TK_REAL", "TK_TYPE", "TK_MAIN", "TK_GLOBAL", "TK_PARAMETER", "TK_LIST", "TK_SQL", "TK_SQR", "TK_INPUT",
    "TK_OUTPUT", "TK_COMMA", "TK_SEM", "TK_COLON", "TK_DOT", "TK_ENDWHILE", "TK_OP", "TK_CL", "TK_IF", "TK_THEN", "TK_ENDIF", "TK_READ",
    "TK_WRITE", "TK_RETURN", "TK_PLUS", "TK_MINUS", "TK_MUL", "TK_DIV", "TK_CALL", "TK_RECORD", "TK_ENDRECORD", "TK_ELSE", "TK_AND",
    "TK_OR", "TK_NOT", "TK_LT", "TK_LE", "TK_EQ", "TK_GT", "TK_GE", "TK_NE"};

Lexer *newLexer(char *fileName) {
    Lexer *lexer = (Lexer *) malloc(sizeof(Lexer));
    lexer -> buffer = newBuffer(fileName);
    lexer -> lineNumber = 1;
    lexer -> symbolTable = newSymbolTable();
    return lexer;
}

Token *newToken(Lexer *lexer, TokenType tokenType) {
    Token *token = (Token *) malloc(sizeof(Token));
    token -> token = tokenType;
    token -> lexeme = getLexeme(lexer -> buffer);
    token -> lineNumber = lexer -> lineNumber;
    if(exists(token -> lexeme, lexer -> symbolTable) == NULL && (tokenType == TK_ID)) {
        insertToken(token -> lexeme, token -> token, lexer -> symbolTable);
    }
    return token;
}

Token *getNextToken(Lexer *lexer) {
    TwinBuffer *buffer = lexer -> buffer;
    int state = 0;
    char lookahead = nextChar(buffer);
    while(1) {
        switch (state) {
        case 0:
            switch (lookahead) {
                case '\n':
                    printf("NewLine \n");
                    lexer -> lineNumber += 1;
                case '\t':
                case ' ':
                    getLexeme(lexer -> buffer);
                    break;
                case '~':
                    return newToken(lexer, TK_NOT);
                case '+':
                    return newToken(lexer, TK_PLUS);
                case '-':
                    return newToken(lexer, TK_MINUS);
                case '*':
                    return newToken(lexer, TK_MUL);
                case '/':
                    return newToken(lexer, TK_DIV);
                case '&':
                    state = 19;
                    break;
                case '@':
                    state = 22;
                    break;
                case '(':
                    return newToken(lexer, TK_OP);
                case ')':
                    return newToken(lexer, TK_CL);
                case '.':
                    return newToken(lexer, TK_DOT);
                case ':':
                    return newToken(lexer, TK_COLON);
                case ';':
                    return newToken(lexer, TK_SEM);
                case ',':
                    return newToken(lexer, TK_COMMA);
                case '[':
                    return newToken(lexer, TK_SQL);
                case ']':
                    return newToken(lexer, TK_SQR);
                case '%':
                    state = 33;
                    break;
                case '>':
                    state = 7;
                    break;
                case '!':
                    state = 12;
                    break;
                case '=':
                    state = 10;
                    break;
                case '<':
                    state = 1;
                    break;
                case '#':
                    state = 39;
                    break;
                case '_':
                    state = 42;
                    break;
                case 'b':
                case 'c':
                case 'd':
                    state = 46;
                    break;
                case '\0':
                    printf("Tokenization Complete\n");
                    return NULL;
                default: {
                    if(lookahead == 'a' || (lookahead >= 'e' && lookahead <= 'z')) {
                        state = 50;
                        break;
                    } else if(isdigit(lookahead) != 0) {
                        state = 34;
                        break;
                    } else {
                        printf("Lexical Error : Line Number %d : Some other symbol expected Found %c\n", lexer -> lineNumber, lookahead);
                        getLexeme(lexer -> buffer);
                        state = 0;
                        // return NULL;
                    }
                }
            }
            break;
        case 1:
            if(lookahead == '-') {
                state = 2;
            } else if(lookahead == '=') {
                return newToken(lexer, TK_LE);
            } else {
                retract(lexer -> buffer, 1);
                return newToken(lexer, TK_LT);
            }
            break;
        case 2:
            if(lookahead == '-') {
                state = 3;
            } else {
                retract(lexer -> buffer, 2);
                return newToken(lexer, TK_LT);
            }
            break;
        case 3:
            if(lookahead == '-') {
                return newToken(lexer, TK_ASSIGNOP);
            } else {
                retract(lexer -> buffer, 3);
                return newToken(lexer, TK_LT);
            }
        case 7:
            if(lookahead == '=') {
                return newToken(lexer, TK_GE);
            } else {
                retract(lexer -> buffer, 1);
                return newToken(lexer, TK_GT);
            }
            break;
        case 10:
            if(lookahead == '=') {
                return newToken(lexer, TK_EQ);
            } else {
                printf("Lexical Error : Line Number %d : Expected = Found %c\n", lexer -> lineNumber, lookahead);
                getLexeme(lexer -> buffer);
                state = 0;
                // return NULL;
            }
            break;
        case 12:
            if(lookahead == '=') {
                return newToken(lexer, TK_NE);
            } else {
                printf("Lexical Error : Line Number %d : Expected = Found %c\n", lexer -> lineNumber, lookahead);
                getLexeme(lexer -> buffer);
                state = 0;
                // return NULL;
            }
            break;
        case 19:
            if(lookahead == '&') {
                state = 20;
            } else {
                printf("Lexical Error : Line Number %d : Expected & Found %c\n", lexer -> lineNumber, lookahead);
                getLexeme(lexer -> buffer);
                state = 0;
                // return NULL;
            }
            break;
        case 20:
            if(lookahead == '&') {
                return newToken(lexer, TK_AND);
            } else {
                printf("Lexical Error : Line Number %d : Expected & Found %c\n", lexer -> lineNumber, lookahead);
                getLexeme(lexer -> buffer);
                state = 0;
                // return NULL;
            }
            break;
        case 22:
            if(lookahead == '@') {
                state = 23;
            } else {
                printf("Lexical Error : Line Number %d : Expected @ Found %c\n", lexer -> lineNumber, lookahead);
                getLexeme(lexer -> buffer);
                state = 0;
                // return NULL;
            }
            break;
        case 23:
            if(lookahead == '@') {
                return newToken(lexer, TK_OR);
            } else {
                printf("Lexical Error : Line Number %d : Expected @ Found %c\n", lexer -> lineNumber, lookahead);
                getLexeme(lexer -> buffer);
                state = 0;
                //  return NULL;
            }
            break;
        case 33:
            if(lookahead == '\n') {
                getLexeme(lexer -> buffer);
                state = 0;
            }
            break;
        case 34:
            if(isdigit(lookahead)) {
                state = 34;   
            } else if(lookahead == '.') {
                state = 36;
            } else {
                retract(lexer -> buffer, 1);
                return newToken(lexer, TK_NUM);
            }
            break;
        case 36:
            if(isdigit(lookahead)) {
                state = 37;
            } else {
                printf("Lexical Error : Line Number %d : Expected digit Found %c\n", lexer -> lineNumber, lookahead);
                getLexeme(lexer -> buffer);
                state = 0;
                // return NULL;
            }
            break;
        case 37:
            if(isdigit(lookahead)) {
                return newToken(lexer, TK_RNUM);
            } else {
                printf("Lexical Error : Line Number %d : Expected digit Found %c\n", lexer -> lineNumber, lookahead);
                getLexeme(lexer -> buffer);
                state = 0;
                // return NULL;
            }
            break;
        case 39:
            if(islower(lookahead)) {
                state = 40;
            } else {
                printf("Lexical Error : Line Number %d : Expected [a-z] Found %c\n", lexer -> lineNumber, lookahead);
                getLexeme(lexer -> buffer);
                state = 0;
                // return NULL;
            }
            break;
        case 40:
            if(islower(lookahead)) {
                state = 40;
            } else {
                retract(lexer -> buffer, 1);
                return newToken(lexer, TK_RECORDID);
            }
            break;
        case 42:
            if(isalpha(lookahead)) {
                state = 43;
            } else {
                printf("Lexical Error : Line Number %d : Expected [a-z|A-Z] Found %c\n", lexer -> lineNumber, lookahead);
                getLexeme(lexer -> buffer);
                state = 0;
                // return NULL;
            }
            break;
        case 43:
            if(isalpha(lookahead)) {
                state = 43;
            } else if(isdigit(lookahead)) {
                state = 44;
            } else {
                retract(lexer -> buffer, 1);
                return newToken(lexer, TK_FUNID);
            }
            break;
        case 44:
            if(isdigit(lookahead)) {
                state = 44;
            } else {
                retract(lexer -> buffer, 1);
                return newToken(lexer, TK_FUNID);
            }
            break;
        case 46:
            if(islower(lookahead)) {
                state = 50;
            } else if(lookahead >= '2' && lookahead <= '7') {
                state = 47;
            } else {
                printf("Lexical Error : Line Number %d : Expected [a-z]|[2-7] Found %c\n", lexer -> lineNumber, lookahead);
                getLexeme(lexer -> buffer);
                state = 0;
                // return NULL;
            }
            break;
        case 47:
            if(lookahead >= 'b' && lookahead <= 'd') {
                state = 47;
            } else if(lookahead >= '2' && lookahead <= '7') {
                state = 48;
            } else {
                retract(lexer -> buffer, 1);
                return newToken(lexer, TK_ID);
            }
            break;
        case 48:
            if(lookahead >= '2' && lookahead <= '7') {
                state = 48;
            } else {
                retract(lexer -> buffer, 1);
                return newToken(lexer, TK_ID);
            }
            break;
        case 50:
            if(islower(lookahead)) {
                state = 50;
            } else {
                retract(lexer -> buffer, 1);
                return newToken(lexer, TK_FIELDID);
            }
            break;
        }
        lookahead = nextChar(buffer);
    }
}

void printToken(Token *token) {
    printf("Token Type = %-10s Line Number = %-3d Value = %s\n", tokenTypeToString[token -> token], token -> lineNumber, token -> lexeme);
}

void main() {
    Lexer *lexer = newLexer("./test.txt");
    printf("Lexer Created\n");
    Token *token;
    while((token = getNextToken(lexer)) != NULL) {
        printToken(token);
    }
}