/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project STAGE 1

****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolDef.h"
#include "lexerDef.h"
//#include "lexer.h"
#include "parserDef.h"

// To keep track of the lexeme
bool rb = false;
int prev = -2;
int pos = 0, bpos = 0;
char lex[MAX];

int count = 0;
int lineNumber = 1;

// Index of beginning of lexeme
int lexemeBegin = 0;
	
// Current index being scanned
int forward = BUF_SIZE;

// Which buffer is 'forward' in?
int curBuffer = 0;
int x = 0;

int trans[STATES][ALPHS];

FILE* getStream(FILE* fp, buffer B, buffersize k) {
	count = fread(B, SIZE, k, fp);
	return fp;
}

int cc;

void removeWhites(char lex[]) {
	int i;
	char tlex[MAX];
	
	int j = 0;
	for (i=0; i<MAX; ++i) {
		if ((lex[i] == '\n') || (lex[i] == ' ') || (lex[i] == '\t'))
			continue;
		tlex[j++] = lex[i];
	}
	
	tlex[j] = '\0';
	strcpy(lex, tlex);
}

void clearBuffer(char* lex) {
	int i;
	for (i=0; i<MAX; ++i) {
		lex[i] = '\0';
	}

//	strcpy(lex, "");
}

tokenInfo getNextToken(FILE* fp, buffer B[], buffersize k) {
	int curState = 0;
	int t;
	tokenInfo ti;
	clearBuffer(lex);
	bpos = 0;
	pos = 0;
	while (1) {
		if (forward >= k) {
				curBuffer = (curBuffer + 1) % 2;
			if (x == 0) {
				fp = getStream(fp, B[curBuffer], k);
				cc = count;
			}
			x = 0;
			forward = 0;
		} else {
			t = B[curBuffer][forward];
			while((trans[curState][t-1] >= 0) && (forward < k)) {
				t = B[curBuffer][forward];
				
				if (rb == true && pos == 0 && prev == -2) {
					bpos = pos;
					pos++;
				} else {
					lex[bpos] = B[curBuffer][forward];
					bpos++;
				}
				
				if (t == 10)
					lineNumber++;
				curState = trans[curState][t-1];
				if (curState == -1) {
					printf("\nERROR_2 : Unknown symbol %c at line number %d ", B[curBuffer][forward], lineNumber);
					ti.tkid = -1;
					return ti;
					break;
				}
				forward++;
				cc--;
				if (count < k && cc == 0) {
					break;
				}
			}
			if (cc == 0 && count < k) {
				ti.tkid = -2;
				return ti;
			}
			if (forward >= k)
				continue;
			else {
				switch(trans[curState][B[curBuffer][forward]-1]) {
				case -1:
					printf("\nERROR_3 : Unknown pattern %s%c at line number %d ", lex, B[curBuffer][forward], lineNumber);
					ti.tkid = -1;
					ti.lineNum = lineNumber;
					return ti;
					break;
				case -2:
					prev = -2;
					lex[bpos] = '\0';
					rb = false;
					ti.tkid = curState;
					ti.lineNum = lineNumber;
					removeWhites(lex);
					strcpy(ti.lex, lex);
					return ti;
					break;
				case -3:
					prev = -3;
					lex[bpos-1] = '\0';
					rb = true;
					if (B[curBuffer][forward] == '\n' && curState != TK_ID && curState != TK_FIELDID && curState != TK_RECORDID)
						lineNumber--;
					forward--;
					if (forward < 0) {
						forward = BUF_SIZE + forward;
						curBuffer = (curBuffer + 1) % 2;
						x = 1;
					}
					ti.tkid = curState;
					ti.lineNum = lineNumber;
					removeWhites(lex);
					strcpy(ti.lex, lex);
					return ti;
					break;
				}
			}
		}
	}
}

void loadTransTable(FILE* fp) {
	int i, j;
	char temp[MAX+1];
	for (i=0; i<ALPHS-1; ++i) {
		fscanf(fp, "%s\t", temp);
	}
	fscanf(fp, "%s\n", temp);
	
	j = 0;
	while (j < STATES) {
		for (i=0; i<ALPHS-1; ++i) {
			fscanf(fp, "%s\t", temp);
			trans[j][i] = atoi(temp);
		}
		fscanf(fp, "%s\n", temp);
		trans[j][i] = atoi(temp);
		
		j++;
	}
}
/*
Tables loadTransTableAndSymTab() {
	buffer B[2] = {NULL, NULL};
	buffersize k = BUF_SIZE;
	
	int i;
	
	for (i=0; i<2; ++i) {
		B[i] = (buffer)malloc(k*SIZE);
	}
	
	FILE* fp = fopen("finale_dfa_more_changes.csv", "r");
	if (!fp) {
		printf("Finite automata file MISSING\n");
		return NULL;
	}

	loadTransTable(fp);
	fclose(fp);
	
	int token;
	bool inglobal = true;
	int tabCt = 0, curInd = 0;
	char curFun[MAX];
	strcpy(curFun, "_");
	
	Tables T = initSymTab();
	strcpy(T[curInd].funLex, "_");
	
	tokenInfo ti;
	
	FILE* stream = fopen(filename, "r");
	
	if (stream == NULL) {
		printf("Source file does not exist!\n");
		return 0;
	}
	
	char temple[30];
	
	do {
		clearBuffer(lex);
		bpos = 0; pos = 0;
		ti = getNextToken(stream, B, k);
		token = ti.tkid;
		symTabEntry ste;
		ste.lineNum = lineNumber;
		
		int i, j=0;
		for (i=0; lex[i]!='\0'; ++i) {
			if (lex[i] == '\n' || lex[i] == ' ' || lex[i] == '\0') {
				continue;
			} else {
				temple[j++] = lex[i];
			}
		}
		temple[j] = '\0';
		strcpy(lex, temple);
//		printf("%d, %s\n", token, lex);
		
		if (token == TK_FUNID && strlen(lex) > MAXFUNLEN) {
			printf("\nERROR_1 : Expected Length of function %s is %d\n", lex, MAXFUNLEN);
			return T;
		}
		
		if (token == TK_ID && strlen(lex) > MAXIDLEN) {
			printf("\nERROR_1 : Expected Length of identifier %s is %d\n", lex, MAXFUNLEN);
			return T;
		}

		if ((token == TK_FUNID || token == TK_MAIN) && (inglobal == true)) {
			tabCt++;
			curInd = tabCt;
			strcpy(curFun, lex);
			inglobal = false;
			strcpy(T[curInd].funLex, lex);
		} else if (token == TK_END) {
			curInd = 0;
			strcpy(curFun, "_");
			inglobal = true;
		}
		
		int tk = token;
		
		strcpy(ste.lex, lex);
		ste.tkid = tk;
		
		symbolTab* basest;
		basest = addToSymTab(T[curInd], ste);
		
		strcpy(ti.symid, curFun);
		T[curInd] = *basest;
		ti.tkid = token;
		ti.addr = T[curInd].count - 1;

	} while (token != -2 && token != -1);
	
	return T;

}
*/
