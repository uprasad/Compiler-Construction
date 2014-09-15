/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#ifndef lexer
#define lexer
#include <stdio.h>
#include "lexerDef.h"

extern FILE* getStream(FILE* fp, buffer B, buffersize k);
extern tokenInfo getNextToken(FILE* fp, buffer B[], buffersize k);
extern void loadTransTable(FILE* fp);
extern void clearBuffer(char* lex);

#endif
