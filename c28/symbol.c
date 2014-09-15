#include <stdlib.h>
#include <string.h>

#include "symbolDef.h"
#include "grammarOps.h"
#include "lexerDef.h"

void printSymbolTable(Tables T) {
	int i, j;
	printf("%-15s %-15s %-15s %-15s %-15s", "Lexeme", "Type", "Record name", "Offset", "Scope");
	for (i=0; i<MAX_TABLES; ++i) {
		if (strcmp(T[i].funid, "") == 0)
			break;
		//printf("Function : %s\n", T[i].funid);
		for (j=0; j<T[i].count; ++j) {
			printf("%-15s %-15s %-15s %-15d %-15s\n",T[i].e[j].sym, reverseEnum(T[i].e[j].type), T[i].e[j].rec, T[i].e[j].offset, T[i].e[j].funid);
		}
		printf("\n\n");
	}
}

Tables initSymbolTable() {
	Tables T;
	T = (Tables)malloc(MAX_TABLES*sizeof(Table));
	
	int i;
	for (i=0; i<MAX_TABLES; ++i) {
		strcpy(T[i].funid, "");
		T[i].count = 0;
	}
	
	strcpy(T[0].funid, "global");
	
	return T;
}

int getType2(Tables T, astNode node, int ind)
{
	int i, j;
	for(i=0; i<MAX_TABLES; i++)
	{
		if (i != ind && i != 0)
			continue;
		if (strcmp(T[i].funid, "") == 0)
			continue;
		for(j=0; j<T[i].count; j++)
		{
			if(node->ti.tkid == TK_NUM)
				return TK_INT;
			if(node->ti.tkid == TK_RNUM)
				return TK_REAL;
			if(strcmp(node->ti.lex, T[i].e[j].sym)==0)
				return T[i].e[j].type;
			if (strcmp(node->ti.lex, T[i].e[j].rec) == 0)
				return T[i].e[j].type;
		}
	}
	return 0; 
}

int getType(Tables T, astNode node);

void checkRedec(Tables T, astNode root, int ind) {
	astNode temp = root;
	astNode hc = temp->headChild;
	
	if (temp == NULL)
		return;
		
	if (strcmp(temp->lex, "106") == 0) {
		if (getType2(T, temp, ind) == 0) {
			printf("ERROR : Variable %s undeclared before use\n", temp->ti.lex);
		}
	}
	
	while (hc != NULL) {
		checkRedec(T, hc, ind);
		hc = hc->nextChild;
	}
}

int getOffset(Tables T, char rec[]) {
	int i, j;
	int off = 0;
	for (i=0; strcmp(T[i].funid, "") != 0; ++i) {
		for (j=0; j<T[i].count; ++j) {
			if (T[i].e[j].type != 156 && strcmp(T[i].e[j].rec, rec) == 0) {
				if (T[i].e[j].type == 49) {
					off += 2;
				} else if (T[i].e[j].type == 15) {
					off += 4;
				}
			}
		}
	}
	
	return off;
}

Tables fillOffset(Tables T) {
	int i, j;
	Entry e;
	
	int offset = 0;
	
	for (i=0; strcmp(T[i].funid, "") != 0; ++i) {
		for (j=0; j<T[i].count; ++j) {
			T[i].e[j].offset = offset;
			e = T[i].e[j];
			if (e.type == 49 && strcmp(e.rec, "") == 0)
				offset += 2;
			if (e.type == 15 && strcmp(e.rec, "") == 0)
				offset += 4;
			if (e.type == 156)
				offset += getOffset(T, e.rec);
		}
	}
	
	return T;
}

