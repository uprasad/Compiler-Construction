/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#ifndef parserDef
#define parserDef

#define NUMCHILD 10

#define GRAMBUF 200
#define HASHTABLE_SIZE 211
#define NONTERMS 100
#define HPR 10           // number of pointers from each element in the hashtable
#define FIRST_SET_SIZE 30
#define FOLLOW_SET_SIZE 30
#define STATES 172

#define FIHC 4
#define FOHC 20

#define NAMELEN 20

extern int _done;

typedef enum{FALSE, TRUE} boolean;

enum {
	END_OF_FILE
};

typedef struct {
	char symid[30];
	int tkid;
	int addr;
	char lex[30];
	int lineNum;
} tokenInfo;

struct _node
{
	char lexName[30];
	struct _node* next;
	boolean isTerminal;
};

typedef struct _node Lexeme;

typedef struct _heads
{
	char lexemeName[30];
	Lexeme* nextLexeme[HPR];
}HeadLexeme;

typedef HeadLexeme* Hashtable;

typedef int* firstSet[NONTERMS];
typedef int* followSet[NONTERMS];

struct _treeNode_ {
	char lex[30];
	int prodno;
	int lineNum;
	tokenInfo ti;
	struct _treeNode_ *parent;
	struct _treeNode_ *child[NUMCHILD];
};

typedef struct _treeNode_  _treeNode;
typedef _treeNode* treeNode;

typedef struct _item_ {
	char lex[30];
	struct _item_* next;
} _item;

typedef _item *item;

typedef struct {
	item head;
	int count;
} _Stack;

typedef _Stack *Stack;

typedef struct _astNode_ {
	char lex[30];
	int prodno;
	tokenInfo ti;
	struct _astNode_ *parent;
	struct _astNode_ *headChild;
	struct _astNode_ *nextChild;
	int as_type;
} _astNode;

typedef _astNode* astNode;

#endif
