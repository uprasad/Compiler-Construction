/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project STAGE 1

****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTabDef.h"
//#include "symbolTab.h"
//#include "lexerDef.h"

Tables initSymTab() {
	Tables T;
	T = (Tables)malloc(NUMTABS*sizeof(symbolTab));
	
	int i;
	for (i=0; i<NUMTABS; ++i) {
		T[i].count = 0;
		strcpy(T[i].funLex, "");
		T[i].ste = NULL;
	}
	
	return T;
}

symbolTab* addToSymTab(symbolTab st, symTabEntry ste) {
	symbolTab* ST;
	tokenInfo ti;
	if (st.ste == NULL) {
		st.ste = (symTabEntry*)malloc(MAXENTRY*sizeof(symTabEntry));
		st.ste[st.count] = ste;
		ti.addr = st.count;
		st.count++;
	} else {
		st.ste[st.count] = ste;
		ti.addr = st.count;
		st.count++;
	}
	ST = (symbolTab*)malloc(sizeof(symbolTab));
	*ST = st;
	
	return ST;
}

void printSymTab(Tables T) {
	int i=0, j;
	while (strcmp(T[i].funLex, "") != 0) {
		printf("%s\n", T[i].funLex);
		for (j=0; j<T[i].count; ++j) {
			printf("%d, %d, %s\n", T[i].ste[j].tkid, T[i].ste[j].lineNum, T[i].ste[j].lex);
		}
		printf("\n");
		i++;
	}
}
