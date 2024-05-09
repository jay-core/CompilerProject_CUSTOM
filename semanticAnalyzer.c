/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */



#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<string.h>
#include "parserDef.h"
#include "lexer.h"
#include "symbolTable.h"
#include "semanticAnalyser.h"
char *idRepr(int id);
struct linklist{
	parseTree this;
	struct linklist* next;
};
typedef struct linklist* LinkedList;

LinkedList add_1(LinkedList l, parseTree t){
	if(l == NULL){
		LinkedList l = malloc(sizeof(struct linklist));
		l->this = t;
		l->next = NULL;
	}
	else{
		l->next = add_1(l->next, t);
	}
	return l;
}


parseTree* findChildren(parseTree root, int* size){
	if(root->nonTerminal == mainFunction){
		parseTree* funList = malloc(sizeof(parseTree));
		funList[0] = root;
		*size = 1;
		return funList;
	}
	else if((root->nonTerminal == output_par) || (root->nonTerminal == outputParameters) ||(root->nonTerminal == inputParameters) ||(root->nonTerminal == idList) || (root->nonTerminal == input_par)){
		parseTree curr = root;
		int count = 0;
		while(curr->numChildAST != 0){
			if(curr->numChildAST == 1) curr =  &(curr->children[0]);
			curr = &(curr->children[1]);
			count++;
		}
		count++;
		parseTree* funList = malloc(sizeof(parseTree) * count);
		*size = count;
		curr = root;
		count = 0;
		while(curr->numChildAST != 0){
			if(curr->numChildAST == 1) curr = &(curr->children[0]);
			funList[count++] = &(curr->children[0]);
			curr = &(curr->children[1]);
		}
		funList[count++] = curr;
		funList[count] = &(curr->children[0]);
		
		return funList;
	}
	else if((root->nonTerminal == otherStmts) || (root->nonTerminal == otherFunctions) || (root->nonTerminal == elsePart)){
		parseTree curr = root;
		int count = 0;
		while(curr->numChildAST != 1 && (curr->children[0].nonTerminal != curr->children[1].nonTerminal)){
			//printf("before1\n");
			curr = &(curr->children[1]);
			//printf("%d\n", curr->numChildAST);
			//printf("before2\n");
			count++;
		}
		if(curr->numChildAST == 1){count++;}
		else{count = count + 2;}
		//printf("%d\n", count);
		parseTree* funList = malloc(sizeof(parseTree) * count);
		*size = count;
		//printf("%ld\n", (*(&funList + 1) - funList) / (-6));
		curr = root;
		count = 0;
		while(curr->numChildAST != 1 && (curr->children[0].nonTerminal != curr->children[1].nonTerminal)){
			funList[count++] = &(curr->children[0]);
			curr = &(curr->children[1]);
		}
		if(curr->numChildAST == 1)
			funList[count] = &(curr->children[0]);
		else{
			funList[count++] = &(curr->children[0]);
			funList[count] = &(curr->children[1]);
		}
		return funList;
	}
}

LinkedList findTerminalsBoolean(parseTree root, LinkedList l){
	if(root->numChildAST == 0){
		l = add_1(l, root);
	}
	else if(root->numChildAST == 2){ 
		l = findTerminalsBoolean(&(root->children[0]), l);
		l = findTerminalsBoolean(&(root->children[1]), l);
	}
	else{
		l = findTerminalsBoolean(&(root->children[0]), l);
		l = findTerminalsBoolean(&(root->children[1]), l);
		l = findTerminalsBoolean(&(root->children[2]), l);
	}
	return l;
}

int WhileSemantics(parseTree t){return 0;}