Tables populateSymbolTable(astNode root, Tables T, int ind) {
	astNode temp = root;
	astNode hc = temp->headChild;
	astNode thc;
	Entry e;
	
	int k=0; int flag=0, fun_flag=0;
	
	if (temp == NULL)
		return T;
		
	if (temp->ti.tkid == TK_FUNID|| temp->ti.tkid == TK_MAIN) {
		ind++;
		for(k=0; k<ind; k++){
			if(strcmp(T[k].funid, temp->ti.lex)==0){
				flag = 1;
				break;
			}
		}
		if(hc == NULL)					// this implies the function name occurs in a call, hence hc equals NULL, and function not to be added to symbol table
			flag = 1;
		if(flag==0){
			while (strcmp(T[ind].funid, "") != 0)
				ind++;
			strcpy(T[ind].funid, temp->ti.lex);
		}
	} else if (strcmp(temp->lex, "declarations") == 0) {
		thc = hc;
		while (thc != NULL) {
			strcpy(e.sym, thc->ti.lex);
			e.type = thc->headChild->ti.tkid;
			if (e.type == TK_RECORDID) {
				strcpy(e.rec, thc->headChild->ti.lex);
			} else {
				strcpy(e.rec, "");
			}
			if (thc->headChild->nextChild == NULL) {
				if (getType2(T, thc, ind) != 0)
					printf("Redeclaration of variable %s\n", thc->ti.lex);
				e.scope = LOCAL;
				strcpy(e.funid, T[ind].funid);
				T[ind].e[T[ind].count] = e;
				T[ind].count++;
			}
			else {
				if (getType(T, thc) != 0)
					printf("Redeclaration of variable %s\n", thc->ti.lex);
				e.scope = GLOBAL;
				T[0].e[T[0].count] = e;
				T[0].count++;
			}
			
			thc = thc->nextChild;
		}
	} else if (strcmp(temp->lex, "typeDefinitions") == 0) {
		while (hc != NULL) {
			if (getType2(T, hc, ind) != 0)
				printf("Redeclaration of variable %s\n", hc->ti.lex);
			strcpy(e.rec, hc->ti.lex);
			thc = hc->headChild->headChild;
			while (thc != NULL) {
				e.scope = LOCAL;
				e.type = thc->headChild->ti.tkid;
				strcpy(e.sym, thc->ti.lex);
				strcpy(e.funid, T[ind].funid);
				thc = thc->nextChild;
				T[ind].e[T[ind].count] = e;
				T[ind].count++;
			}
			hc = hc->nextChild;
		}
	} else if (strcmp(temp->lex, "parameter_list") == 0) {
		while (hc != NULL) {
			thc = hc->nextChild;
			if (getType2(T, thc, ind) != 0)
				printf("Redeclaration of variable %s\n", hc->ti.lex);
			e.type = hc->ti.tkid;
			e.scope = LOCAL;
			strcpy(e.sym, thc->ti.lex);
			strcpy(e.funid, T[ind].funid);
			if (e.type == TK_RECORDID) {
				strcpy(e.rec, hc->ti.lex);
			} else {
				strcpy(e.rec, "");
			}
			hc = thc->nextChild;
			T[ind].e[T[ind].count] = e;
			T[ind].count++;
		}
	} else if(temp->ti.tkid == TK_CALL){ 														///code to check if a recursive call is being made	
		if(strcmp(T[ind].funid, hc->nextChild->ti.lex) == 0){
			printf("ERROR: Recursive call detected in function '%s'\n", T[ind].funid);
		}
		for(k=0; k<ind; k++)
		{
			if(strcmp(T[k].funid, hc->nextChild->ti.lex) == 0){
				fun_flag = 1;
				break;
			}
		}
		if(fun_flag == 0){
			printf("ERROR: First define the function '%s' before call \n", hc->nextChild->ti.lex);
		}
	}
	
	while (hc != NULL) {
		T = populateSymbolTable(hc, T, ind);
		hc = hc->nextChild;
	}
	return T;
}

void sem_ii(Tables T, astNode root, int ind) {
	astNode temp = root;
	astNode hc = temp->headChild;
	
	int i;
	if (temp == NULL)
		return;
		
	if (temp->ti.tkid == TK_FUNID) {
		if (temp->headChild != NULL) {
			if (temp->headChild->ti.tkid == TK_INPUT) {
				for (i=0; strcmp(T[i].funid, "") != 0; ++i) {
					if (strcmp(T[i].funid, temp->ti.lex) == 0)
						ind = i;
				}
			}
		}
	}
	if (temp->ti.tkid == TK_MAIN) {
		for (i=0; strcmp(T[i].funid, "") != 0; ++i) {
			if (strcmp(T[i].funid, temp->ti.lex) == 0)
				ind = i;
		}
	}
	
	if (strcmp(temp->lex, "idList") == 0 || strcmp(temp->lex, "otherStmts") == 0) {
		checkRedec(T, temp, ind);
	}
	
	while (hc != NULL) {
		sem_ii(T, hc, ind);
		hc = hc->nextChild;
	}
}

