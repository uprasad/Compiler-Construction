#ifndef codeGen
#define codeGen
#include "grammarOps.h"
#include "symbolDef.h"
#include "parserDef.h"

extern FILE* generateCode(FILE* fp, astNode root, Tables T);
extern Tables fillOffset(Tables T);
#endif
