/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbolDef.h"
#include "grammarOps.h"

int label = 0;
char t[30];
int flag = 0;
int flag1 = 0;

int offsetOf(Tables T, char id[]) {
	int i=0, j=0;
	for (i=0; strcmp(T[i].funid, "") != 0; ++i) {
		for (j=0; j<T[i].count; ++j) {
			if (strcmp(T[i].e[j].sym, id) == 0) {
				return T[i].e[j].offset;
			}
		}
	}
	
	return -1;
}

FILE* arithGen(FILE* fp, astNode root, Tables T) {
	if (root == NULL)
		return fp;
		
	astNode hc = root->headChild;
	
	while (hc != NULL) {
		arithGen(fp, hc, T);
		hc = hc->nextChild;
	}
	
	if (strcmp(root->ti.lex, "") != 0) {
//		printf(">> %s\n", root->ti.lex);
		if (root->ti.tkid == 106 || root->ti.tkid == 156)
			fprintf(fp, "\tMOV AX, DS:%d\n\tADD BX, -4\n\tMOV [BX], AX\n", offsetOf(T, root->ti.lex));
		else if (root->ti.tkid == 153 || root->ti.tkid == 150)
			fprintf(fp, "\tADD BX, -4\n\tMOV CX, %s\n\tMOV [BX], CX\n", root->ti.lex);
			
		else if (root->ti.tkid == 117) {
			fprintf(fp, "\tMOV AX, [BX]\n\tADD BX, 4\n\tADD AX, [BX]\n\tMOV [BX], AX\n");
		}
		else if (root->ti.tkid == 118) {
			fprintf(fp, "\tMOV AX, [BX]\n\tADD BX, 4\n\tSUB [BX], AX\n");
		}
		else if (root->ti.tkid == 119) {
			fprintf(fp, "\tMOV AX, [BX]\n\tADD BX, 4\n\tMOV CX, [BX]\n\tADD BX, 4\n\tMUL CX\n\tADD BX, -4\n\tMOV [BX], AX\n");
		}
		else if (root->ti.tkid == 120) {
			fprintf(fp, "\tMOV CX, [BX]\n\tADD BX, 4\n\tMOV AX, [BX]\n\tADD BX, 4\n\tDIV CX\n\tADD BX, -4\n\tMOV [BX], AX\n");
		}
	}
	
	return fp;
}

FILE* ifGen(FILE* fp, astNode root, Tables T) {
	astNode temp = root;
	
	if (temp == NULL)
		return fp;
	
	astNode hc = temp->headChild;
	
	while (hc != NULL) {
		ifGen(fp, hc, T);
		hc = hc->nextChild;
	}
	
	if (temp->ti.tkid == 106 || temp->ti.tkid == 156) {
		fprintf(fp, "\tMOV AX, DS:%d\n\tADD BX, -4\n\tMOV [BX], AX\n", offsetOf(T, temp->ti.lex));
	}
	if (temp->ti.tkid == 150 || temp->ti.tkid == 153) {
		fprintf(fp, "\tMOV AX, %s\n\tADD BX, -4\n\tMOV [BX], AX\n", temp->ti.lex);
	}
	
	if (temp->ti.tkid == 127 || temp->ti.tkid == 123 || temp->ti.tkid == 132 || temp->ti.tkid == 130 || temp->ti.tkid == 129 || temp->ti.tkid == 134) {
		fprintf(fp, "\tMOV CX, [BX]\n\tADD BX, 4\n\tMOV AX, [BX]\n\tADD BX, 4\n\t.if AX %s CX\n\t\tADD BX, -4\n\t\tMOV [BX], 1\n\t.else\n\t\tADD BX, -4\n\t\tMOV [BX], 0\n\t.endif\n", temp->ti.lex);
	}
	
	if (temp->ti.tkid == 137)
		fprintf(fp, "\tMOV CX, [BX]\n\tADD BX, 4\n\tMOV AX, [BX]\n\tADD BX, 4\n\t.if AX == 1 && CX == 1\n\t\tADD BX, -4\n\t\tMOV [BX], 1\n\t.else\n\t\tADD BX, -4\n\t\tMOV [BX], 0\n\t.endif\n");
	if (temp->ti.tkid == 140)
		fprintf(fp, "\tMOV CX, [BX]\n\tADD BX, 4\n\tMOV AX, [BX]\n\tADD BX, 4\n\t.if AX == 1 || CX == 1\n\t\tADD BX, -4\n\t\tMOV [BX], 1\n\t.else\n\t\tADD BX, -4\n\t\tMOV [BX], 0\n\t.endif\n");
	if (temp->ti.tkid == 121)
		fprintf(fp, "\tMOV AX, [BX]\n\tADD BX, 4\n\t.if AX == 0\n\t\tADD BX, -4\n\t\tMOV [BX], 1\n\t.else\n\t\tADD BX, -4\n\t\tMOV [BX], 0\n\t.endif\n");
	
	return fp;
}	

void generateCode(FILE* fp, astNode root, Tables T) {
	astNode temp = root;
	
	if (temp == NULL)
		return;
		
	astNode hc = temp->headChild;
	
	if (strcmp(temp->ti.lex, t) == 0 && flag == 1) {
		flag = 0;
		fprintf(fp, "\t.endif\n");
	}
	
	if (strcmp(temp->ti.lex, t) == 0 && flag1 == 1) {
		flag1 = 0;
		fprintf(fp, "\tjmp Label%d\n\t.endif\n", label);
		label++;
	}

	if (temp->ti.tkid == 126) {
		fp = arithGen(fp, temp->headChild->nextChild, T);
		fprintf(fp, "\tMOV AX, [BX]\n\tADD BX, 4\n\tMOV DS:%d, AX\n", offsetOf(T, temp->headChild->ti.lex));
	}
/*	
	if (temp->ti.tkid == 17) {
		fprintf(fp, "mov al, 1h\nint 21h\nmov [BP + %d], DL\nmov al, 1h\nint 21h\nmov [BP + %d + 1], DL\n", offsetOf(T, hc->ti.lex), offsetOf(T, hc->ti.lex));
	}
	
	if (temp->ti.tkid == 39) {
		fprintf(fp, "mov DL, [BP + %d]\nmov al, 2h\nint 21h\nmov DL, [BP + %d + 1]\nmov al, 2h\nint 21h\n", offsetOf(T, hc->ti.lex), offsetOf(T, hc->ti.lex));
	}
*/
	if (temp->ti.tkid == 42) {
		strcpy(t, temp->nextChild->ti.lex);
		fp = ifGen(fp, temp->headChild, T);
		fprintf(fp, "\tMOV AX, [BX]\n\tADD BX, 4\n\t.if AX == 1\n");
	}

	if (temp->ti.tkid == 82) {
		fprintf(fp, "\t.else\n");
		flag = 1;
	}
	
	if (temp->ti.tkid == 34) {
		fprintf(fp, "Label%d:\n", label);
		flag1 = 1;
		strcpy(t, temp->nextChild->ti.lex);
		fp = ifGen(fp, temp->headChild, T);
		fprintf(fp, "\tMOV AX, [BX]\n\tADD BX, 4\n\t.if AX == 1\n");
	}

	while (hc != NULL) {
		generateCode(fp, hc, T);
		hc = hc->nextChild;
	}
}