int checkFuncCallStmts(parseTree stmt, parseTree* funList, symbolTable s, int* assigned, parseTree output_par, int i, int size){
	int ids_size = 0;
	parseTree* ids = findChildren(output_par, &ids_size);
	int error = 0;
	//printf("%s\n",idRepr(stmt->nonTerminal));
	switch(stmt->nonTerminal){
		case funCallStmt:
		;
			char* fname;
			fname = malloc(sizeof(char) * 30);
			if(stmt->numChildAST == 3) strcpy(fname, stmt->children[1].terminal->lexeme);
			else strcpy(fname, stmt->children[0].terminal->lexeme);
			tableHeader* callee = s->fTable[hashVal(fname, 10000)];
			//if(callee == NULL) return -1;
			int index;
			for(int l = 0; l < size; l++){
				if(strcmp(fname, funList[l]->children[0].terminal->lexeme) == 0){
					index = l;
					break;
				}
			}
			if(index > i){
				printf("Line : %llu Afunction %s cannot be called before its declaration\n", stmt->children[1].terminal->lineNum, stmt->children[1].terminal->lexeme);
				error = -1;
			}
			if(index == i){
				printf("Line : %llu Afunction %s cannot be called recursively\n", stmt->children[1].terminal->lineNum, stmt->children[1].terminal->lexeme);
				error = -1;
			}
			parseTree outpar = &(stmt->children[0]);
			if(outpar->numChildAST != callee->numOutPar){
				printf("Line : %llu Number of parameters required for function %s does not match number of returned parameters\n", stmt->children[1].terminal->lineNum, stmt->children[1].terminal->lexeme);
				error = -1;
			}
			int opsize = 0;
			parseTree* oplist = findChildren(outpar, &opsize);
			for(int l = 0; l < opsize; l++){
				TableLoc* ptr1 = funList[i]->tp->localTable[hashVal(oplist[l]->terminal->lexeme, 10000)];
				record** array = malloc(sizeof(record*) * callee->numOutPar);
				record* head = callee->outParList;
				for(int m = 0; m < callee->numOutPar; m++){
					array[m] = head;	
					head = head->next;
				}
				if(ptr1 == NULL || (strcmp(ptr1->type, array[l]->type) != 0)){
					printf("Line :%llu type of output parameter in function %s does not match the return type\n", oplist[l]->terminal->lineNum, callee->fname);
					error = -1;
				}
				if(assigned != NULL){
					for(int m = 0; m < ids_size; m++){
						TableLoc* ptr2 = funList[i]->tp->localTable[hashVal(ids[m]->terminal->lexeme, 10000)];
						if(ptr1 == NULL || ptr2 == NULL) continue;	
						if(strcmp(ptr2->type,ptr1->type) == 0) assigned[m] = 1;
					}
				}
			}
			parseTree inppar = &(stmt->children[2]);
			int inppar_size = 0;
			parseTree* inppar_list = findChildren(inppar, &inppar_size); 
			printf("%d,%s\n",inppar_size,callee->fname);
			if(inppar_size != callee->numInpPar){
				printf("Line : %llu Number of input parameters required for function %s does not match number of input parameters\n", stmt->children[1].terminal->lineNum, stmt->children[1].terminal->lexeme);
				error = -1;
			}
			int ipsize = 0;
			parseTree* iplist = findChildren(inppar, &ipsize);
			for(int l = 0; l < ipsize; l++){
				if(!funList[i]->tp)
				{
					printf("Line : %llu type of input parameter in function %s does not match the input argument type\n", iplist[l]->terminal->lineNum, callee->fname);
					error = -1;
					continue;
				}
				TableLoc* ptr1 = funList[i]->tp->localTable[hashVal(iplist[l]->terminal->lexeme, 10000)];
				record** array = malloc(sizeof(record*) * callee->numInpPar);
				record* head = callee->inParList;
				for(int m = 0; m < callee->numInpPar; m++){
					array[m] = head;
					head = head->next;
				}
				if(ptr1 == NULL || (strcmp(ptr1->type, array[l]->type) != 0)){
					printf("Line : %llu type of input parameter in function %s does not match the input argument type\n", iplist[l]->terminal->lineNum, callee->fname);
					error = -1;
				}
			} 
			break;
		case iterativeStmt:
		//printf("hwgdq");
			if(stmt->numChildAST == 3){
				int temp1_size = 0;
				parseTree* temp1 = findChildren(&(stmt->children[2]), &temp1_size);
				if(checkFuncCallStmts(&(stmt->children[1].children[0]), funList, s, assigned, output_par, i, size) == -1) error = -1;
				for(int p = 0; p < temp1_size; p++){
					if(checkFuncCallStmts(temp1[p], funList, s, assigned, output_par, i, size) == -1) error = -1;
				}
			}
			else{
				if(checkFuncCallStmts(&(stmt->children[1]), funList, s, assigned, output_par, i, size) == -1) error = -1;
			}
			if(WhileSemantics(stmt) == -1){
				if(stmt->children[0].numChildAST == 3)
					printf("Line : %llu None of the variables participating in the iterations of the while loop gets updated.\n", stmt->children[0].children[1].terminal->lineNum);
				else{
					printf("Line : %llu None of the variables participating in the iterations of the while loop gets updated.\n", stmt->children[0].children[0].terminal->lineNum);
				}
			}
			break;
		case conditionalStmt:
			if(stmt->numChildAST == 4){
				int temp1s, temp2s;
				temp1s = temp2s = 0;
				parseTree* temp1 = findChildren(&(stmt->children[2]), &temp1s);
				parseTree* temp2 = findChildren(&(stmt->children[3]), &temp2s);
				if(checkFuncCallStmts(&(stmt->children[1]), funList, s, assigned, output_par, i, size) == -1) error = -1;
				for(int p = 0; p < temp1s; p++){
					if(checkFuncCallStmts(temp1[p], funList, s, assigned, output_par, i, size) == -1) error = -1;
				}
				for(int p = 0; p < temp2s; p++){
					if(checkFuncCallStmts(temp2[p], funList, s, assigned, output_par, i, size) == -1) error = -1;
				}
			}
			else if(stmt->numChildAST == 3){
				int temp1s = 0;
				parseTree* temp1 = findChildren(&(stmt->children[2]), &temp1s);
				if(checkFuncCallStmts(&(stmt->children[1]), funList, s, assigned, output_par, i, size) == -1) error = -1;
				for(int p = 0; p < temp1s; p++){
					if(checkFuncCallStmts(temp1[p], funList, s, assigned, output_par, i, size) == -1) error = -1;
				}
			}
			else{
				if(checkFuncCallStmts(&(stmt->children[1]), funList, s, assigned, output_par, i, size) == -1) error = -1;
			}
			break;
		case assignmentStmt:
		//printf("Here134\n");
			if(assigned != NULL){
			if(!funList[i]->tp)
				{
					//printf("Line No %llu type of input parameter in function %s does not match the input argument type\n", iplist[l]->terminal->lineNum, callee->fname);
					//error = -1;
					//continue;
					return 0;
				}
				//printf("Here134\n");
				TableLoc* ptr1;
				if(stmt->children[0].terminal != NULL){
				
					if(s->fTable[hashVal("global",10000)]->localTable[hashVal(stmt->children[0].terminal->lexeme, 10000)])
							ptr1 = s->fTable[hashVal("global",10000)]->localTable[hashVal(stmt->children[0].terminal->lexeme, 10000)];
					else ptr1 = funList[i]->tp->localTable[hashVal(stmt->children[0].terminal->lexeme, 10000)];
				}
				else{
					if(s->fTable[hashVal("global",10000)]->localTable[hashVal(stmt->children[0].children[1].terminal->lexeme, 10000)])
							ptr1 = s->fTable[hashVal("global",10000)]->localTable[hashVal(stmt->children[0].children[1].terminal->lexeme, 10000)];
					else ptr1 = funList[i]->tp->localTable[hashVal(stmt->children[0].children[1].terminal->lexeme, 10000)];
				}		
				 //= funList[i]->tp->localTable[hashVal(stmt->children[0].children[0].terminal->lexeme, 10000)];
				 //printf("wchjbjc");
				/*if(ptr1 == NULL) return 0;
				for(int m = 0; m < ids_size; m++){
				TableLoc* ptr2 ;
				if(s->fTable[hashVal("global",10000)]->localTable[hashVal(ids[m]->terminal->lexeme, 10000)])
						ptr2 = s->fTable[hashVal("global",10000)]->localTable[hashVal(ids[m]->terminal->lexeme, 10000)];
						else ptr2 = funList[i]->tp->localTable[hashVal(ids[m]->terminal->lexeme, 10000)];
						
					//= funList[i]->tp->localTable[hashVal(ids[m]->terminal->lexeme, 10000)];
					if(ptr2 == NULL) continue;
					if(ptr1->type == ptr2->type) 
						assigned[m] = 1;
						break;
				}*/
				
			}
			//return 0;
			break;
		case ioStmt:
		//break;
			//printf("jdcbjwcj");
			if(assigned != NULL){
			if(!funList[i]->tp)
				{
					//printf("Line No %llu type of input parameter in function %s does not match the input argument type\n", iplist[l]->terminal->lineNum, callee->fname);
					//error = -1;
					//continue;
					return 0;
				}
				TableLoc* ptr1 ;//= funList[i]->tp->localTable[hashVal(stmt->children[1].terminal->lexeme, 10000)];
				if(stmt->children[1].terminal != NULL){
				
					if(s->fTable[hashVal("global",10000)]->localTable[hashVal(stmt->children[1].terminal->lexeme, 10000)])
							ptr1 = s->fTable[hashVal("global",10000)]->localTable[hashVal(stmt->children[1].terminal->lexeme, 10000)];
					else ptr1 = funList[i]->tp->localTable[hashVal(stmt->children[1].terminal->lexeme, 10000)];
				}
				else if(stmt->children[1].nonTerminal == allVar){
					if(s->fTable[hashVal("global",10000)]->localTable[hashVal(stmt->children[1].children[0].children[1].terminal->lexeme, 10000)])
							ptr1 = s->fTable[hashVal("global",10000)]->localTable[hashVal(stmt->children[1].children[0].children[1].terminal->lexeme, 10000)];
					else ptr1 = funList[i]->tp->localTable[hashVal(stmt->children[1].children[0].children[1].terminal->lexeme, 10000)];
				}
				else{
					if(s->fTable[hashVal("global",10000)]->localTable[hashVal(stmt->children[1].children[1].terminal->lexeme, 10000)])
							ptr1 = s->fTable[hashVal("global",10000)]->localTable[hashVal(stmt->children[1].children[1].terminal->lexeme, 10000)];
					else ptr1 = funList[i]->tp->localTable[hashVal(stmt->children[1].children[1].terminal->lexeme, 10000)];
				}	
				/*if(ptr1 == NULL) return 0;
				for(int m = 0; m < ids_size; m++){
				if(!funList[i]->tp)
					{
						//printf("Line No %llu type of input parameter in function %s does not match the input argument type\n", iplist[l]->terminal->lineNum, callee->fname);
						//error = -1;
						continue;
						//return 0;
					}
					TableLoc* ptr2 = funList[i]->tp->localTable[hashVal(ids[m]->terminal->lexeme, 10000)];
					if(ptr2 == NULL) continue;
					if(strcmp(ptr1->type, ptr2->type) == 0) 
						assigned[m] = 1;
						break;
				}*/
			}
			return 0;
		default:
		//printf("Here134\n");
			return 0;
	}
}

