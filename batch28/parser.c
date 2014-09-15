/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "lexerDef.h"
//#include "parseTreeDef.h"
#include "grammarOps.h"
#include "lexer.h"

int _done = 0;

char* reverseEnum(int token)
{
	char* tk_id;
	tk_id = (char*)malloc(30*sizeof(char));
		
	switch(token){
		case 126: 
			strcpy(tk_id, "TK_ASSIGNOP");
			break;
		case 158: 
			strcpy(tk_id, "TK_COMMENT");
			break;
		case 109: 
			strcpy(tk_id, "TK_FIELDID");
			break;
		case 106: 
			strcpy(tk_id, "TK_ID");
			break;
		case 150: 
			strcpy(tk_id, "TK_NUM");
			break;
		case 153: 
			strcpy(tk_id, "TK_RNUM");
			break;
		case 167: 
			strcpy(tk_id, "TK_FUNID");
			break;
		case 156: 
			strcpy(tk_id, "TK_RECORDID");
			break;
		case 29: 
			strcpy(tk_id, "TK_WITH");
			break;
		case 61: 
			strcpy(tk_id, "TK_PARAMETERS");
			break;
		case 64: 
			strcpy(tk_id, "TK_END");
			break;
		case 34: 
			strcpy(tk_id, "TK_WHILE");
			break;
		case 49: 
			strcpy(tk_id, "TK_INT");
			break;
		case 15: 
			strcpy(tk_id, "TK_REAL");
			break;
		case 96: 
			strcpy(tk_id, "TK_TYPE");
			break;
		case 147: 
			strcpy(tk_id, "TK_MAIN");
			break;
		case 24: 
			strcpy(tk_id, "TK_GLOBAL");
			break;
		case 60: 
			strcpy(tk_id, "TK_PARAMETER");
			break;
		case 87: 
			strcpy(tk_id, "TK_LIST");
			break;
		case 110: 
			strcpy(tk_id, "TK_SQL");
			break;
		case 111: 
			strcpy(tk_id, "TK_SQR");
			break;
		case 47: 
			strcpy(tk_id, "TK_INPUT");
			break;
		case 165: 
			strcpy(tk_id, "TK_OUTPUT");
			break;
		case 113: 
			strcpy(tk_id, "TK_SEM");
			break;
		case 112: 
			strcpy(tk_id, "TK_COLON");
			break;
		case 114: 
			strcpy(tk_id, "TK_DOT");
			break;
		case 70: 
			strcpy(tk_id, "TK_ENDWHILE");
			break;
		case 115: 
			strcpy(tk_id, "TK_OP");
			break;
		case 116: 
			strcpy(tk_id, "TK_CL");
			break;
		case 42: 
			strcpy(tk_id, "TK_IF");
			break;
		case 92: 
			strcpy(tk_id, "TK_THEN");
			break;
		case 170: 
			strcpy(tk_id, "TK_ENDIF");
			break;
		case 17: 
			strcpy(tk_id, "TK_READ");
			break;
		case 39: 
			strcpy(tk_id, "TK_WRITE");
			break;
		case 7: 
			strcpy(tk_id, "TK_RETURN");
			break;
		case 117: 
			strcpy(tk_id, "TK_PLUS");
			break;
		case 118: 
			strcpy(tk_id, "TK_MINUS");
			break;
		case 119: 
			strcpy(tk_id, "TK_MUL");
			break;
		case 120: 
			strcpy(tk_id, "TK_DIV");
			break;
		case 101: 
			strcpy(tk_id, "TK_CALL");
			break;
		case 12: 
			strcpy(tk_id, "TK_RECORD");
			break;
		case 77: 
			strcpy(tk_id, "TK_ENDRECORD");
			break;
		case 82: 
			strcpy(tk_id, "TK_ELSE");
			break;
		case 137: 
			strcpy(tk_id, "TK_AND");
			break;
		case 140: 
			strcpy(tk_id, "TK_OR");
			break;
		case 121: 
			strcpy(tk_id, "TK_NOT");
			break;
		case 127: 
			strcpy(tk_id, "TK_LT");
			break;
		case 123: 
			strcpy(tk_id, "TK_LE");
			break;
		case 132: 
			strcpy(tk_id, "TK_EQ");
			break;
		case 130: 
			strcpy(tk_id, "TK_GT");
			break;
		case 129: 
			strcpy(tk_id, "TK_GE");
			break;
		case 134: 
			strcpy(tk_id, "TK_NE");
			break;
		case 171: 
			strcpy(tk_id, "TK_COMMA");
			break;
		case 9999:
			strcpy(tk_id, "EPS");
			break;
		case -1:
			return("");
		case -2:
//			printf("Tokenization done!\n");
			return("");
	}
	return tk_id;
}