int getType(Tables T, astNode node)
{
	int i,j;
	for(i=0; i<MAX_TABLES; i++)
	{
		if (strcmp(T[i].funid, "") == 0)
			continue;
		for(j=0; j<T[i].count; j++)
		{
			if(node->ti.tkid == TK_NUM)
				return TK_INT;
			if(node->ti.tkid == TK_RNUM)
				return TK_REAL;
			if(strcmp(node->ti.lex, T[i].e[j].sym)==0)
				return T[i].e[j].type;
			if (strcmp(node->ti.lex, T[i].e[j].rec) == 0)
				return T[i].e[j].type;
		}
	}
	return 0; 
}

int expType(Tables T, astNode root) {
	astNode temp = root;
	astNode hc = temp->headChild;
	
	if (temp == NULL)
		return 0;
		
	if (hc == NULL) {
		return getType(T, temp);
	}
	
	if (hc->nextChild != NULL) {
		if (expType(T, hc) == expType(T, hc->nextChild)) {
			return expType(T, hc);
		} else {
			printf("ERROR : Type mismatch in arithmetic expression\n");
			return 0;
		}
	} else {
		return expType(T, hc);
	}
}

void typeChecker(Tables T, astNode root) {
	astNode temp = root;
	astNode hc = temp->headChild;
	
	if (strcmp(temp->lex, "arithmeticExpression") == 0) {
		temp->as_type = expType(T, temp);
	}
	
	while (hc != NULL) {
		typeChecker(T, hc);
		hc = hc->nextChild;
	}
}

void assgnOp(Tables T, astNode root) {
	astNode temp = root;
	astNode hc = temp->headChild;
	
	if(strcmp(temp->lex, "126") == 0)
	{
		if(expType(T,temp->headChild) != temp->headChild->nextChild->as_type)
		{
			printf("ERROR: Type mismatch in assignment operator!\n");
		}	
	}

	while (hc != NULL) {
		assgnOp(T, hc);
		hc = hc->nextChild;
	}
}

void boolExp(Tables T, astNode root){
	astNode temp = root;
	astNode hc = temp->headChild;
	int tok;
	tok = temp->ti.tkid;
	if(tok == TK_AND || tok == TK_OR || tok == TK_LT || tok == TK_LE || tok == TK_EQ || tok == TK_GT || tok == TK_GE || tok == TK_NE)
	{
		if(expType(T, temp->headChild) != expType(T, temp->headChild->nextChild))
		{
			printf("ERROR: Type mismatch in booolean expression\n");								//remember to check for TK_NOT
		}
	}
	
	while (hc != NULL) {
		boolExp(T, hc);
		hc = hc->nextChild;
	}
}

int checkOverload(Tables T) {
	int i, j;
	for (i=0; strcmp(T[i].funid, "") != 0; ++i) {
		for (j=0; strcmp(T[j].funid, "") != 0; ++j) {
			if (i == j)
				continue;
			if (strcmp(T[i].funid, T[j].funid) == 0) {
				return 1;
			}
		}
	}
	return 0;
}


