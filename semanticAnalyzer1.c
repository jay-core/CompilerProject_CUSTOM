/* 
* Group 05
* Kush Mehta			2018B5A70956P
* Purusharth Amrut		2018B5A70897P
* Patel Darsh Rajesh	2018B4A70532P
* Harsh Jhunjhunwala	2018B5A70691P
* Jatin Aggarwal		2018B4A70884P
*/

#include "semanticAnalyzer1.h"

LinkedList addOneElem(LinkedList l, parseTree t) {
    if(l==NULL) {
        l = (LinkedList)malloc(sizeof(struct linkedList));
        l->this = t;
        l->next = NULL;
    }else l->next = addOneElem(l->next, t);

    return l;
}

LinkedList findBooleanTerminals(parseTree root, LinkedList l) {
    if(root->numChild==0) l = addOneElem(l, root);
    else if(root->numChild==2) {
        l = findBooleanTerminals(&(root->children[0]), l);
        l = findBooleanTerminals(&(root->children[1]), l);
    }else {
        l = findBooleanTerminals(&(root->children[0]), l);
        l = findBooleanTerminals(&(root->children[1]), l);
        l = findBooleanTerminals(&(root->children[2]), l);
    }
    return l;
}

/*=====================================================================*/

parseTree* getChildren(parseTree root, int *size) {
    if(root->nt==mainFunction) {
        parseTree *functionList = (parseTree*)malloc(sizeof(parseTree));
        functionList[0] = root;
        *size = 1;
        return functionList;
    }else if(root->nt==input_par || root->nt==idList || 
    root->nt==inputParameters || root->nt==outputParameters ||
    root->nt==output_par) {
        parseTree currNode = root;
        int cnt = 0;

        while(currNode->numChild!=0) {
            if(currNode->numChild==1) currNode = &(currNode->children[0]);
            currNode = &(currNode->children[1]);
            cnt++;
        }
        cnt++;

        parseTree *functionList = (parseTree*)malloc(cnt*sizeof(parseTree));
        *size = cnt;
        currNode = root;
        cnt = 0;

        while(currNode->numChild!=0) {
            if(currNode->numChild==1) currNode = &(currNode->children[0]);
            functionList[cnt++] = &(currNode->children[0]);
            currNode = &(currNode->children[1]);
        }

        functionList[cnt++] = currNode;
        functionList[cnt] = &(currNode->children[0]);

        return functionList;
    }else if(root->nt==elsePart || root->nt==otherFunctions || 
    root->nt==otherStmts) {
        parseTree currNode = root;
        int cnt = 0;

        while(currNode->numChild!=1 && 
        currNode->children[0].nt!=currNode->children[1].nt) {
            currNode = &(currNode->children[1]);
            cnt++;
        }

        if(currNode->numChild==1) cnt++;
        else cnt += 2;

        parseTree *functionList = (parseTree*)malloc(cnt*sizeof(parseTree));
        *size = cnt;
        currNode = root;
        cnt = 0;

        while(currNode->numChild!=1 && 
        currNode->children[0].nt!=currNode->children[1].nt) {
            functionList[cnt++] = &(currNode->children[0]);
            currNode = &(currNode->children[1]);
        }

        if(currNode->numChild==1) functionList[cnt] = &(currNode->children[0]);
        else {
            functionList[cnt++] = &(currNode->children[0]);
            functionList[cnt] = &(currNode->children[1]);
        }

        return functionList;
    }
}

int whileStmtSemantics(parseTree t) {
    return 0;
} 

