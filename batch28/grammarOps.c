/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parserDef.h"
#include <malloc.h>
//#include "lexerDef.h"

int ParseTable[HASHTABLE_SIZE][STATES];
char *FirstHC[FIHC] = {"stmts", "otherStmts", "stmt", "funCallStmt"};
int FirstHCV[FIHC][FIRST_SET_SIZE] = { {12, 96, 106, 156, 34, 42, 101, 17, 39, 110, 7, 9999},
					{106, 156, 34, 42, 17, 39, 110, 101, 9999},
					{106, 156, 34, 42, 17, 39, 110, 101, 9999},
					{101, 110, 9999} };

char *FollowHC[FOHC] = {"declarations", "fieldDefinition", "stmt", "assignmentStmt", "iterativeStmt", "conditionalStmt", "ioStmt", "funCallStmt",
					"inputParameters", "idList", "more_ids", "elsePart", "term", "factor", "termPrime", "all", "otherStmts", "remaining_list", "parameter_list", "typeDefinitions"};
int FollowHCV[FOHC][FOLLOW_SET_SIZE] = { {106, 156, 110, 101, 34, 42, 17, 39, 7},
						{77, 96},
						{106, 156, 34, 42, 17, 39, 110, 101, 9999, 7, 70, 170, 82},
						{106, 156, 34, 42, 17, 39, 110, 101, 9999, 7, 70, 170, 82},
						{106, 156, 34, 42, 17, 39, 110, 101, 9999, 7, 70, 170, 82},
						{106, 156, 34, 42, 17, 39, 110, 101, 9999, 7, 70, 170, 82},
						{106, 156, 34, 42, 17, 39, 110, 101, 9999, 7, 70, 170, 82},
						{106, 156, 34, 42, 17, 39, 110, 101, 9999, 7, 70, 170, 82},
						{106, 156, 34, 42, 17, 39, 110, 101, 9999, 7, 70, 170, 82, 113},
						{111},
						{111},
						{106, 156, 34, 42, 17, 39, 110, 101, 9999, 7, 70, 170, 82},
						{113, 116, 117, 118},
						{113, 116, 117, 118, 119, 120},
						{113, 116, 117, 118},
						{113, 116, 117, 118, 119, 120},
						{7, 70, 170, 82},
						{111},
						{111}, 
						{96, 106, 156, 110, 101, 34, 42, 17, 39, 7}};

Lexeme* lexemize(char* str)
{
	Lexeme* L;
	int i;
	L=(Lexeme*)malloc(sizeof(Lexeme));
	strcpy(L->lexName, str);
	L->next = NULL;
	for(i=0; i<strlen(str); i++)
	{
		if(isdigit(str[i]) == 0)
		{
			L->isTerminal = FALSE;
			return L;
		}
	}
	L->isTerminal = TRUE;
	return L;
}

void printSet(int* A) {
	int i=0;
	for (i=0; i<FIRST_SET_SIZE; ++i)
		printf("%d ", A[i]);
	printf("\n");
}	

Lexeme* addRHStoList(Lexeme* Head, Lexeme RHS)								//list for storing RHS of production
{
	if(Head == 0)
	{
		Lexeme* temp;
		temp = (Lexeme*)malloc(sizeof(Lexeme));
		strcpy(temp->lexName, RHS.lexName);
		temp->isTerminal = RHS.isTerminal;
		temp->next = 0;
		return temp;
	}
	
	else
	{
		Lexeme* temp;
		temp = Head;
		while(temp->next != 0)
		{
			temp = temp->next;
		}
		temp->next = (Lexeme*)malloc(sizeof(Lexeme));
		temp = temp->next;
		strcpy(temp->lexName, RHS.lexName);
		temp->isTerminal = RHS.isTerminal;
		temp->next = 0;
		return Head;
	}
}

