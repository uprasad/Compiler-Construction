/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#ifndef parser
#define parser

#include <stdio.h>
#include "parserDef.h"
#include "lexer.h"
#include "grammarOps.h"

extern Stack push(Stack s, char* lex);
extern void printStack(Stack s);
extern Stack pop(Stack s);
extern char* top(Stack s);
extern bool isEmpty(Stack s);

extern int printParseTree(treeNode root, int c);
extern void printParseTree2(treeNode root, FILE* fp, int c);
extern astNode addChild(astNode root, astNode child);
extern int printAST(FILE* fp, astNode root, int c);
extern treeNode _parseInput(int PT[HASHTABLE_SIZE][STATES], Hashtable H, buffer B[], buffersize bk, FILE* fp);

#endif