int checkForFunctionCallStmts(parseTree stmt, parseTree *functionList, 
symbolTable s, int *assignedVal, parseTree tmpOutputPar, int i, int size) {
    int idsSize = 0;
    parseTree *idsList = getChildren(tmpOutputPar, &idsSize);
    int error = 0;

    switch(stmt->nt) {
        case assignmentStmt:
            if(assignedVal!=NULL) {
                if(!functionList[i]->th) return 0;
                
                // TableLoc *tmpPtr;
                tablePtr *tabPtr;
                if(stmt->children[0].terminal!=NULL) {
                    if(s->fTable[hashFuncLUT("global", 10000)]->localTable[hashFuncLUT(stmt->children[0].terminal->lexeme, 10000)]) {
                        tabPtr = s->fTable[hashFuncLUT("global", 10000)]->localTable[hashFuncLUT(stmt->children[0].terminal->lexeme, 10000)];
                    }else {
                        tabPtr = functionList[i]->th->localTable[hashFuncLUT(stmt->children[0].children[1].terminal->lexeme, 10000)];
                    }
                }else {
                    if(s->fTable[hashFuncLUT("global",10000)]->localTable[hashFuncLUT(stmt->children[0].children[1].terminal->lexeme, 10000)]) {
                        tabPtr = s->fTable[hashFuncLUT("global",10000)]->localTable[hashFuncLUT(stmt->children[0].children[1].terminal->lexeme, 10000)];
                    }else {
                        tabPtr = functionList[i]->th->localTable[hashFuncLUT(stmt->children[0].children[1].terminal->lexeme, 10000)];
                    }
                }
            }
            break;
        case iterativeStmt:
            if(stmt->numChild==3) {
                int tmpSize = 0;
                parseTree *tmpList = getChildren(&(stmt->children[2]), &tmpSize);

                if(checkForFunctionCallStmts(&(stmt->children[1].children[0]), 
                functionList, s, assignedVal, tmpOutputPar, i, size)==-1) error = -1;

                for(int k=0; k<tmpSize; k++)  {
                    if(checkForFunctionCallStmts(tmpList[k], functionList, s, 
                    assignedVal, tmpOutputPar, i, size)==-1) error = -1;
                }
            }else {
                if(checkForFunctionCallStmts(&(stmt->children[1]), functionList,
                s, assignedVal, tmpOutputPar, i, size)==-1) error = -1;
            }

            if(whileStmtSemantics(stmt)==-1) {
                if(stmt->children[0].numChild==3) {
                    printf("Line : %llu None of the variables participating in the iterations of the while loop gets updated.\n", stmt->children[0].children[1].terminal->lineNum);
				}else{
					printf("Line : %llu None of the variables participating in the iterations of the while loop gets updated.\n", stmt->children[0].children[0].terminal->lineNum);
				}
            }
            break;
        case conditionalStmt:
            if(stmt->numChild==4) {
                int tmp1Size, tmp2Size;
                tmp1Size = tmp2Size = 0;
                parseTree *tmpList1 = getChildren(&(stmt->children[2]), &tmp1Size);
                parseTree *tmpList2 = getChildren(&(stmt->children[3]), &tmp2Size);

                if(checkForFunctionCallStmts(&(stmt->children[1]), functionList, s,
                assignedVal, tmpOutputPar, i, size)==-1) error = -1;

                for(int k=0; k<tmp1Size; k++) {
                    if(checkForFunctionCallStmts(tmpList1[k], functionList, s,
                    assignedVal, tmpOutputPar, i, size)==-1) error = -1;
                }

                for(int k=0; k<tmp1Size; k++) {
                    if(checkForFunctionCallStmts(tmpList2[k], functionList, s,
                    assignedVal, tmpOutputPar, i, size)==-1) error = -1;
                }
            }else if(stmt->numChild==3) {
                int tmpSize = 0;
                parseTree *tmpList = getChildren(&(stmt->children[2]), &tmpSize);

                if(checkForFunctionCallStmts(&(stmt->children[1]), functionList, s,
                assignedVal, tmpOutputPar, i, size)==-1) error = -1;

                for(int k=0; k<tmpSize; k++) {
                    if(checkForFunctionCallStmts(tmpList[k], functionList, s,
                    assignedVal, tmpOutputPar, i, size)==-1) error = -1;
                }
            }else {
                if(checkForFunctionCallStmts(&(stmt->children[1]), functionList, s,
                assignedVal, tmpOutputPar, i, size)==-1) error = -1;
            }
            break;
        case ioStmt:
            if(assignedVal!=NULL) {
                if(!functionList[i]->th) return 0;

                // TableLoc *tmpPtr;
                tablePtr *tabPtr;
                if(stmt->children[1].terminal!=NULL) {
                    if(s->fTable[hashFuncLUT("global",10000)]->localTable[hashFuncLUT(stmt->children[1].terminal->lexeme, 10000)]) {
                        tabPtr = s->fTable[hashFuncLUT("global",10000)]->localTable[hashFuncLUT(stmt->children[1].terminal->lexeme, 10000)];
                    }else {
                        tabPtr = functionList[i]->th->localTable[hashFuncLUT(stmt->children[1].terminal->lexeme, 10000)];
                    }                        
                }else if(stmt->children[1].nt==var) {
                    if(s->fTable[hashFuncLUT("global",10000)]->localTable[hashFuncLUT(stmt->children[1].children[0].children[1].terminal->lexeme, 10000)]) {
                        tabPtr = s->fTable[hashFuncLUT("global",10000)]->localTable[hashFuncLUT(stmt->children[1].children[0].children[1].terminal->lexeme, 10000)];
                    }else {
                        tabPtr = functionList[i]->th->localTable[hashFuncLUT(stmt->children[1].children[0].children[1].terminal->lexeme, 10000)];
                    }
                }else {
                    if(s->fTable[hashFuncLUT("global",10000)]->localTable[hashFuncLUT(stmt->children[1].children[1].terminal->lexeme, 10000)]) {
                        tabPtr = s->fTable[hashFuncLUT("global",10000)]->localTable[hashFuncLUT(stmt->children[1].children[1].terminal->lexeme, 10000)];
                    }else {
                        tabPtr = functionList[i]->th->localTable[hashFuncLUT(stmt->children[1].children[1].terminal->lexeme, 10000)];
                    }
                }
            }
            return 0;
        case funCallStmt:
            ;
            char *nameStr;
            nameStr = malloc(30*sizeof(char));

            if(stmt->numChild==3) strcpy(nameStr, stmt->children[1].terminal->lexeme);
            else strcpy(nameStr, stmt->children[0].terminal->lexeme);

            tableHeader *callee = s->fTable[hashFuncLUT(nameStr, 10000)];
			
			int idx;
			for(int k=0; k<size; k++){
				if(strcmp(nameStr, functionList[k]->children[0].terminal->lexeme)==0){
                    idx = k;
					break;
				}
			}

			if(idx>i){
				printf("Line : %llu Afunction %s cannot be called before its declaration\n", stmt->children[1].terminal->lineNum, stmt->children[1].terminal->lexeme);
				error = -1;
			}
			if(idx==i){
				printf("Line : %llu Afunction %s cannot be called recursively\n", stmt->children[1].terminal->lineNum, stmt->children[1].terminal->lexeme);
				error = -1;
			}

			parseTree outputParam = &(stmt->children[0]);
			if(outputParam->numChild!=callee->numOfOutPar){
				printf("Line : %llu Number of parameters required for function %s does not match number of returned parameters\n", stmt->children[1].terminal->lineNum, stmt->children[1].terminal->lexeme);
				error = -1;
			}

            int outputParamSize = 0;
			parseTree* outputParamList = getChildren(outputParam, &outputParamSize);

			for(int k=0; k<outputParamSize; k++){
				tablePtr *tabPtr1 = functionList[i]->th->localTable[hashFuncLUT(outputParamList[k]->terminal->lexeme, 10000)];
				record **recArr = (record**)malloc(callee->numOfOutPar*sizeof(record*));
				record *recHead = callee->outParList;
				
                for(int m=0; m<callee->numOfOutPar; m++){
                    recArr[m] = recHead;
                    recHead = recHead->next;
				}

				if(tabPtr1==NULL || (strcmp(tabPtr1->type, recArr[k]->type)!=0)){
					printf("Line :%llu type of output parameter in function %s does not match the return type\n", 
                    outputParamList[k]->terminal->lineNum, callee->funName);
					error = -1;
				}

				if(assignedVal!=NULL){
					for(int m=0; m<idsSize; m++){
						tablePtr *tabPtr2 = functionList[i]->th->localTable[hashFuncLUT(idsList[m]->terminal->lexeme, 10000)];
						if(tabPtr1==NULL || tabPtr2==NULL) continue;	
						if(strcmp(tabPtr2->type, tabPtr1->type)==0) assignedVal[m] = 1;
					}
				}
			}

            parseTree inputParam = &(stmt->children[2]);
			int inputParamSize = 0;
			parseTree *inputParamList = getChildren(inputParam, &inputParamSize); 

			printf("%d, %s\n", inputParamSize, callee->funName);
			
            if(inputParamSize!=callee->numOfInPar){
				printf("Line : %llu Number of input parameters required for function %s does not match number of input parameters\n", stmt->children[1].terminal->lineNum, stmt->children[1].terminal->lexeme);
				error = -1;
			}

			int inputSize = 0;
			parseTree* inputList = getChildren(inputParam, &inputSize);

            for(int k=0; k<inputSize; k++){
				if(!functionList[i]->th) {
					printf("Line : %llu type of input parameter in function %s does not match the input argument type\n", 
                    inputList[k]->terminal->lineNum, callee->funName);
					error = -1;
					continue;
				}

				tablePtr *tabPtr1 = functionList[i]->th->localTable[hashFuncLUT(inputList[k]->terminal->lexeme, 10000)];
				record **recArr = (record**)malloc(callee->numOfInPar*sizeof(record*));
				record *recHead = callee->inParList;

				for(int m=0; m<callee->numOfInPar; m++){
                    recArr[m] = recHead;
                    recHead = recHead->next;
				}

				if(tabPtr1==NULL || (strcmp(tabPtr1->type, recArr[k]->type)!=0)){
					printf("Line : %llu type of input parameter in function %s does not match the input argument type\n", 
                    inputList[k]->terminal->lineNum, callee->funName);
					error = -1;
				}
			}

            break;
        default:
            return 0;
    }
}