Hashtable initializeHashtable(Hashtable H, int Hashtablesize)
{
	int i,j;
	H = (Hashtable)malloc(HASHTABLE_SIZE*sizeof(HeadLexeme));
	
	for(i=0; i<Hashtablesize; i++)
	{
		strcpy(H[i].lexemeName, "");
		for(j=0; j<HPR; j++)
		{
			H[i].nextLexeme[j] = NULL;
		}
	}
	
	return H;
}

int stringHashFunction(char* str, int Hashtablesize)
{
	int hash = 5381;										//djb2 hash
	int c;
	while((c = *str++))
		hash = ((hash<<5) + hash) + c;
	
	return (unsigned int)hash%Hashtablesize;
}

int getNextAddress(int index, int j, int hashtablesize)
{
	return (int)((index + j)%hashtablesize);
}


Hashtable addLexemetoTable(Hashtable H, Lexeme L, Lexeme* RHShead, int Hashtablesize)
{
	int index;
	int j;
	int k=0;
	
	index = stringHashFunction(L.lexName, HASHTABLE_SIZE);
	if (strcmp(L.lexName, "logicalOp") == 0) {
		//printf("HERE %s - %s \n", L.lexName, H[index].lexemeName);
	}
	
	if(strcmp(H[index].lexemeName, "") == 0)
	{
		strcpy(H[index].lexemeName, L.lexName);
		j=0;
		while(H[index].nextLexeme[j] != 0 && j<HPR)						//if index found is empty directly
		{
			j++;
		}
		if(j>=HPR)
		{
			printf("Terminal at hashindex %d needs more than HPR pointers! Error", index);
			return H;
		}
		H[index].nextLexeme[j] = RHShead;
		return H;
	}
	else if(strcmp(H[index].lexemeName, L.lexName) == 0)				// if both have same name(means production is of form A --> XYZ | PQR) find next free pointer and add the RHS there
	{
		for(j=0;j<HPR; j++)
		{
			if(H[index].nextLexeme[j] == NULL)
			{
				H[index].nextLexeme[j] = RHShead;
				return H;
			}
		}
		
		if(j>=HPR)
		{
			printf("Terminal at hashindex %d needs more than HPR pointers! Error", index);
			return H;
		}
		
	}
	
	else								//need to probe
	{
		do
		{
			index = getNextAddress(index, k, HASHTABLE_SIZE);
			k++;
			if(k==Hashtablesize)
			{
				printf("Hashtable full!!\n");
				return NULL;
			}
			if(strcmp(H[index].lexemeName, L.lexName) == 0)
			{
				for(j=0;j<HPR; j++)
				{
					if(H[index].nextLexeme[j] == NULL)
					{
						H[index].nextLexeme[j] = RHShead;
						return H;
					}
				}
		
				if(j>=HPR)
				{
					printf("Terminal at hashindex %d needs more than HPR pointers! Error", index);
					return H;
				}
			}
			
		}while(strcmp(H[index].lexemeName, "") != 0  && k<Hashtablesize);
		
		strcpy(H[index].lexemeName, L.lexName);
		
		j=0;
		while(H[index].nextLexeme[j] != 0 && j<HPR)
		{
			j++;
		}
		if(j>=HPR)
		{
			printf("Terminal at hashindex %d needs more than HPR pointers! Error", index);
			return H;
		}
		H[index].nextLexeme[j] = RHShead;
		return H;
	}
	
	
	
	return H;
}

int findinHashtable(Hashtable H, Lexeme L, int hashtablesize)
{
	int index;
	int j = 0;
	index = stringHashFunction(L.lexName, hashtablesize);
	if(strcmp(H[index].lexemeName, L.lexName) == 0)
	{
		return index;
	}
	
	else
	{
		do
		{
			index = getNextAddress(index, j, hashtablesize);
			j++;
		}while(strcmp(H[index].lexemeName, L.lexName) != 0 && j<hashtablesize);
	}
	
	if(j>=hashtablesize)
	{
		//printf("NOT FOUND IN HASHTABLE: ERROR!!");
		return -1;
	}
	else
		return index;
}

