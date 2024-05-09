/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#include "typechecker.h"

int TypeChecker(parsetree root, symbolTable sTable){

	if(root.nt == function || root.nt == mainFunction)
	{
		tp = root.th;
	} 

	int Atype = error + 1;
	int Btype = Atype + 1;

	if(root.nt != -1){
		switch(root.nt){
			case assignmentStmt:			
				Atype = TypeChecker(root.children[0], sTable);
				Btype = TypeChecker(root.children[1], sTable);

				if(Atype == error || Btype == error) return error;

				else if(Atype==integer && Btype==integer){
					return integer;
				} else if(Atype==integer && Btype==real){
					parsetree temp = root;
					while(temp.terminal == NULL && temp.numChild != 0){
						temp = temp.children[0];
					}
					printf("Type error in line %llu: type mismatch", temp.terminal->lineNum);
					break;
				} else if(Atype==real && Btype==integer){
					return real;
				} else if(Atype==real && Btype==real){
					return real;
				}

			case booleanExpression:			
				if(root.numChild == 3){
					Atype = TypeChecker(root.children[0], sTable);
					Btype = TypeChecker(root.children[2], sTable);

					if((root.children[1].terminal->tokenType == TK_AND) || (root.children[1].terminal->tokenType == TK_OR)){
						if(Atype != boolean || Btype != boolean){
							parsetree temp = root;
							while(temp.terminal == NULL && temp.numChild != 0){
			
								temp = temp.children[0];
							}
							printf("Type error in line %llu: Logical operators are applied only to boolean types but your expression contins non-boolean types. \n", temp.terminal->lineNum);
							return error;
							break;
						}	
					} else if(root.children[1].terminal->tokenType == TK_OR){
						if(Atype != boolean || Btype != boolean){
							parsetree temp = root;
							while(temp.terminal == NULL && temp.numChild != 0){
			
								temp = temp.children[0];
							}
							printf("Type error in line %llu: Logical operators are applied only to boolean types but your expression contins non-boolean types. \n", temp.terminal->lineNum);
							return error;
							break;
						}
					}
					else{
						if((Atype!= integer && Atype!= real)||(Btype!=integer&&Btype!=real))
						{
							printf("line %llu: TYPE ERROR - relational operator requires int or real operands\n", root.children[0].terminal->lineNum);
							return error;
							break;
						}
						if(Atype!=Btype)
						{
							printf("line %llu: TYPE ERROR - both operands should have same type\n", root.children[0].terminal->lineNum);
							return error;
							break;
						}
						return boolean;	
					}
					break;
				}
				else if (root.numChild == 2){
					Atype = TypeChecker(root.children[1], sTable);
					if(Atype == error){
						return error;
						break;
					}
						
					else if(Atype!= boolean)
					{
						printf("line %llu: TYPE ERROR - NOT operator requires boolean operands\n", root.children[0].terminal->lineNum);
						return error;
					}
					return boolean;
				}
				break;

			case arithmeticExpression:			
				Atype = TypeChecker(root.children[0], sTable);
				Btype = TypeChecker(root.children[1], sTable);
				if(Atype==error || Btype==error){
					return error;
				} else if(Atype==integer && Btype==integer){
					return integer;
				} else if(Atype==integer && Btype==real){
					return real;
				} else if(Atype==real && Btype==integer){
					return real;
				} else if(Atype==real && Btype==real){
					return real;
				}
				break;

			case term:			
				Atype = TypeChecker(root.children[0], sTable);
				Btype = TypeChecker(root.children[1], sTable);
				if(Atype==error || Btype==error){
					break;
				} else if(Atype==integer && Btype==integer){
					return integer;
				} else if(Atype==integer && Btype==real){
					return real;
				} else if(Atype==real && Btype==integer){
					return real;
				} else if(Atype==real && Btype==real){
					return real;
				}
				break;

			case termPrime:			
				if(root.numChild == 3){
					Atype = TypeChecker(root.children[1], sTable);
					Btype = TypeChecker(root.children[2], sTable);
					if(Atype==error || Btype==error){
						break;
					} else if(Atype==integer && Btype==integer){
						return integer;
					} else if(Atype==integer && Btype==real){
						return real;
					} else if(Atype==real && Btype==integer){
						return real;
					} else if(Atype==real && Btype==real){
						return real;
					}
				} else{
					Atype = TypeChecker(root.children[1], sTable);
					return Atype;
				}
				break;

			case expPrime:			
				if(root.numChild == 3){
					Atype = TypeChecker(root.children[1], sTable);
					Btype = TypeChecker(root.children[2], sTable);
					if(Atype==error || Btype==error){
						break;
					} else if(Atype==integer && Btype==integer){
						return integer;
					} else if(Atype==integer && Btype==real){
						return real;
					} else if(Atype==real && Btype==integer){
						return real;
					} else if(Atype==real && Btype==real){
						return real;
					}	
				}
				else{
					Atype = TypeChecker(root.children[1], sTable);
					return Atype;
				}
				break;	

			case singleOrRecId:
			;			
				record* rectype;
				tablePtr* globalEntry = sTable->fTable[hashFuncLUT("global", 9973)]->localTable[hashFuncLUT(root.children[0].terminal->lexeme, 9973)];
				if(globalEntry != NULL)
					rectype  = sTable->fTable[hashFuncLUT("global", 9973)]->localTable[hashFuncLUT(root.children[0].terminal->lexeme, 9973)]->ptr;
				else {
					tablePtr* localEntry = tp->localTable[hashFuncLUT(root.children[0].terminal->lexeme, 9973)];
					if (localEntry != NULL)
						rectype = tp->localTable[hashFuncLUT(root.children[0].terminal->lexeme, 9973)]->ptr;	
					else return error;
				}

				if(rectype == NULL) break;
				while(rectype != NULL){
					if(!strcmp(rectype->recname , root.children[1].terminal->lexeme)) {
						if(strcmp(rectype->type,"int")==0)
							return integer;
						else if(strcmp(rectype->type,"real")==0)
							return real;
						else 
							return rec;
					}
					rectype = rectype->next;
				}

			default:
				for(int i = 0; i < root.numChild; i++){
					TypeChecker(root.children[i] , sTable);
				}
				break;
		}
	}
	else{
		switch(root.terminal->tokenType){
			case TK_ID:			
				if(tp == NULL) {
					perror("EMPTY TABLE POINTER!");
					return error;
				}
				
				char* ch;
				tablePtr* globalEntry = sTable->fTable[hashFuncLUT("global", 9973)]->localTable[hashFuncLUT(root.terminal->lexeme, 9973)];
				if(globalEntry != NULL)
					ch = sTable->fTable[hashFuncLUT("global", 9973)]->localTable[hashFuncLUT(root.terminal->lexeme, 9973)]->type;
				else {
					tablePtr* localEntry = tp->localTable[hashFuncLUT(root.terminal->lexeme, 9973)];
					if (localEntry != NULL)
						ch = tp->localTable[hashFuncLUT(root.terminal->lexeme, 9973)]->type;
					else {
						printf("line %llu: variable %s has not been declared\n",root.terminal->lineNum,root.terminal->lexeme);	
						return error;
					}
				}
				if(strcmp(ch,"int")==0)
					return integer;
				else if(strcmp(ch,"real")==0)
					return real;
				else 
					return rec;
				break;

			case TK_NUM:
				return integer;
				break;
			case TK_RNUM:
				return real;
				break;
			case TK_RECORD:
				return rec;
				break;
			case TK_UNION:
				return uniontype;
				break;
			default:
				break;

		}
	}
	
}