treeNode initParseTree(char* lex) {
	treeNode root;
	root = (treeNode)malloc(sizeof(_treeNode));

	strcpy(root->lex, lex);
	root->parent = NULL;
	root->prodno = -1;
	
	int i;
	for (i=0; i<NUMCHILD; ++i) {
		root->child[i] = NULL;
	}
	
	return root;
}

Stack initStack() {
	Stack s;
	s = (Stack)malloc(sizeof(_Stack));
	s->count = 0;
	s->head = NULL;
	
	return s;
}

Stack push(Stack s, char* lex) {
	if (s->head == NULL) {
		s->head = (item)malloc(sizeof(_item));
		strcpy(s->head->lex, lex);
		s->head->next = NULL;
		s->count++;
		return s;
	} else {
		item it;
		it = (item)malloc(sizeof(_item));
		it->next = s->head;
		strcpy(it->lex, lex);
		s->head = it;
		s->count++;
		return s;
	}
}

void printStack(Stack s) {
	if (s->head == NULL || s == NULL) {
		printf("EMPTY!!\n");
		return;
	}
	item ts = s->head;
	do {
		printf("%s\n", ts->lex);
		ts = ts->next;
	} while (ts != NULL);
}

Stack pop(Stack s) {
	if (s->head == NULL) {
		printf("ERROR! Nothing to POP\n");
		exit(0);
	}
	item ts;
	ts = s->head;
	s->head = s->head->next;
	
	free(ts);
	s->count--;
	return s;
}

char* top(Stack s) {
	if (s->head == NULL) {
		return NULL;
	}
	return s->head->lex;
}

bool isEmpty(Stack s) {
	if (s->head == NULL)
		return true;
	return false;
}

void printParseTree2(treeNode root, FILE* fp, int c) {
	treeNode temp = root;
	if (temp == NULL)
		return;	
	
	if(atoi(temp->lex)!=0)
	{
		//printf("HERE\n");
		fprintf(fp, "%d\t\t\t%-25s%-25sYES\t\t\t%-25s\n", temp->lineNum, reverseEnum(atoi(temp->lex)), temp->parent->lex, reverseEnum(atoi(temp->lex)));
		
	}
	else
	{
		//printf("THERE\n");
		if(temp->parent == NULL)
			fprintf(fp, "%d\t\t\t--------\t\t\t\t--------\t\t\t\t\tNO\t\t\t%-25s\n", temp->lineNum, temp->lex);
		else
		fprintf(fp, "%d\t\t\t--------\t\t\t\t%-25s  NO\t\t\t%-25s\n", temp->lineNum, temp->parent->lex, temp->lex);
	}	
	int i=0;
	while (i<NUMCHILD) {
		printParseTree2(temp->child[i], fp, c+1);
		i++;
	}
}

int printParseTree(treeNode root, int c) {
	static int lal = 0;
	treeNode temp = root;
	if (temp == NULL)
		return lal;
	int i=0;
	lal++;
	for (i=0; i<c; ++i) {
		printf("  ");
	}
	
	
	if(atoi(temp->lex)!=0)
		printf("%s : %s\n", reverseEnum(atoi(temp->lex)), temp->ti.lex);
	else
		printf("%s\n", temp->lex);
		
	i=0;
	while (i<NUMCHILD) {
		printParseTree(temp->child[i], c+1);
		i++;
	}
	return lal;
}

