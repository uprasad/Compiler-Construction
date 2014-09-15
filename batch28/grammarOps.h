/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#ifndef grammarOps
#define grammarOps

#include <stdio.h>
#include "parserDef.h"

extern void printSet(int* A);

extern Lexeme* lexemize(char* str);

extern int* setUnion(int*, int*);

extern int* getFirstSet(int*, Hashtable, Lexeme);

extern int* getFollowSet(Hashtable, Lexeme);

extern Lexeme* addRHStoList(Lexeme* Head, Lexeme RHS);

extern Hashtable initializeHashtable(Hashtable H, int Hashtablesize);

extern int stringHashFunction(char* str, int Hashtablesize);

extern int getNextAddress(int index, int j, int hashtablesize);

extern Hashtable addLexemetoTable(Hashtable H, Lexeme L, Lexeme* RHShead, int Hashtablesize);

extern int findinHashtable(Hashtable H, Lexeme L, int hashtablesize);

extern boolean ifPresent(int* arr, int elem, int arrsize);

extern int* getfirstSet(int* firstSet, int firstsetsize, Hashtable H, Lexeme L, int hashtablesize);

extern void printHash(Hashtable H);

extern int* setUnion(int* A, int* B);

extern int* findForFollow(Hashtable H, Lexeme L);

extern int* getFirstSet(int* firstSet, Hashtable H, Lexeme L);

extern int* getFollowSet(Hashtable H, Lexeme L);

extern void initializeParseTable(int ParseTable[HASHTABLE_SIZE][STATES]);

extern int populateParseTable(int ParseTable[HASHTABLE_SIZE][STATES], Hashtable H, int terminal, Lexeme L);

void printParseTable(int ParseTable[HASHTABLE_SIZE][STATES], FILE* fp);

extern Stack initStack();
extern treeNode initParseTree(char* lex);
extern char* reverseEnum(int token);
extern astNode initAST(char* lex, int prodno);
extern astNode populateAST(treeNode ptRoot, Hashtable ASTtable);

#endif