int compareidList(Tables T, astNode node1, astNode node2, int flag) {							// 
	astNode hc1 = node1->headChild;
	astNode hc2 = node2->headChild;

	
	while(hc1 != NULL || hc2 != NULL){
		if(hc1->ti.tkid != TK_ID || hc1->ti.tkid != TK_RECORDID){
			if(hc1->nextChild != NULL)
				hc1 = hc1->nextChild;
			else{
				if(flag==1){
					printf ("ERROR: Function returns (null) but returned value is %s of type: %s\n", hc2->ti.lex, reverseEnum(getType(T, hc2)));
					return -1;
				}
				if(flag == 2){
					printf ("ERROR: Function declaration has input parameter (null) but called with %s of type: %s", hc2->ti.lex, reverseEnum(getType(T, hc2)));
					return -1;
				}
				if(flag == 3){
					printf ("ERROR: Function declaration has output parameter (null) but function call assigns in %s of type: %s", hc2->ti.lex, reverseEnum(getType(T, hc2)));
					return -1;
				}
			}
		}
		if(getType(T, hc1) == getType(T, hc2)) {
			if(hc1->nextChild != NULL)
				hc1 = hc1->nextChild;
			else if(hc2->nextChild == NULL)
					break;
			if(hc2->nextChild != NULL)	
				hc2 = hc2->nextChild;
			else{
				if(flag==1){
					printf("ERROR: Function returns %s of type: %s but returned value is (null)\n", hc1->nextChild->ti.lex, reverseEnum(hc1->ti.tkid)); 
					return -1;
				}
				if(flag == 2){
					printf ("ERROR: Function declaration has input parameter %s of type: %s but called with (null)", hc1->nextChild->ti.lex, reverseEnum(hc1->ti.tkid));
					return -1;
				}
				if(flag == 3){
					printf ("ERROR: Function declaration has output parameter %s of type: %s but function call assigns (null)", hc1->nextChild->ti.lex, reverseEnum(hc1->ti.tkid));
					return -1;
				}
			}
			continue;
		}
		else{
			if(flag==1){
				printf("ERROR: Function returns %s of type: %s but returned value is %s of type: %s in the function definition\n", hc1->ti.lex, reverseEnum(getType(T, hc1)), hc2->ti.lex, reverseEnum(getType(T,hc2)));
				return -1;
			}
			if(flag==2){
				printf("ERROR: Function declaration has input parameter %s of type: %s but called with %s of type: %s", hc1->ti.lex, reverseEnum(getType(T, hc1)), hc2->ti.lex, reverseEnum(getType(T,hc2)));
				return -1;
			}
			if(flag==3){
				printf("ERROR: Function declaration has output parameter %s of type: %s but function call assigns in %s of type: %s", hc1->ti.lex, reverseEnum(getType(T, hc1)), hc2->ti.lex, reverseEnum(getType(T,hc2)));
				return -1;
			}
		}
	}
	
	return 0;
}

void matchReturnType(Tables T, astNode root)								//
{
	astNode temp = root;
	if(temp->headChild == NULL)
		return;
	astNode hc = temp->headChild;		

	
	if(temp->ti.tkid == TK_OUTPUT)
	{
		
		astNode thc = temp->nextChild;
		astNode tnode = thc->headChild;
		int voidflag = 0;
		int res;
				
		while(strcmp(tnode->lex, "idList") != 0)
		{
			tnode = tnode->nextChild;
			if(tnode == NULL){
				voidflag = 1;
				break;
			}
		}
		res = compareidList(T, hc, tnode, 1);
		if (res == -1){
			printf("Return type mismatch\n");
		}
		
	}
	
	while(hc!=NULL)
	{
		matchReturnType(T, hc);
		hc = hc->nextChild;
	}
}

astNode jumpto(astNode root, char* nodelex)
{
	astNode temp = root;
	if(temp->headChild == NULL)
		return temp;
	astNode hc = temp->headChild;
	
	if(strcmp(temp->ti.lex, nodelex)==0)
		return temp;
		
	while(hc!=NULL)
	{
		temp =jumpto(hc, nodelex);
		if(temp->ti.tkid == TK_FUNID)
			break;
		hc = hc->nextChild;
	}
	//printf("******* %s\n", temp->ti.lex);
	return temp;
}

void funInputCheck(Tables T, astNode root, astNode glob_astr)
{
	astNode temp = root;
	if(temp->headChild == NULL)
		return;
	astNode hc = temp->headChild;
	
	if(temp->ti.tkid == TK_CALL){
		astNode funnode_in; astNode funnode_out;
		astNode thc = temp->headChild;
		//astNode tnode = thc->headChild;
		int res1, res2;
		
		funnode_in = jumpto(glob_astr, thc->nextChild->ti.lex);
		
		if(funnode_in->headChild == NULL){
			printf("Please check the call for function '%s': Called before  its definition\n", funnode_in->ti.lex);
			printf("*****Aborting execution.... please rectify and rerun program!*****\n");
			exit(0);
		}
		
		funnode_out = funnode_in->headChild->nextChild->headChild;
		

		res1 = compareidList(T, funnode_in->headChild->headChild, thc->nextChild->nextChild, 2);
		if(res1 == -1)
			printf(" ---> In call of function '%s'\n\n", funnode_in->ti.lex); 
		res2 = compareidList(T, funnode_out, thc, 3);
		if(res2 == -1)
			printf(" ---> In call of function '%s'\n\n", funnode_in->ti.lex); 
		
	}
	
	while(hc!=NULL)
	{
		funInputCheck(T, hc, glob_astr);
		hc = hc->nextChild;
	}
}