boolean ifPresent(int* arr, int elem, int arrsize)
{
	int i;
	for(i=0; i<arrsize; i++)
	{
		if(arr[i] == elem)
		{
			return TRUE;
		}
		else
		continue;
	}
	return FALSE;
}

int fsc = 0;
int* setUnion(int*, int*);
int* getFirstSet(int*, Hashtable, Lexeme);
int* getFollowSet(Hashtable, Lexeme);

int* getfirstSet(int* firstSet, int firstsetsize, Hashtable H, Lexeme L, int hashtablesize)
{
	int *tset;
	tset = (int*)malloc(FIRST_SET_SIZE*sizeof(int));
	if (L.isTerminal == TRUE) {
		tset[0] = atoi(L.lexName);
		firstSet = setUnion(firstSet, tset);
		return firstSet;
	}
	int index;
	int j=0;
	
	Lexeme* temp;
	index = findinHashtable(H, L, hashtablesize);
	while(H[index].nextLexeme[j] != 0 && j<HPR)
	{
	 	temp = H[index].nextLexeme[j];
	 	if(temp->isTerminal==TRUE)
	 	{
	 		if(ifPresent(firstSet, atoi(temp->lexName), firstsetsize)==FALSE)
	 		{
		 		firstSet[fsc] = atoi(temp->lexName);
		 		fsc++;
		 		j++;
		 		continue; 
	 		}
	 		else continue;
	 	}
	 	else if(temp->isTerminal==FALSE)
	 	{
	 		firstSet = getFirstSet(firstSet, H, *temp);
	 		j++;
	 	}	
	}
	
	return firstSet;
}

void printHash(Hashtable H) {
	int i, j;
	for (i=0; i<HASHTABLE_SIZE; ++i) {
		printf("%d - %s\n", i, H[i].lexemeName);
		for (j=0; j<HPR; ++j) {
			Lexeme* l;
			l = H[i].nextLexeme[j];
			if (l == NULL) {
				printf("%d : \n", j);
				continue;
			}
			do {
				printf("%d : %s ", j, l->lexName);
				l = l->next;
			} while (l != NULL);
			printf("\n");
		}
	}
}

int* setUnion(int* A, int* B) {
	int i = 0, j = 0;
	
	int k = 0;
	while (B[k]!=0)
		k++;
	
	for (i=0; A[i]!=0; ++i) {
		for (j=0; B[j]!=0; ++j) {
			if (A[i] == B[j])
				break;
		}
		if (B[j] == 0)
			B[j] = A[i];
	}				
	
	return B;
}

char timeslex[30];

int* findForFollow(Hashtable H, Lexeme L) {
	int i, j;
	int* set;
	int *fset;
	fset = (int*)malloc(FIRST_SET_SIZE*sizeof(int));
	set = (int*)malloc(FOLLOW_SET_SIZE*sizeof(int));
	for (i=0; i<FIRST_SET_SIZE; ++i) {
		fset[i] = 0;
		set[i] = 0;
	}

	for (i=0; i<HASHTABLE_SIZE; ++i) {
		for (j=0; j<HPR; ++j) {
			Lexeme* l;
			l = H[i].nextLexeme[j];
			if (l == NULL) {
				continue;
			}
			do {
				if (strcmp(L.lexName, l->lexName) == 0) {
					//printf("%s\n", H[i].lexemeName);
					if (l->next != NULL) {
						fset = getFirstSet(fset, H, *(l->next));
						set = setUnion(set, fset);
					} else {
						if (strcmp(l->lexName, H[i].lexemeName) == 0)
						{}
						else {
							fset = getFollowSet(H, *(lexemize(H[i].lexemeName)));
							set = setUnion(set, fset);
						}
					}
				}
				l = l->next;
			} while (l != NULL);
		}
	}
	
	return set;
}

int* getFirstSet(int* firstSet, Hashtable H, Lexeme L) {
	int i;
	for (i=0; i<FIHC; ++i) {
		if (strcmp(L.lexName, FirstHC[i]) == 0) {
			return FirstHCV[i];
		}
	}
	
	return getfirstSet(firstSet, FIRST_SET_SIZE, H, L, HASHTABLE_SIZE);
}

