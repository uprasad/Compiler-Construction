#ifndef symbol
#define symbol
#include "grammarOps.h"
#include "symbolDef.h"

extern void printSymbolTable(Tables T);
extern Tables initSymbolTable();
extern Tables populateSymbolTable(astNode root, Tables T, int ind);

extern int getType(Tables, astNode);
extern int expType(Tables, astNode);
extern void typeChecker(Tables, astNode);
extern void assgnOp(Tables, astNode);
extern void boolExp(Tables, astNode);
extern astNode jumpto(astNode root, char* nodelex);
extern int compareidList(Tables T, astNode node1, astNode node2, int flag);
extern void matchReturnType(Tables T, astNode root);
extern void funInputCheck(Tables T, astNode root, astNode glob_astr);
extern void sem_ii(Tables T, astNode root, int ind);
extern int checkOverload(Tables T);

#endif
