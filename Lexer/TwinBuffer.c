#include "TwinBuffer.h"

// A twin buffer includes two buffers to reduce the number of I/O operations
// required while breaking the source code into lexemes. Buffers ensure that
// we can perform I/O in chunks instead of character by character. Two buffers
// are required to handle the case when a lexeme spans across buffer boundaries

// Create a new Twin Buffer object, to read source code from [fileName]
TwinBuffer *newBuffer(char *fileName) {
	TwinBuffer *twinBuffer = (TwinBuffer *) malloc(sizeof(TwinBuffer));
	twinBuffer -> file = fopen(fileName, "r");
	if(twinBuffer -> file == NULL) {
		printf("Error opening file. Pleaase enter valid file name\n");
		free(twinBuffer);
		return NULL;
	}
	twinBuffer -> buff1 = (char *) malloc(sizeof(char) * MAX_BUFF_SIZE);
	twinBuffer -> buff2 = (char *) malloc(sizeof(char) * MAX_BUFF_SIZE);
	twinBuffer -> buff1[MAX_BUFF_SIZE - 1] = '\0';
	twinBuffer -> buff2[MAX_BUFF_SIZE - 1] = '\0';
	twinBuffer -> length = 0;
	twinBuffer -> lexemeBegin = twinBuffer -> buff1;
	twinBuffer -> forward = twinBuffer -> buff1;
	int read = fread(twinBuffer -> buff1, sizeof(char), (MAX_BUFF_SIZE - 1), twinBuffer -> file);
	if(read != MAX_BUFF_SIZE - 1) {
		twinBuffer -> buff1[read] = '\0';
	}
	return twinBuffer;
}

// Return the next character of the from the buffered I/O. If the current buffer has 
// exhausted its capacity, load more data into the other buffer, and continue reading
char nextChar(TwinBuffer *twinBuffer) {
	if(twinBuffer -> forward == twinBuffer -> buff1 + (MAX_BUFF_SIZE - 1)) {
		int read = fread(twinBuffer -> buff2, sizeof(char), (MAX_BUFF_SIZE - 1), twinBuffer -> file);
		if(read != (MAX_BUFF_SIZE - 1)) {
			twinBuffer -> buff2[read] = '\0';
		}
		twinBuffer -> forward = twinBuffer -> buff2;
	} else if(twinBuffer -> forward == twinBuffer -> buff2 + (MAX_BUFF_SIZE - 1)) {
		int read = fread(twinBuffer -> buff1, sizeof(char), MAX_BUFF_SIZE - 1, twinBuffer -> file);
		if(read != (MAX_BUFF_SIZE - 1)) {
			twinBuffer -> buff1[read] = '\0';
		}
		twinBuffer -> forward = twinBuffer -> buff1;
	}
	char c = *(twinBuffer -> forward);
	twinBuffer -> length += 1;
	twinBuffer -> forward = twinBuffer -> forward + 1;
	return c;
}

// Function to get the current lexeme read by the lexer.
char *getLexeme(TwinBuffer *twinBuffer) {
	char *lexeme = (char *)malloc(sizeof(char) * (twinBuffer -> length + 1));
	for(int i = 0; i < twinBuffer -> length; i++) {
		if(twinBuffer -> lexemeBegin == twinBuffer -> buff1 + (MAX_BUFF_SIZE - 1)) {
			twinBuffer -> lexemeBegin = twinBuffer -> buff2;
		}
		if(twinBuffer -> lexemeBegin == twinBuffer -> buff2 + (MAX_BUFF_SIZE - 1)) {
			twinBuffer -> lexemeBegin = twinBuffer -> buff1;
		}
		lexeme[i] = *(twinBuffer -> lexemeBegin);
		twinBuffer -> lexemeBegin = twinBuffer -> lexemeBegin + 1;
	}
	lexeme[twinBuffer -> length] = '\0';
	twinBuffer -> length = 0;
	return lexeme;
}

// Function to move the forward pointer backwards by [noOfRetractions] characters
// Required because the transition diagram requires certain number to lookahead 
// symbols before deciding the token boundaries in certain scenarios
void retract(TwinBuffer *twinBuffer, int noOfRetractions) {
	for(int i = 0; i < noOfRetractions; i++) {
		if(twinBuffer -> forward == twinBuffer -> buff1) {
			twinBuffer -> forward = twinBuffer -> buff2;
		}
		if(twinBuffer -> forward == twinBuffer -> buff2) {
			twinBuffer -> forward = twinBuffer -> buff1;
		}
		twinBuffer -> forward = twinBuffer -> forward - 1;
		twinBuffer -> length -= 1;
	}
}