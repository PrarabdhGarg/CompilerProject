#include <stdlib.h>
#include <stdio.h>

#define MAX_BUFF_SIZE 100

typedef struct {
	char *buff1;
	char *buff2;
	char *lexemeBegin;
	char *forward;
	int length;
	FILE *file;
} TwinBuffer;

extern TwinBuffer *newBuffer(char *fileName);
extern char nextChar(TwinBuffer *twinBuffer);
extern void retract(TwinBuffer *twinBuffer, int noOfRetractions);
extern char *getLexeme(TwinBuffer *twinBuffer);