int printAST(FILE *fp, astNode root, int c) {
	static int lal = 0;
	astNode temp = root;
	if (temp == NULL)
		return lal;
	int i=0;
	lal++;
	for (i=0; i<c; ++i) {
		fprintf(fp, "%s", "\t");
	}
	
	if(atoi(temp->lex)!=0)
		fprintf(fp, "%s : %s\n", reverseEnum(atoi(temp->lex)), temp->ti.lex);
	else
		fprintf(fp, "%s\n", temp->lex);
	
	astNode tt = temp->headChild;
	
	while (tt != NULL) {
		printAST(fp, tt, c+1);
		tt = tt->nextChild;
	}
	return lal;
}

astNode initAST(char* lex, int prodno) {
	astNode astRoot;
	astRoot = (astNode)malloc(sizeof(_astNode));
	
	astRoot->prodno = prodno;
	strcpy(astRoot->lex, lex);
	
	astRoot->parent = NULL;
	astRoot->headChild = NULL;
	astRoot->nextChild = NULL;
	
	return astRoot;
}

astNode addChild(astNode root, astNode child) {	
	astNode temp = root;
	if (child == NULL)
		return root;
	
	if (temp->headChild == NULL) {
		temp->headChild = child;
		return temp;
	}
	
	astNode _tt = temp->headChild;
	
	while (_tt->nextChild != NULL) {
		_tt = _tt->nextChild;
	}
	
	_tt->nextChild = child;
	child->nextChild = NULL;
	
	return temp;
}

astNode populateAST(treeNode ptRoot, Hashtable ASTtable) {
	astNode *astemp;
	astemp = (astNode*)malloc(NUMCHILD*sizeof(_astNode));
	
	astNode astr = initAST(ptRoot->lex, ptRoot->prodno);
	astr->ti = ptRoot->ti;

	int i;
	
	for (i=0; i<NUMCHILD; ++i) {
		if (ptRoot->child[i] != NULL) {
			astemp[i] = initAST(ptRoot->child[i]->lex, ptRoot->child[i]->prodno);
			astemp[i]->ti = ptRoot->child[i]->ti;
		} else {
			astemp[i] = NULL;
		}
	}
	
	bool _term = true;
	bool cterm = true;
	for (i=0; i<NUMCHILD; ++i) {
		if (ptRoot->child[i] != NULL) {
			_term = false;
			if (atoi(ptRoot->child[i]->lex) == 0) {
				cterm = false;
			}
		}
	}
	
	if (_term == true)
		return astr;

	
	if (cterm != true)	{
		for (i=0; i<NUMCHILD; ++i) {
			if (ptRoot->child[i] != NULL)
				astemp[i] = populateAST(ptRoot->child[i], ASTtable);
			else
				astemp[i] = NULL;
		}
	}
	
	Lexeme* lx = lexemize(ptRoot->lex);
	
	int index = findinHashtable(ASTtable, *lx, HASHTABLE_SIZE);
	int choice = atoi(ASTtable[index].nextLexeme[ptRoot->prodno]->lexName);
	Lexeme* l = ASTtable[index].nextLexeme[ptRoot->prodno];
	
	int x;
	Lexeme *ln = l->next;
	
	switch(choice) {
	case 0:
		x = atoi(l->next->lexName);
		astr = astemp[x];
		
		ln = ln->next;
		while (ln != NULL) {
			astr = addChild(astr, astemp[atoi(ln->lexName)]);
			ln = ln->next;
		}

		return astr;
		break;
		
	case 1:
		for (x=0; x<NUMCHILD; ++x) {
			if (strcmp(ptRoot->child[x]->lex, ln->lexName) == 0) {
				break;
			}
		}
		
		for (i=0; i<x; ++i) {
			astr = addChild(astr, astemp[i]);
		}
		
		if (astemp[x] == NULL) {
			return astr;
		}
		
		astNode tempt = astr->headChild;
		
		while (tempt->nextChild != NULL) {
			tempt = tempt->nextChild;
		}
		tempt->nextChild = astemp[x]->headChild;

		return astr;
		break;
		
	case 2:
		for (x=0; x<NUMCHILD; ++x) {
			astr = addChild(astr, astemp[x]);
		}
		
		return astr;
		break;
		
	case 3:
		astr = NULL;
		
		return astr;
		break;
		
	}
	
	return astr;
}