int checkSemanticsOfFunction(parseTree root, symbolTable s) {
    int error = 0;
    if(root->numChild==2) {
        int size = 0;
        parseTree *functionList = getChildren(&(root->children[0]), &size);

        int flag = 0;
        for(int i=0; i<size; i++) {
            tableHeader *tmpEntry = functionList[i]->th;
            if(functionList[i]->children[functionList[i]->numChild-1].nt==stmts) {
                parseTree tmp = &(functionList[i]->children[functionList[i]->numChild-1]);

                if(tmp->children[tmp->numChild-1].nt==returnStmt || 
                (tmp->children[tmp->numChild-1].terminal!=NULL && 
                tmp->children[tmp->numChild-1].terminal->tokenType==TK_ID)) {
                    parseTree returnStmtNode = &(tmp->children[tmp->numChild-1]);

					if(returnStmtNode->numChild!=0 && returnStmtNode->numChild != tmpEntry->numOfOutPar){
						printf("Error in function %s, number of parameters does not match the number output parameters in function definition\n", 
                        tmpEntry->funName);
						error = -1; 
					}else if(tmpEntry->numOfOutPar != 1){
						printf("Error in function %s, number of parameters does not match the number output parameters in function definition\n", 
                        tmpEntry->funName);
						error = -1; 
					}

					int idSize = 0;
					parseTree *idList;
					if(tmp->children[tmp->numChild-1].nt==returnStmt){
						idList = getChildren(&(returnStmtNode->children[0].children[0]), &idSize);
					}else{
						idList = malloc(sizeof(parseTree));
						idList[0] = returnStmtNode;
						idSize = 1;
					}

					for(int j=0; j<idSize; j++) {
						if(!functionList[i]->th) {
							flag = 1;
							break;
						}

                        tablePtr *tabPtr;
						if(s->fTable[hashFuncLUT("global",10000)]->localTable[hashFuncLUT(idList[j]->terminal->lexeme, 10000)]) {
                            tabPtr = s->fTable[hashFuncLUT("global",10000)]->localTable[hashFuncLUT(idList[j]->terminal->lexeme, 10000)];
                        }else {
                            tabPtr = functionList[i]->th->localTable[hashFuncLUT(idList[j]->terminal->lexeme, 10000)];
                        }
						
						if(!tabPtr || tabPtr->type==NULL){
							flag = 1;
							break;
						}
							
						record **recArr = (record**)malloc(tmpEntry->numOfOutPar*sizeof(record*));
						record *recHead = tmpEntry->outParList;
						
                        for(int m=0; m<tmpEntry->numOfOutPar; m++) {
							recArr[m] = recHead;
							recHead = recHead->next;
						}

						if(recArr[j]!=NULL && strcmp(tabPtr->type, recArr[j]->type)!=0) {
							printf("line : %llu returned type of %s does not match expected type\n",
                            idList[j]->terminal->lineNum, idList[j]->terminal->lexeme);
							error=-1;
						}
					}

                    if(flag) continue;

                    if(functionList[i]->children[functionList[i]->numChild-2].nt==output_par) {
						parseTree outputParam = &(functionList[i]->children[functionList[i]->numChild-2]);
						int idsSize = 0;
						parseTree* idsList = getChildren(outputParam, &idsSize);
						int *assignedVal = (int*)malloc(idsSize*sizeof(int));
						memset(assignedVal, 0, idsSize);

						if(tmp->numChild!=1 && tmp->children[tmp->numChild-2].nt==otherStmts){
							parseTree otherStmts = &(tmp->children[tmp->numChild-2]);
							int stmtSize = 0;
							parseTree *stmtList = getChildren(otherStmts, &stmtSize);

							for(int k=0; k <stmtSize; k++) {
								if(checkForFunctionCallStmts(&(stmtList[k]->children[0]), functionList, 
                                s, assignedVal, outputParam, i, size)==-1) error = -1;
							} 
							
                            if(outputParam!=NULL) {
								for(int l=0; l<idsSize; l++) {
									if(assignedVal[l]==0) error=-1;
								}
							}
						}
					}
                }else {
                    if(functionList[i]->children[functionList[i]->numChild - 2].nt==output_par){
						parseTree outputParam = &(functionList[i]->children[functionList[i]->numChild-2]);
						int idsSize = 0;
						parseTree* idsList = getChildren(outputParam, &idsSize);
						int *assignedVal = (int*)malloc(idsSize*sizeof(int));
						memset(assignedVal, 0, idsSize);
						if(tmp->numChild!=1 && tmp->children[tmp->numChild-2].nt==otherStmts) {
							parseTree otherStmts = &(tmp->children[tmp->numChild-2]);
							int stmtSize = 0;
							parseTree *stmtList = getChildren(otherStmts, &stmtSize);

							for(int k=0; k<stmtSize; k++){
								if(checkForFunctionCallStmts(stmtList[k], functionList, 
                                s, assignedVal, outputParam, i, size)==-1) error = -1;
							}

							if(outputParam!=NULL){
								for(int l=0; l<idsSize; l++){
									if(assignedVal[l]==0){
										printf("%s output parameter not assigned ay value before returning it in function %s\n", idsList[l]->terminal->lexeme, functionList[i]->children[0].terminal->lexeme);
										error=-1;
									}
								}
							}
						}
					}
                }
            }
        }

        size = 0;
        functionList = getChildren(&(root->children[1]), &size);
        parseTree tmp = &(root->children[1].children[0]);

        if(tmp->numChild!=1 && 
        tmp->children[tmp->numChild-2].nt==otherStmts) {
            parseTree otherStmts = &(tmp->children[tmp->numChild-2]);
            int stmtSize = 0;
            parseTree *stmtList = getChildren(otherStmts, &stmtSize);

            for(int k=0; k<stmtSize; k++) {
                if(checkForFunctionCallStmts(stmtList[k], functionList,
                s, NULL, NULL, -1, size)==-1) error = -1;
            }
        }
    }else {
        int size = 0;
        parseTree *functionList = getChildren(&(root->children[0]), &size);
        parseTree tmp = &(root->children[0].children[0]);

        if(tmp->numChild!=1 && 
        tmp->children[tmp->numChild-2].nt==otherStmts) {
            parseTree tmpOtherStmts = &(tmp->children[tmp->numChild-2]);
            int stmtSize = 0;
            parseTree *stmtList = getChildren(tmpOtherStmts, &stmtSize);

            for(int k=0; k<stmtSize; k++) {
                if(checkForFunctionCallStmts(stmtList[k], functionList,
                s, NULL, NULL, -1, size)==-1) error = -1;
            }
        }
    }
}