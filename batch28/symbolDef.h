/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#ifndef symbolDef
#define symbolDef

//#include "lexerDef.h"
//#include "parserDef.h"
//#include "parser.h"
//#include "lexer.h"

#define MAX_ENTRY 200
#define MAX_TABLES 20

typedef enum {LOCAL, GLOBAL} Scope;
typedef struct {
	char funid[30];
	int type;
	char rec[30];
	Scope scope;
	int offset;
//	char symbol[30];
	char sym[30];
} Entry;

typedef struct {
	char funid[30];
	Entry e[MAX_ENTRY];
	int count;
} Table;

typedef Table* Tables;

#endif