treeNode _parseInput(int PT[HASHTABLE_SIZE][STATES], Hashtable H, buffer B[], buffersize bk, FILE* fp) {
//	char tlex[30];
	lineNumber = 1;
	Stack S_orig = initStack();
	Stack S_helper;
	int index, newindex;
	int nxttoken;
	Lexeme* temp; Lexeme* prog;
	Lexeme* iterator;
	prog = lexemize("program");
	S_helper = initStack();
	index = findinHashtable(H, *prog, HASHTABLE_SIZE);
	
	tokenInfo ti;

	ti = getNextToken(fp, B, bk);
	nxttoken = ti.tkid;
//	printf("%d, %s\n", ti.tkid, ti.lex);

	int nodcount = 0;
	
	treeNode root = initParseTree("program");
	treeNode tempNode = root;
	treeNode dummy;
	treeNode dum2[NUMCHILD];
	dummy = (treeNode)malloc(sizeof(_treeNode));
	
	int y;
	for (y=0; y<NUMCHILD; ++y) {
		dum2[y] = (treeNode)malloc(sizeof(_treeNode));
	}
	
	while (nxttoken == 158) {
		ti = getNextToken(fp, B, bk);
		nxttoken = ti.tkid;
		//printf("%d, %s\n", ti.tkid, ti.lex);
	}
	
	if(PT[index][nxttoken] == -999)
	{
		int *fset;
		int k =0;
		fset = (int*)malloc(FIRST_SET_SIZE*sizeof(int));
		fset = getFirstSet(fset, H, *prog);
		printf("\nAt line %d: The token '%d' for lexeme '%s' does not match\n", lineNumber, nxttoken, prog->lexName);
		printf("Expected token(s):\t"); 
		for(k=0; k<FIRST_SET_SIZE; k++)
		{
			if(fset[k]==0 || fset[k] == 9999)
				continue;
			printf("'%d' ", fset[k]);
		}
		//printf("\n");
		printf("\nIncorrect syntax at beginning!\n");
		return root;
	}
	iterator = H[index].nextLexeme[PT[index][nxttoken]];
	tempNode->prodno = PT[index][nxttoken];
	tempNode->lineNum = lineNumber;
//	strcpy(tempNode->lexc, lex);
	
	while(iterator!=NULL)
	{	
		S_helper = push(S_helper, iterator->lexName);
		tempNode->child[nodcount] = initParseTree(iterator->lexName);
		dummy = tempNode->child[nodcount];
		dummy->parent  = tempNode;
		iterator = iterator->next;
		nodcount++;
	}

	char* tempstr;
	tempstr = (char*)malloc(30*sizeof(char));
	while(isEmpty(S_helper) != true)
	{
		tempstr = top(S_helper);	
		S_orig = push(S_orig, tempstr);
		S_helper = pop(S_helper);
	}

	do
	{
//		printParseTree(tempNode, 0);
//		getchar();
		int p;
		for (p=0; p<NUMCHILD; ++p) {
			if (tempNode->child[p] != NULL) {
				if (strcmp(top(S_orig), tempNode->child[p]->lex) == 0) {
					if (tempNode->child[p]->child[0] != NULL)
						continue;
					tempNode = tempNode->child[p];
					break;
				}
			} else {
				dummy = tempNode->parent;
				for (y=0; y<NUMCHILD; ++y) {
					if (dummy->child[y] != NULL)
						dum2[y] = dummy->child[y];
				}
				tempNode = tempNode->parent;
				for (y=0; y<NUMCHILD; ++y) {
					if (tempNode->child[y] != NULL)
						tempNode->child[y] = dum2[y];
				}
				break;
			}
		}

		temp = lexemize(top(S_orig));

		S_helper = initStack();
		int flag = 0;
		int y;
		
//		printf("---Stack---\n");
//		printStack(S_orig);
//		printf("--- FIN ---\n\n");
		
		if(atoi(top(S_orig)) == 9999)
		{
			S_orig = pop(S_orig);
			do {
				flag = 0;
				tempNode = tempNode->parent;
				char *tops = (char*)malloc(30*sizeof(char));
				tops = top(S_orig);

				if (tops != NULL) {
					for (y=0; y<NUMCHILD; ++y) {
						if (tempNode->child[y] != NULL) {
							if (strcmp(tempNode->child[y]->lex, tops) == 0) {
								if (tempNode->child[y]->child[0] != NULL)
									continue;
								flag = 1;
								break;
							}
						}
					}
				} else break;
			} while (flag == 0);
			
			continue;
		}
		
		if( atoi(top(S_orig)) == nxttoken)
		{
			S_orig = pop(S_orig);
			tempNode->ti = ti;
			do {
				flag = 0;
				tempNode = tempNode->parent;
				char *tops = (char*)malloc(30*sizeof(char));
				tops = top(S_orig);
				if (tops != NULL) {
					for (y=0; y<NUMCHILD; ++y) {
						if (tempNode->child[y] != NULL) {
							if (strcmp(tempNode->child[y]->lex, tops) == 0) {
								if (tempNode->child[y]->child[0] != NULL)
									continue; 
								flag = 1;
								break;
							}
						}
					}
				} else break;
			} while (flag == 0);
			
			ti = getNextToken(fp, B, bk);
			nxttoken = ti.tkid;
//			printf("%d, %s\n", ti.tkid, ti.lex);
			
			if(nxttoken==-2)
				break;
			
			//printf("%d, %s, %s\n", nxttoken, reverseEnum(nxttoken), top(S_orig));
			
			continue;
		}
		newindex = findinHashtable(H, *temp, HASHTABLE_SIZE);
		while (nxttoken == 158) {
			ti = getNextToken(fp, B, bk);
			nxttoken = ti.tkid;
//			printf("%d, %s\n", ti.tkid, ti.lex);
		}
		
	 	if(temp->isTerminal == TRUE)
		{
			int *fset;
			int k =0;
			fset = (int*)malloc(FIRST_SET_SIZE*sizeof(int));
			fset = getFirstSet(fset, H, *temp);
			printf("\nAt line %d: The token '%s' for lexeme '%s' does not match\n", lineNumber, reverseEnum(nxttoken), reverseEnum(atoi(temp->lexName)));
			printf("Expected token(s):\t"); 
			for(k=0; k<FIRST_SET_SIZE; k++)
			{
				if(fset[k]==0 || fset[k] == 9999)
					continue;
				printf("'%s' ", reverseEnum(fset[k]));
			}
			//printf("\n");
			printf("ERROR_5: Cant parse further, Aborting...\n");
			return root;
		}
		else if(PT[newindex][nxttoken] == -999)
		{
			int *fset;
			int k =0;
			fset = (int*)malloc(FIRST_SET_SIZE*sizeof(int));
			fset = getFirstSet(fset, H, *temp);
			printf("\nAt line %d: The token '%s' for lexeme '%s' does not match\n", lineNumber, reverseEnum(nxttoken), temp->lexName);
			printf("Expected token(s): "); 
			for(k=0; k<FIRST_SET_SIZE; k++)
			{
				if(fset[k]==0 || fset[k] == 9999)
					continue;
				printf("'%s' ", reverseEnum(fset[k])); 
			}
			//printf("\n");
			printf("ERROR_5: Parse table entry doesnt exist, Aborting...\n");
			return root;
		}	
		else
		{
			nodcount = 0;
			iterator = H[newindex].nextLexeme[PT[newindex][nxttoken]];
			tempNode->prodno = PT[newindex][nxttoken];
			tempNode->lineNum = lineNumber;
			if(iterator == NULL)
			{
				
			}
			while(iterator!=NULL)
			{
				S_helper = push(S_helper, iterator->lexName);
				dummy = initParseTree(iterator->lexName);
				tempNode->child[nodcount] = dummy;
//				printf("--- %s\n", tempNode->child[nodcount]->lex);
				dummy->parent = tempNode;
				
				while (nxttoken == 158) {
					ti = getNextToken(fp, B, bk);
					nxttoken = ti.tkid;
//					printf("%d, %s\n", ti.tkid, ti.lex);
				}
				iterator = iterator->next;
				nodcount++;
			}
			char* tempstr;
			tempstr = (char*)malloc(30*sizeof(char));
			S_orig = pop(S_orig);
						
			while(isEmpty(S_helper) != true)
			{
				tempstr = top(S_helper);
				S_orig = push(S_orig, tempstr);
				S_helper = pop(S_helper);
			}
		}
	}while(isEmpty(S_orig)!=true);
	
	_done = 1;
	return root;
}

