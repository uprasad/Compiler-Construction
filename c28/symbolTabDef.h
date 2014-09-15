/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project STAGE 1

****************************/

#ifndef symbolTabDef
#define symbolTabDef

#define MAX 30
#define NUMTABS 10
#define MAXENTRY 100

typedef struct {
	int tkid;
	int lineNum;
	char lex[MAX];
} symTabEntry;

typedef struct {
	int count;
	char funLex[MAX];
	symTabEntry *ste;
} symbolTab;

typedef symbolTab* Tables;

#endif
