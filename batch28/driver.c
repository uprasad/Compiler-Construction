/****************************

BATCH #28
ABHIJIT LAL : 2009B3A7577P
UDBHAV PRASAD : 2009B4A7523P

PLCC Compiler Project FINAL

****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symbol.h"
#include "grammarOps.h"
#include "lexer.h"
#include "codeGen.h"
#include "parser.h"

char *filename;

int Term[53] = { 
    TK_ASSIGNOP,	// 1
	TK_COMMENT,
	TK_FIELDID,
	TK_ID,
	TK_NUM,
	TK_RNUM,
	TK_FUNID,
	TK_RECORDID,
	TK_WITH,
	TK_PARAMETERS,	// 10
	TK_END,
	TK_WHILE,
	TK_INT,
	TK_REAL,
	TK_TYPE,
	TK_MAIN,
	TK_GLOBAL,
	TK_PARAMETER,
	TK_LIST,
	TK_SQL,		// 20
	TK_SQR,
	TK_INPUT,
	TK_OUTPUT,
	TK_SEM,
	TK_COLON,
	TK_DOT,
	TK_ENDWHILE,
	TK_OP,
	TK_CL,
	TK_IF,		// 30
	TK_THEN,
	TK_ENDIF,
	TK_READ,
	TK_WRITE,
	TK_RETURN,
	TK_PLUS,
	TK_MINUS,
	TK_MUL,
	TK_DIV,
	TK_CALL,	// 40
	TK_RECORD,
	TK_ENDRECORD,
	TK_ELSE,
	TK_AND,
	TK_OR,
	TK_NOT,
	TK_LT,
	TK_LE,
	TK_EQ,
	TK_GT,	// 50
	TK_GE,
	TK_NE,
	TK_COMMA
};

int main(int argc, char** argv)
{
	printf("\n\nLEVEL 4: Symbol Table, AST, Type Checker, Semantic Checker, Code Generation modules (ALL) work\n");
	printf("Press Any Key To Continue\n\n");
	getchar();

	if (argv[1] == NULL) {
		printf("Enter arguments in the format : %s <source_file> <ASM_out_file>\n", argv[0]);
		return 0;
	}
	
	FILE* fp;
	char bufferForG[GRAMBUF];
	char temp[30];
	int buffptr = 0;
	fp = fopen("grammar_n", "r");
	
	int i=0;
	
	Hashtable H = NULL;
	Lexeme *LHS;
	Lexeme *RHS;

	H = initializeHashtable(H, HASHTABLE_SIZE);
	
	int* firstSet;
	firstSet = (int*)malloc(FIRST_SET_SIZE*sizeof(int));
		
	while(!feof(fp))
	{
		fscanf(fp, "%s\n", bufferForG);
		if(bufferForG[buffptr]=='<')
		{
			buffptr++;
			do
			{
				temp[i++] = bufferForG[buffptr++];
				
			}while(bufferForG[buffptr] != '>');
			temp[i] = '\0';
			LHS = lexemize(temp);
			LHS->isTerminal = FALSE;
			strcpy(temp, "");
			i=0;
			
			Lexeme* RHShead=NULL;
			
			do
			{
				buffptr++;
				if(bufferForG[buffptr] == '<')
				{
					buffptr++;
					do
					{
						temp[i++] = bufferForG[buffptr++];
						
					}while(bufferForG[buffptr] != '>');
					
					temp[i] = '\0';
					RHS = lexemize(temp);
					RHS->isTerminal = FALSE;
					RHShead = addRHStoList(RHShead, *RHS);
					
					strcpy(temp, "");
					i=0;
				}
				else if(bufferForG[buffptr] == '[')
				{
					buffptr++;
					do
					{
						temp[i++] = bufferForG[buffptr++];
						
					}while(bufferForG[buffptr] != ']');	
					
					temp[i] = '\0';
					RHS = lexemize(temp);
					RHS->isTerminal = TRUE;
					RHShead = addRHStoList(RHShead, *RHS);
					strcpy(temp, "");
					i=0;
				}

			}while(bufferForG[buffptr]!= '\0');

			H = addLexemetoTable(H, *LHS, RHShead, HASHTABLE_SIZE);
			
		}
		buffptr++;
		strcpy(bufferForG, "");
		buffptr = 0;
	}
	fclose(fp);

	char endtok[10];
	strcpy(endtok, "TK_END");
	int k, l;
	int ParseTable[HASHTABLE_SIZE][STATES];

	initializeParseTable(ParseTable);
	Lexeme* lex;
	
	for(k=0; k<HASHTABLE_SIZE; k++)
	{
		if(strcmp(H[k].lexemeName, "") == 0)
			continue;
		else 
			lex = lexemize(H[k].lexemeName);

		for(l=0; l<53; l++)
		{
			populateParseTable(ParseTable, H, Term[l], *lex);
		}
	}
	
	FILE* fp1 = fopen("LAL", "w");
	printParseTable(ParseTable, fp1);
	
	filename = (char*)malloc(NAMELEN*sizeof(char));
	strcpy(filename, argv[1]);
	
//	Tables T = loadTransTableAndSymTab();
	
	buffer B[2];//= {NULL, NULL};
	B[1] = (buffer)malloc(BUF_SIZE*sizeof(char));
	B[2] = (buffer)malloc(BUF_SIZE*sizeof(char));
	buffersize bk = BUF_SIZE;
	
	for (i=0; i<2; ++i) {
		B[i] = (buffer)malloc(bk*SIZE);
	}
	
	FILE* dfaf = fopen("finale_dfa_more_changes.csv", "r");
	if (!dfaf) {
		printf("Finite automata file MISSING\n");
		return 0;
	}

	loadTransTable(dfaf);
	fclose(dfaf);
	
	FILE* _fp = fopen(argv[1], "r");
	
	if (_fp == NULL) {
		printf("Source file does not exist!\n");
		return 0;
	}

//	int x;
	int* first_set; Lexeme* le;
	le = lexemize("otherStmts");
	first_set = (int*)malloc(FIRST_SET_SIZE*sizeof(int)); 
	treeNode root = initParseTree("program");
//	Stack s = initStack();
	root = _parseInput(ParseTable, H, B, bk, _fp);
	
	fclose(_fp);
	
	Hashtable ASTtable = NULL;
	FILE* fp_ast = fopen("ast_rules", "r");

	int buf_ctr = 0;
	strcpy(temp, "");
	i=0;
	char tchar;
	ASTtable = initializeHashtable(ASTtable, HASHTABLE_SIZE);
	
	while(!feof(fp_ast))
	{
		char bufferforAST[GRAMBUF];
		buf_ctr = 0;
		fscanf(fp_ast, "%s\n", bufferforAST);
		if(bufferforAST[buf_ctr]=='<')
		{
			buf_ctr++;
			do
			{
				temp[i++] = bufferforAST[buf_ctr++];
				
			}while(bufferforAST[buf_ctr] != '>');
			
			temp[i] = '\0';
			LHS = lexemize(temp);
			strcpy(temp, "");
			i=0;
			Lexeme* Rhead=NULL;
			do
			{
				fscanf(fp_ast, "\t%s%c", temp, &tchar);
				if(tchar == EOF)
					break;
				RHS = lexemize(temp);
				Rhead = addRHStoList(Rhead, *RHS);
				strcpy(temp,"");
				i=0;
				//fp_ast--;
			}while(tchar!='\n');
			
			ASTtable = addLexemetoTable(ASTtable, *LHS, Rhead, HASHTABLE_SIZE);
		}
	}
	
	astNode astr = initAST("program", 0);
	astr = populateAST(root, ASTtable);

//	printParseTree(root, 0);

	int user, token;
	bool done = false;
	
	//******************************
	Tables T = initSymbolTable();
	T = populateSymbolTable(astr, T, 0);
	T = fillOffset(T);
	
	FILE* ptfp = fopen(argv[2], "w");
	if (ptfp == NULL) {
		printf("Enter arguments in the format : %s <source_file> <ASM_out_file>\n", argv[0]);
		return 0;
	}
	
	FILE* fp_AST = fopen("atree.txt", "w");

	FILE* asmfp;
	tokenInfo ti;
	while(done == false) {
		printf("\nChoose an option :\n");
		printf("\n 1. Print tokens.\n 2. Print parse tree.\n 3. Print abstract syntax tree.\n 4. Print symbol table\n 5. Type and semantic checking\n 6. Generate code\n 7. Quit\n\nYour choice : ");
		scanf("%d", &user);
		
		switch (user) {
		case 1:
			printf("\nToken List:\n\n");
			FILE* userfp = fopen(argv[1], "r");
			for (i=0; i<2; ++i) {
				B[i] = (buffer)malloc(bk*SIZE);
			}
			forward = BUF_SIZE;
			do {
				ti = getNextToken(userfp, B, bk);
				printf("%d :::: %s\n", ti.tkid, ti.lex);
				token = ti.tkid;
			} while (token != -2 && token != -1);
			
			if (token != -1)
				printf("%s\n", endtok);
			fclose(userfp);
			
			break;
			
		case 2:
			printf("Parse Tree:\n");
			FILE* fp_parser = fopen("ptree.txt", "w");
			if(fp_parser == NULL)
			{
				printf("Please specify the file to print parse tree\n");
				break;
			}
			if (_done == 1)
				printf("Compiled successfully, Parse tree printed in file %s\n", argv[2]);
			fprintf(fp_parser, "%-10s %-20s%-20s\t\t\t%-20s%-15s\n", "LINE NO", "TOKEN", "parentNode symbol", "isLeafNode", "Node symbol"); 
			fprintf(fp_parser, "%s\n", "***********************************************************************************************");
			printParseTree2(root, fp_parser, 0);
			fclose(fp_parser);
			break;
			
		case 3:
			printf("Abstract Syntax Tree:\n");
			printf("\nSize in bytes of AST: %ld\n", sizeof(_astNode)*printAST(fp_AST, astr, 0));
			printf("AST printed in file %s\n", "atree.txt");
			fclose(fp_AST);
			break;
			
		case 7:
			done = true;
			break;
		
		case 4:
			printSymbolTable(T);
			break;
			
		case 5:
			typeChecker(T, astr);
			assgnOp(T, astr);
			boolExp(T, astr);
			sem_ii(T, astr, 0);
			if (checkOverload(T) != 0)
				printf("Function overloading not allowed!\n");
			matchReturnType(T, astr);
			//glob_astr = astr;
			funInputCheck(T, astr, astr);
			break;
			
		case 6:
			asmfp = fopen(argv[2], "w");
			fprintf(asmfp, ".model small\n.code\nstart:\n");
			generateCode(asmfp, astr, T);
			fprintf(asmfp, "\tMOV AX, 4C00h\n\tint 21h\n");
			fprintf(asmfp, "end start\n");
			fclose(asmfp);
			break;
			
		default:
			printf("\nChoose from 1-7 please!\n");
		}
	}
	
	return 0;
}