int checkFunctionSemantics(parseTree root, symbolTable s){
	int error = 0;
	if(root->numChildAST == 2){
		int size = 0; 
		parseTree* funList = findChildren(&(root->children[0]), &size);
		//printf("%d\n", size);
		int flag2 = 0;
		for(int i = 0; i < size; i++){
			//printf("I am in the scope of %s\n", funList[i]->tp->fname);
			tableHeader* fentry = funList[i]->tp;
			if(funList[i]->children[funList[i]->numChildAST - 1].nonTerminal == stmts){
				//printf("after1\n");
				parseTree temp = &(funList[i]->children[funList[i]->numChildAST - 1]);
				if(temp->children[temp->numChildAST - 1].nonTerminal == returnStmt || ((temp->children[temp->numChildAST - 1].terminal != NULL) && (temp->children[temp->numChildAST - 1].terminal->tokenType == TK_ID))){
					//printf("Here\n");
					parseTree returnNode = &(temp->children[temp->numChildAST - 1]);
					if(returnNode->numChildAST != 0 && returnNode->numChildAST != fentry->numOutPar){
						printf("Error in function %s, number of parameters does not match the number output parameters in function definition\n", fentry->fname);
						error = -1; 
					}
					else if(fentry->numOutPar != 1){
						printf("Error in function %s, number of parameters does not match the number output parameters in function definition\n", fentry->fname);
						error = -1; 
					}
					int IDsize = 0;
					parseTree* idList;
					if(temp->children[temp->numChildAST - 1].nonTerminal == returnStmt){
						//printf("Here\n");
						idList = findChildren(&(returnNode->children[0].children[0]), &IDsize);
					}
					else{
						//printf("Andar\n");
						 idList = malloc(sizeof(parseTree));
						idList[0] = returnNode;
						//printf("%s\n",idList[0]->terminal->lexeme);
						IDsize = 1;
					}
					//printf("%s1\n",idList[0]->terminal->lexeme);
					for(int j=0;j < IDsize;j++)
					{
						//printf("%s2\n",idList[0]->terminal->lexeme);
						//printf("Here1\n");
						if(!funList[i]->tp)
						{
							//printf("Line No %llu type of input parameter in function %s does not match the input argument type\n", iplist[l]->terminal->lineNum, callee->fname);
							//error = -1;
							//continue;
							//return -1;
							flag2 = 1;
							break;
						}
						//printf("%s3\n",idList[0]->terminal->lexeme);
						//printf("beech\n");
						TableLoc* ptr2;
						if(s->fTable[hashVal("global",10000)]->localTable[hashVal(idList[j]->terminal->lexeme, 10000)])
						ptr2 = s->fTable[hashVal("global",10000)]->localTable[hashVal(idList[j]->terminal->lexeme, 10000)];
						else ptr2 = funList[i]->tp->localTable[hashVal(idList[j]->terminal->lexeme, 10000)];
						
						/************** If you start debugging, start it from the above line. It is accessing a NULL pointer. The next "Here" is not printing ********************/
						
						//printf("Here2\n");
						//if(ptr2)
						//printf("yahan0\n");
						//int flag2 = 0;
						if(!ptr2 || ptr2->type == NULL){
							flag2 = 1;
							break;
						}
							
						//printf("yahan\n");
						record** array = malloc(sizeof(record*) * fentry->numOutPar);
						record* head = fentry->outParList;
						for(int m = 0; m < fentry->numOutPar; m++){
							array[m] = head;
							head = head->next;
						}
						if( array[j]!=NULL && strcmp(ptr2->type, array[j]->type) != 0)
						{
							printf("line : %llu returned type of %s does not match expected type\n",idList[j]->terminal->lineNum, idList[j]->terminal->lexeme);
							error=-1;
						}
					}
					if(flag2) continue;
					if(funList[i]->children[funList[i]->numChildAST - 2].nonTerminal == output_par){
						//printf("here3\n");
						parseTree output_par = &(funList[i]->children[funList[i]->numChildAST - 2]);
						int ids_size = 0;
						parseTree* ids = findChildren(output_par, &ids_size);
						int* assigned = malloc(sizeof(int) * ids_size);
						memset(assigned, 0, ids_size);
						if(temp->numChildAST != 1 && temp->children[temp->numChildAST - 2].nonTerminal == otherStmts){
							parseTree other_Stmts = &(temp->children[temp->numChildAST - 2]);
							int size_stmt_list = 0;
							parseTree* stmt_list = findChildren(other_Stmts, &size_stmt_list);
							for(int k = 0; k < size_stmt_list; k++){
								if(checkFuncCallStmts(&stmt_list[k]->children[0], funList, s, assigned, output_par, i, size) == -1) error = -1;
							} 
							if(output_par != NULL){
								for(int l = 0; l < ids_size; l++){
									if(assigned[l] == 0){
										//printf("%d\n\n\n\n", size_stmt_list);
										//printf("%s output parameter not assigned ay value before returning it in function %s\n", ids[l]->terminal->lexeme, funList[i]->children[0].terminal->lexeme);
										error=-1;
									}
								}
							}
							//break;
						}
					}
				}
				else{
					//printf("else here\n");
					if(funList[i]->children[funList[i]->numChildAST - 2].nonTerminal == output_par){
						parseTree output_par = &(funList[i]->children[funList[i]->numChildAST - 2]);
						int ids_size = 0;
						parseTree* ids = findChildren(output_par, &ids_size);
						int* assigned = malloc(sizeof(int) * ids_size);
						memset(assigned, 0, ids_size);
						if(temp->numChildAST != 1 && temp->children[temp->numChildAST - 2].nonTerminal == otherStmts){
							parseTree other_Stmts = &(temp->children[temp->numChildAST - 2]);
							int size_stmt_list = 0;
							parseTree* stmt_list = findChildren(other_Stmts, &size_stmt_list);
							for(int k = 0; k < size_stmt_list; k++){
								if(checkFuncCallStmts(stmt_list[k], funList, s, assigned, output_par, i,size) == -1) error = -1;
							} 
							if(output_par != NULL){
								for(int l = 0; l < ids_size; l++){
									if(assigned[l] == 0){
										printf("%s output parameter not assigned ay value before returning it in function %s\n", ids[l]->terminal->lexeme, funList[i]->children[0].terminal->lexeme);
										error=-1;
									}
								}
							}
							//break;
						}
					}
				}
			}
			//printf("after2\n");
		}
		//printf("Here\n");
		size = 0;
		funList = findChildren(&(root->children[1]), &size);
		parseTree temp3 = &(root->children[1].children[0]); 
		if(temp3->numChildAST != 1 && temp3->children[temp3->numChildAST - 2].nonTerminal == otherStmts){
			parseTree other_Stmts = &(temp3->children[temp3->numChildAST - 2]);
			int size_stmt_list = 0;
			parseTree* stmt_list = findChildren(other_Stmts, &size_stmt_list);
			for(int k = 0; k < size_stmt_list; k++){
				if(checkFuncCallStmts(stmt_list[k], funList, s, NULL, NULL, -1, size) == -1) error = -1;
			} 
			//break;
		}
		
	}
	else{
		//fentry of the main function
		//printf("Here3\n");
		int size = 0;
		parseTree* funList = findChildren(&(root->children[0]), &size);
		parseTree temp = &(root->children[0].children[0]); 
		if(temp->numChildAST != 1 && temp->children[temp->numChildAST - 2].nonTerminal == otherStmts){
			parseTree other_Stmts = &(temp->children[temp->numChildAST - 2]);
			int size_stmt_list = 0;
			parseTree* stmt_list = findChildren(other_Stmts, &size_stmt_list);
			for(int k = 0; k < size_stmt_list; k++){
				if(checkFuncCallStmts(stmt_list[k], funList, s, NULL, NULL, -1, size) == -1) error = -1;
			} 
			//break;
		}
	}
}  