int* getFollowSet(Hashtable H, Lexeme L) {
	int i;
	for (i=0; i<FOHC; ++i) {
		if (strcmp(L.lexName, FollowHC[i]) == 0) {
			return FollowHCV[i];
		}
	}
	return findForFollow(H, L);
}

void initializeParseTable(int ParseTable[HASHTABLE_SIZE][STATES])
{
	int i,j;
	for(i=0; i<HASHTABLE_SIZE; i++)
	{
		for(j=0; j<STATES; j++)
		{
			ParseTable[i][j] = -999;
		}
	}
}

int* initwithZeroes(int* arr, int arrsize)
{	
	int i;
	for(i=0; i<arrsize; i++)
	{
		arr[i] = 0;
	}
	return arr;
}

int populateParseTable(int ParseTable[HASHTABLE_SIZE][STATES], Hashtable H, int terminal, Lexeme L)
{
	fsc = 0;
	int index; int i,k;
	index = findinHashtable(H, L, HASHTABLE_SIZE);
	Lexeme* temp;
	Lexeme* tt;
	Lexeme* lex;
	lex = lexemize(H[index].lexemeName);
	int* firstset;
	firstset = (int*)malloc(FIRST_SET_SIZE*sizeof(int));
	int* follset;
	follset = (int*)malloc(FOLLOW_SET_SIZE*sizeof(int));
	
	int found = 0;
	
//	printf("%d\n", terminal);
	for(i=0; H[index].nextLexeme[i]!=0; i++)
	{
		temp = H[index].nextLexeme[i];
		tt = H[index].nextLexeme[i];
		while(temp!=NULL)
		{
			firstset = (int*)malloc(FIRST_SET_SIZE*sizeof(int));
//			firstset = initwithZeroes(firstset, FIRST_SET_SIZE);
			firstset = getFirstSet(firstset, H, *temp);
			//printSet(firstset);
			if((ifPresent(firstset, terminal, FIRST_SET_SIZE))==TRUE && (ifPresent(firstset, 9999, FIRST_SET_SIZE)==FALSE))
			{
				found = 1;
				ParseTable[index][terminal] = i;
				//printf("1st case: no eps present!\n");
				return index;
				break;
			}
			else if((ifPresent(firstset, terminal, FIRST_SET_SIZE))==TRUE && (ifPresent(firstset, 9999, FIRST_SET_SIZE))==TRUE)
			{
				ParseTable[index][terminal] = i;
				//printf("2nd case: element and eps both present!\n");
//				temp=temp->next;
				return index;
				continue;
			}
			else if((ifPresent(firstset, terminal, FIRST_SET_SIZE))==FALSE && (ifPresent(firstset, 9999, FIRST_SET_SIZE))==TRUE)
			{
				temp=temp->next;
				//printf("3rd case: element not present but eps present!\n");
				continue;
			}
			else break;
		}
		if(temp==NULL && (ifPresent(firstset, 9999, FIRST_SET_SIZE))==TRUE)
		{
//			follset = initwithZeroes(follset, FOLLOW_SET_SIZE);
			follset = (int*)malloc(FIRST_SET_SIZE*sizeof(int));
			follset = getFollowSet(H, L);
			if (strcmp(tt->lexName, "9999") == 0) {
				for(k=0; k<FOLLOW_SET_SIZE; k++)
				{
					if(follset[k] == 9999 || follset[k]== 0 || ParseTable[index][follset[k]] != -999)
						continue;
					ParseTable[index][follset[k]] = i;
				}
			}
		}
	}
	
	return index;
}



void printParseTable(int ParseTable[HASHTABLE_SIZE][STATES], FILE* fp)
{
	int i,j;
	for(i=0; i<HASHTABLE_SIZE; i++)
	{
		for(j=0; j<STATES; j++)
		{
			fprintf(fp, "%d ", ParseTable[i][j]);
		}
		fprintf(fp, "\n");
	}
}

