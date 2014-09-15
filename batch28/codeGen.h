/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#ifndef codeGen
#define codeGen
#include "grammarOps.h"
#include "symbolDef.h"
#include "parserDef.h"

extern FILE* generateCode(FILE* fp, astNode root, Tables T);
extern Tables fillOffset(Tables T);
#endif
