/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#ifndef lexerDef
#define lexerDef

//#include "symbolTab.h"

// Number of alphabets
#define ALPHS 130

// Number of states
#define STATES 172

// Offset (for internal use)
#define S ALPHS*3 + 1

// Max size of string
#define MAX 30

// Buffer size in bytes
#define BUF_SIZE 16

// Size quantum
#define SIZE sizeof(char)

// Max functionID length
#define MAXFUNLEN 20

// Max identifier length
#define MAXIDLEN 20

extern int lineNumber;
extern int trans[STATES][ALPHS];

typedef enum {false, true} bool;
extern char* filename;

typedef char* buffer;
typedef unsigned int buffersize;

extern buffer B[2];
extern buffersize k;
extern char lex[MAX];
extern int bpos;
extern int pos;
extern int forward;

enum {
	TK_ASSIGNOP = 126,	// 1
	TK_COMMENT = 158,
	TK_FIELDID = 109,
	TK_ID = 106,
	TK_NUM = 150,
	TK_RNUM = 153,
	TK_FUNID = 167,
	TK_RECORDID = 156,
	TK_WITH = 29,
	TK_PARAMETERS = 61,	// 10
	TK_END = 64,
	TK_WHILE = 34,
	TK_INT = 49,
	TK_REAL = 15,
	TK_TYPE = 96,
	TK_MAIN = 147,
	TK_GLOBAL = 24,
	TK_PARAMETER = 60,
	TK_LIST = 87,
	TK_SQL = 110,		// 20
	TK_SQR = 111,
	TK_INPUT = 47,
	TK_OUTPUT = 165,
	TK_SEM = 113,
	TK_COLON = 112,
	TK_DOT = 114,
	TK_ENDWHILE = 70,
	TK_OP = 115,
	TK_CL = 116,
	TK_IF = 42,		// 30
	TK_THEN = 92,
	TK_ENDIF = 170,
	TK_READ = 17,
	TK_WRITE = 39,
	TK_RETURN = 7,
	TK_PLUS = 117,
	TK_MINUS = 118,
	TK_MUL = 119,
	TK_DIV = 120,
	TK_CALL = 101,	// 40
	TK_RECORD = 12,
	TK_ENDRECORD = 77,
	TK_ELSE = 82,
	TK_AND = 137,
	TK_OR = 140,
	TK_NOT = 121,
	TK_LT = 127,
	TK_LE = 123,
	TK_EQ = 132,
	TK_GT = 130,	// 50
	TK_GE = 129,
	TK_NE = 134,
	TK_COMMA = 171
};

#endif
