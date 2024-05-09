/* 
* Group 05
* Kush Mehta			2018B5A70956P
* Purusharth Amrut		2018B5A70897P
* Patel Darsh Rajesh	2018B4A70532P
* Harsh Jhunjhunwala	2018B5A70691P
* Jatin Aggarwal		2018B4A70884P
*/

#include "symbolTable.h"

#define TABLE_SIZE 10000
int OFFSET_GLOBAL = 0;

int hashFuncST(char *key, int size) {
    // long long int sum = 0;
    // for(int i=0; i<strlen(key); i++) {
    //     sum *= 10;
    //     sum += (long long int)(key[i]-'0');
    //     if(sum<0) sum = 0;
    // }
    // int hashVal = sum%size;
    // return hashVal;
    const int p = 31, m = 1e9+7;
    int hash = 0;
    long p_pow = 1;
    for(int i=0; i<strlen(key); i++) {
        hash = (hash + (key[i] - '0' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    // printf("Key: %s | Hash: %d\n", key, (hash%9973));
    return hash%(9973);
}

record* insertBack(record *head, record *ptr) {
    if(head==NULL) {
        head = ptr;
        return head;
    }
    record *curr = head, *prev = curr;
    while(curr!=NULL) {
        if(strcmp(curr->recname, ptr->recname)==0) {
            printf("Line %d : %s field already declared in this record\n", ptr->lineNum, curr->recname);
            return head;
        }
        prev = curr;
        curr = curr->next;
    }
    prev->next = ptr;

    return head;
}

int lookUpFunc(tablePtr *tabPtr, char *key) {
    while(tabPtr!=NULL) {
        if(strcmp(tabPtr->name, key)==0) return 1;
        tabPtr = tabPtr->next;
    }
    return 0;
}

void addEntryBack(tablePtr **table, tablePtr *tabPtr) {
    tablePtr *tmp = *table, *prev = *table;

    if(tmp==NULL) {
        *table = tabPtr;
        return;
    }

    while(tmp!=NULL) {
        prev = tmp;
        tmp = tmp->next;
    }
    prev->next = tabPtr;
}

/*============================================================================================*/

symbolTable createSymbolTable(int size) {
    symbolTable st = (symbolTable)malloc(sizeof(symboltable));
    st->numOfFunc = 0;
    st->functions = NULL;
    st->fTable = (tableHeader**)malloc(size*sizeof(tableHeader*));

    for(int i=0; i<size; i++) st->fTable[i] = NULL;

    return st;
}

tablePtr *createNewEntry(char *dataType, char *idName, int idSize, int offset, record *ptr, paramDetail *param) {
    tablePtr *tabPtr = malloc(sizeof(tablePtr));

    tabPtr->type = dataType;
    tabPtr->name = idName;
    tabPtr->size = idSize;
    tabPtr->offset = offset;
    tabPtr->ptr = ptr;
    tabPtr->param = param;
    tabPtr->next = NULL;

    return tabPtr;
}

tableHeader* createTableHeader(char *funName, int size) {
    tableHeader *th = malloc(sizeof(tableHeader));
    th->localTable = (tablePtr**)malloc(size*sizeof(tablePtr*));

    for(int i=0; i<size; i++) th->localTable[i] = NULL;

    th->funName = funName;
    th->numOfVar = 0;
    th->funSize = 0;
    th->numOfInPar = 0;
    th->numOfOutPar = 0;
    th->inParList = NULL;
    th->outParList = NULL;
    th->varList = NULL;

    return th;
}

/*============================================================================================*/

void populateGlobalRecordsHelperDefineType(tableHeader *th, parseTree ast) {
    // ast==defineTypeStmt
    // defineTypeStmt -> TK_DEFINETYPE A TK_RUID TK_AS TK_RUID

    // TODO: defineTypeStmt is not completed for symbolTable
    if(!th || !ast) return;

    tablePtr *tabPtr = th->localTable[hashFuncST(ast->children[2].terminal->lexeme, TABLE_SIZE)];

    if(!lookUpFunc(tabPtr, ast->children[2].terminal->lexeme)) {
        printf("Line %lld : constructed datatype %s is not defined.\n", ast->children[2].terminal->lineNum, ast->children[2].terminal->lexeme);
		return;
    }
    
    // tabPtr = createNewEntry("record", ast->children[2].terminal->lexeme, 0, OFFSET_GLOBAL, NULL, NULL);
    // addEntryBack(&(th->localTable[hashFuncST(ast->children[2].terminal->lexeme, TABLE_SIZE)]), tabPtr);

    // record *varPtr = malloc(sizeof(record));
    // varPtr->recname = ast->children[2].terminal->lexeme;
    // varPtr->type = NULL;
    // varPtr->next = NULL;

    // th->varList = insertBack(th->varList, varPtr);
    // th->numOfVar++;

    // OFFSET_GLOBAL += tabPtr->size;
}

void populateGlobalRecordsHelperUtils2(tablePtr *tabPtr, parseTree ast) {
    // ast==fieldDefinition/fieldDefinitions/moreFields
    if(!tabPtr || !ast || !ast->numChild) return;

    if(ast->nt==fieldDefinition) {
        record *ptr = malloc(sizeof(record));
        ptr->recname = ast->children[1].terminal->lexeme;
        // TODO: check if TK_RUID is declared or not and add its size accordingly
        ptr->type = ast->children[0].terminal->lexeme;
        ptr->lineNum = ast->children[1].terminal->lineNum;
        ptr->next = NULL;

        tabPtr->size += ((ptr->type=="int") ? 2 : 4);
        tabPtr->ptr = insertBack(tabPtr->ptr, ptr);
        return;
    }

    for(int i=0; i<ast->numChild; i++) populateGlobalRecordsHelperUtils2(tabPtr, &(ast->children[i]));
}

void populateGlobalRecordsHelperUtils(tableHeader *th, parseTree ast) {
    //ast==typeDefinition
    // typeDefinition -> TK_RECORD TK_RUID fieldDefinitions
    if(!th || !ast) return;
    
    tablePtr *tabPtr = th->localTable[hashFuncST(ast->children[1].terminal->lexeme, TABLE_SIZE)];

    if(lookUpFunc(tabPtr, ast->children[1].terminal->lexeme)) {
        printf("Line %lld : record %s is global, cannot be declared more than once.\n",
        ast->children[1].terminal->lineNum, ast->children[1].terminal->lexeme);
		return;
    }

    tabPtr = createNewEntry("record", ast->children[1].terminal->lexeme, 0, OFFSET_GLOBAL, NULL, NULL);
    addEntryBack(&(th->localTable[hashFuncST(ast->children[1].terminal->lexeme, TABLE_SIZE)]), tabPtr);

    record *varPtr = malloc(sizeof(record));
    varPtr->recname = ast->children[1].terminal->lexeme;
    varPtr->type = NULL;
    varPtr->next = NULL;

    th->varList = insertBack(th->varList, varPtr);
    th->numOfVar++;

    // populating fields of constructed datatypes
    populateGlobalRecordsHelperUtils2(tabPtr, &(ast->children[2]));
    OFFSET_GLOBAL += tabPtr->size;
}

/*============================================================================================*/

void populateGlobalRecordsHelper(tableHeader *th, parseTree ast) {
    // ast==typedefinitions
    // typeDefinitions -> [typeDefinition -> TK_RECORD TK_RUID fieldDefinitions] typeDefinitions
    if(!ast || !ast->numChild) return;

    if(strcmp(ast->children[0].children[0].terminal->lexeme, "TK_DEFINETYPE")==0) {
        populateGlobalRecordsHelperDefineType(th, &(ast->children[0]));
    }else {
        populateGlobalRecordsHelperUtils(th, &(ast->children[0]));
    }

    populateGlobalRecordsHelper(th, &(ast->children[1]));
}

void populateGlobalIDsHelper(tableHeader *th, parseTree ast) {
    // ast==declarations
    if(!th || !ast || !ast->numChild) return;

    // getting declaration from declarations
    parsetree declar = ast->children[0];

    if(declar.numChild==3 && declar.children[2].numChild==1) {
        // declaration -> dataType TK_ID global_or_not
        // only global variable is processed

        tablePtr *tabPtr;
        if(declar.children[0].nt==-1) {
            // INT or REAL
            tabPtr = th->localTable[hashFuncST(declar.children[1].terminal->lexeme, TABLE_SIZE)];
            
            if(lookUpFunc(tabPtr, declar.children[1].terminal->lexeme)) {
                printf("Line %lld : Variable %s is a global variable, cannot be declared more than once.\n", 
                declar.children[1].terminal->lineNum, declar.children[1].terminal->lexeme);
				return;
            }

            char *dataType = (declar.children[0].terminal->tokenType==TK_INT) ? "int" : "real";
            int idSize = (dataType=="int") ? 2 : 4;

            tabPtr = createNewEntry(dataType, declar.children[1].terminal->lexeme, idSize, OFFSET_GLOBAL, NULL, NULL);
            addEntryBack(&(th->localTable[hashFuncST(declar.children[1].terminal->lexeme, TABLE_SIZE)]), tabPtr);
            OFFSET_GLOBAL += idSize;

            record *varPtr = malloc(sizeof(record));
            varPtr->recname = declar.children[1].terminal->lexeme;
            varPtr->type = NULL;
            varPtr->next = NULL;

            th->varList = insertBack(th->varList, varPtr);
            th->numOfVar++;
        }else {
            // constructed datatype
            // declaration -> [constructedDatatype -> TK_RECORD TK_RUID / TK_UNION TK_RUID / TK_RUID (this will give error)] 
            // .. TK_ID global_or_not

            tabPtr = th->localTable[hashFuncST(declar.children[0].children[1].terminal->lexeme, TABLE_SIZE)];

            // checking if record/union datatype is defined before or not
            if(!lookUpFunc(tabPtr, declar.children[0].children[1].terminal->lexeme)) {
                printf("Line %lld : constructed datatype %s is not defined.\n",
                declar.children[0].children[1].terminal->lineNum, declar.children[0].children[1].terminal->lexeme);
				return;
            }

            char *dataType = declar.children[0].children[1].terminal->lexeme;
            int idSize = tabPtr->size;
            record *ptr = tabPtr->ptr;

            tablePtr *tmp = createNewEntry(dataType, declar.children[1].terminal->lexeme, idSize, OFFSET_GLOBAL, ptr, NULL);
            addEntryBack(&(th->localTable[hashFuncST(declar.children[1].terminal->lexeme, TABLE_SIZE)]), tmp);
            OFFSET_GLOBAL += idSize;

            record *varPtr = malloc(sizeof(record));
            varPtr->recname = declar.children[1].terminal->lexeme;
            varPtr->type = NULL;
            varPtr->next = NULL;

            th->varList = insertBack(th->varList, varPtr);
        }
    }
    // recursively call for declarations_1
    populateGlobalIDsHelper(th, &(ast->children[1]));
}

/*============================================================================================*/

void populateGlobalRecords(parseTree ast, symbolTable st, tableHeader *th) {
    if(!ast || !st) return;

    if(ast->nt==typeDefinitions) {
        populateGlobalRecordsHelper(th, ast);
        return;
    }

    for(int i=0; i<ast->numChild; i++) populateGlobalRecords(&(ast->children[i]), st, th);
}

void populateGlobalIDs(parseTree ast, symbolTable st, tableHeader *th) {
    if(!ast || !st) return;

    if(ast->nt==declarations) {
        populateGlobalIDsHelper(th, ast);
        return;
    }

    for(int i=0; i<ast->numChild; i++) populateGlobalIDs(&(ast->children[i]), st, th);
}

/*============================================================================================*/

char* getDataType(parseTree ast) {
    //ast==constructedDatatype
    // constructedDatatype -> TK_RECORD TK_RUID / TK_UNION TK_RUID / TK_RUID
    if(!ast) return NULL;
    return ast->children[1].terminal->lexeme;
}

void populateFuncParameters(symbolTable st, parseTree ast, int idx, paramDetail *param) {
    //ast==parameter_list
    // parameter_list -> dataType TK_ID [remaining_list -> parameter_list]
    int hashIdx = hashFuncST(ast->children[1].terminal->lexeme, TABLE_SIZE);

    // printf("orig: %s\n", ast->children[1].terminal->lexeme);
    // printSymbolTable(st);

    // checking for variable collisions with global or in scope variables
    int flag = 0;
    if(lookUpFunc(st->fTable[hashFuncST("global", TABLE_SIZE)]->localTable[hashIdx], ast->children[1].terminal->lexeme)) {
        printf("Line %lld : variable %s declared here is already declared as a global variable.\n",
        ast->children[1].terminal->lineNum, ast->children[1].terminal->lexeme);
        flag = 1;
    }else if(lookUpFunc(st->fTable[idx]->localTable[hashIdx], ast->children[1].terminal->lexeme)) {
        // printf("here\n");
        printf("Line %lld : variable %s declared here is already in this scope from the parent function.\n",
        ast->children[1].terminal->lineNum, ast->children[1].terminal->lexeme);
        flag = 1;
    }

    if(flag) return;
    
    if(!flag) {
        if(param->isInpPar) param->parIdx = st->fTable[idx]->numOfInPar;
        else if(param->isOutPar) param->parIdx = st->fTable[idx]->numOfOutPar;

        st->fTable[idx]->numOfVar++;

        tablePtr *tabPtr;
        char strInt[] = "int", strReal[] = "real";
        record *paramListPtr = malloc(sizeof(record));

        if(ast->children[0].nt==constructedDatatype) {
            char *dataType = getDataType(&(ast->children[0].children[0]));
            int consHashVal = hashFuncST(dataType, TABLE_SIZE);

            tabPtr = st->fTable[hashFuncST("global", TABLE_SIZE)]->localTable[consHashVal];

            if(!lookUpFunc(tabPtr, dataType)) {
                printf("Line %lld : constructed datatype %s is not defined.\n",
                ast->children[0].children[0].terminal->lineNum, dataType);
                return;
            }

            int consSize = tabPtr->size;
            tabPtr = createNewEntry(dataType, ast->children[1].terminal->lexeme, consSize, st->fTable[idx]->funSize, tabPtr->ptr, param);
            st->fTable[idx]->funSize += consSize;
            
            paramListPtr->recname = ast->children[1].terminal->lexeme;
            paramListPtr->type = dataType;
            paramListPtr->lineNum = ast->children[1].terminal->lineNum;

            if(param->isInpPar) {
                st->fTable[idx]->inParList = insertBack(st->fTable[idx]->inParList, paramListPtr);
                st->fTable[idx]->numOfInPar++;
            }else if(param->isOutPar) {
                st->fTable[idx]->outParList = insertBack(st->fTable[idx]->outParList, paramListPtr);
                st->fTable[idx]->numOfOutPar++;
            }
        }else if(strcmp(ast->children[0].terminal->lexeme, "int")==0) {
            tabPtr = createNewEntry("int", ast->children[1].terminal->lexeme, 2, st->fTable[idx]->funSize, NULL, param);
            st->fTable[idx]->funSize += 2;

            paramListPtr->recname = ast->children[1].terminal->lexeme;
            paramListPtr->type = strInt;
            paramListPtr->lineNum = ast->children[1].terminal->lineNum;

            if(param->isInpPar) {
                st->fTable[idx]->inParList = insertBack(st->fTable[idx]->inParList, paramListPtr);
                st->fTable[idx]->numOfInPar++;
            }else if(param->isOutPar) {
                st->fTable[idx]->outParList = insertBack(st->fTable[idx]->outParList, paramListPtr);
                st->fTable[idx]->numOfOutPar++;
            }
        }else {
            tabPtr = createNewEntry("real", ast->children[1].terminal->lexeme, 4, st->fTable[idx]->funSize, NULL, param);
            st->fTable[idx]->funSize += 4;

            paramListPtr->recname = ast->children[1].terminal->lexeme;
            paramListPtr->type = strReal;
            paramListPtr->lineNum = ast->children[1].terminal->lineNum;
            
            if(param->isInpPar) {
                st->fTable[idx]->inParList = insertBack(st->fTable[idx]->inParList, paramListPtr);
                st->fTable[idx]->numOfInPar++;
            }else if(param->isOutPar) {
                st->fTable[idx]->outParList = insertBack(st->fTable[idx]->outParList, paramListPtr);
                st->fTable[idx]->numOfOutPar++;
            }
        }
        addEntryBack(&(st->fTable[idx]->localTable[hashIdx]), tabPtr);
    }

    paramDetail *paramTmp;
    if(param->isInpPar) {
        paramTmp = malloc(sizeof(paramDetail));
        paramTmp->isInpPar = true;
        paramTmp->isOutPar = false;
        paramTmp->parIdx = 0;
    }else if(param->isOutPar) {
        paramTmp = malloc(sizeof(paramDetail));
        paramTmp->isInpPar = false;
        paramTmp->isOutPar = true;
        paramTmp->parIdx = 0;
    }
    if(ast->children[2].numChild) populateFuncParameters(st, &(ast->children[2].children[0]), idx, paramTmp);
}

void populateStmtVarUtils2(symbolTable st, tableHeader *th, parseTree ast, int idx) {
    //ast=declaration
    //declaration -> [TK_INT/TK_REAL/constructedDatatype] TK_ID global_or_not 
    if(!st || !th || !ast) return;

    // don't process global variables
    if(ast->children[2].numChild==1) return;

    tablePtr *tabPtr = st->fTable[hashFuncST("global", TABLE_SIZE)]->localTable[hashFuncST(ast->children[1].terminal->lexeme, TABLE_SIZE)];

    if(lookUpFunc(tabPtr, ast->children[1].terminal->lexeme)) {
        printf("Line %lld : %s record definition is already declared globally.\n",
        ast->children[1].terminal->lineNum, ast->children[1].terminal->lexeme);
		return;
    }

    tabPtr = th->localTable[hashFuncST(ast->children[1].terminal->lexeme, TABLE_SIZE)];

    if(lookUpFunc(tabPtr, ast->children[1].terminal->lexeme)) {
        printf("Line %lld : %s already declared in this function.\n",
        ast->children[1].terminal->lineNum, ast->children[1].terminal->lexeme);
		return;
    }

    char *dataTypeStr;
    int dataTypeSize;
    tablePtr *tmpPtr;

    if(ast->children[0].nt==constructedDatatype) {
        dataTypeStr = getDataType(&(ast->children[0]));
        tabPtr = st->fTable[hashFuncST("global", TABLE_SIZE)]->localTable[hashFuncST(dataTypeStr, TABLE_SIZE)];

        if(!lookUpFunc(tabPtr, dataTypeStr)) {
            printf("Line %lld : constructed dataTypeStr %s is not defined.\n",
            ast->children[0].children[0].children[1].terminal->lineNum, dataTypeStr);
			return;
        }

        dataTypeSize = tabPtr->size;
        tmpPtr = createNewEntry(dataTypeStr, ast->children[1].terminal->lexeme, dataTypeSize, th->funSize, tabPtr->ptr, NULL);
    }else {
        dataTypeStr = ast->children[0].terminal->lexeme;
        dataTypeSize = (ast->children[0].terminal->tokenType==TK_INT) ? 2 : 4;
        tmpPtr = createNewEntry(dataTypeStr, ast->children[1].terminal->lexeme, dataTypeSize, th->funSize, NULL, NULL);
    }
    addEntryBack(&(th->localTable[hashFuncST(ast->children[1].terminal->lexeme, TABLE_SIZE)]), tmpPtr);

    record *varPtr = malloc(sizeof(record));
    varPtr->recname = ast->children[1].terminal->lexeme;
    varPtr->type = NULL;
    varPtr->next = NULL;

    th->varList = insertBack(th->varList, varPtr);
    th->numOfVar++;
    th->funSize += dataTypeSize;
}

void populateStmtVarUtils(symbolTable st, tableHeader *th, parseTree ast, int idx) {
    //ast==declarations
    if(!th || !ast || !ast->numChild) return;

    populateStmtVarUtils2(st, th, &(ast->children[0]), idx);

    // recusively calling declarations_1
    populateStmtVarUtils(st, th, &(ast->children[1]), idx);
}

void populateStmtVar(symbolTable st, tableHeader *th, parseTree ast, int idx) {
    //ast==stmts
    if(!th || !ast) return;
    
    for(int i=0; i<ast->numChild; i++) {
        if(ast->children[i].nt==declarations) populateStmtVarUtils(st, th, &(ast->children[i]), idx);
    }
}

void populateFuncUtils(symbolTable st, parseTree ast, char *funcName) {
    //ast==function/_main

    int idx = hashFuncST(funcName, TABLE_SIZE);

    if(st->fTable[idx]!=NULL) {
        if(ast->nt==function) printf("Line %lld : function %s is already declared before.\n", ast->children[0].terminal->lineNum, funcName);
        else printf("_main function declared twice");
        return;
    }

    record *varPtr = malloc(sizeof(record));
    varPtr->recname = funcName;
    varPtr->type = NULL;
    varPtr->next = NULL;

    st->functions = insertBack(st->functions, varPtr);

    tableHeader *th = createTableHeader(funcName, TABLE_SIZE);
    ast->th = th;
    st->fTable[idx] = th;

    if(ast->nt==function) {
        // function -> TK_FUNID input_par output_par stmts
        
        for(int i=0; i<ast->numChild; i++) {
            if(ast->children[i].nt==input_par) {
                // input_par -> parameter_list

                paramDetail *param = malloc(sizeof(paramDetail));
                param->isInpPar = true;
                param->isOutPar = false;
                param->parIdx = 0;
                populateFuncParameters(st, &(ast->children[i].children[0]), idx, param);
            }else if(ast->children[i].nt==output_par) {
                // output_par -> parameter_list (or it can be eps)
                if(!ast->children[i].numChild) continue;

                paramDetail *param = malloc(sizeof(paramDetail));
                param->isInpPar = false;
                param->isOutPar = true;
                param->parIdx = 0;
                populateFuncParameters(st, &(ast->children[i].children[0]), idx, param);
            }
        }
    }
    
    //mainFunction -> stmts
    // stmts is always last in both rules
    populateStmtVar(st, th, &(ast->children[ast->numChild-1]), idx);
}

void populateFuncOther(symbolTable st, parseTree ast) {
    //ast==otherFunctions
    if(!st || !ast || !ast->numChild) return;

    populateFuncUtils(st, &(ast->children[0]), ast->children[0].children[0].terminal->lexeme);

    // recursively calling otherFunctions_1 
    populateFuncOther(st, &(ast->children[1]));
}

void populateFunc(symbolTable st, parseTree ast) {
    // ast==program
    if(!st || !ast) return;
    
    populateFuncOther(st, &(ast->children[0]));
    populateFuncUtils(st, &(ast->children[1]), "_main");}

/*============================================================================================*/

symbolTable populateSymbolTable(parseTree ast) {
    symbolTable st = createSymbolTable(TABLE_SIZE);
    tableHeader *th = createTableHeader("global", TABLE_SIZE);
    st->fTable[hashFuncST("global", TABLE_SIZE)] = th;

    record *varPtr = malloc(sizeof(record));
    varPtr->recname = "global";
    varPtr->type = NULL;
    varPtr->next = NULL;
    
    st->functions = insertBack(st->functions, varPtr);

    populateGlobalRecords(ast, st, th);
    populateGlobalIDs(ast, st, th);
    populateFunc(st, ast);

    return st;
}

/*============================================================================================*/

void printFuncTableVariables(record *tmp, tableHeader *th) {
    char *lex, *dataType, *scope;
    int offset;

    scope = th->funName;

    while(tmp!=NULL) {
        char *str = tmp->recname;
        tablePtr *tabPtr;
        tabPtr = th->localTable[hashFuncST(str, TABLE_SIZE)];
        
        if(tabPtr==NULL) {
            tmp = tmp->next;
            continue;
        }

        while(tabPtr!=NULL) {
            lex = tabPtr->name;
            dataType = tabPtr->type;
            offset = tabPtr->offset;

            printf("%s %20s %20s %20s %20d %20s %20d %20s\n", 
            lex, scope, (str[0]=='#' ? str : "---"), dataType, tabPtr->size, "---", offset, "variableUsage");

            tabPtr = tabPtr->next;
        }
        tmp = tmp->next;
    }
}

void printFuncTable(tableHeader *th) {
    if(!th) return;
    int flag = 0;
    // printf("----------------------------%s table----------------------------\n", th->funName);
    if(strcmp(th->funName, "global")==0) flag = 1;

    record *tmp;

    switch(flag) {
        case 0:
            printFuncTableVariables(th->inParList, th);
            printFuncTableVariables(th->outParList, th);
            printFuncTableVariables(th->varList, th);
            
            break;
        case 1:
            ;
            char *lex, *dataType, *scope;
            int offset;

            scope = th->funName;
            tmp = th->varList;

            while(tmp!=NULL) {
                char *str = tmp->recname;

                if(str[0]=='#') {
                    tmp = tmp->next;
                    continue;
                }

                tablePtr *tabPtr = th->localTable[hashFuncST(str, TABLE_SIZE)];

                if(tabPtr==NULL) {
                    tmp = tmp->next;
                    continue;
                }

                while(tabPtr!=NULL) {
                    lex = tabPtr->name;
                    dataType = tabPtr->type;
                    offset = tabPtr->offset;
                    
                    printf("%s %20s %20s ", lex, scope, (str[0]=='#' ? str : "---"));

                    // datatype
                    if(tabPtr->ptr!=NULL) {
                        record *rPtr;
                        rPtr = tabPtr->ptr;

                        printf("%20s", "<");
                        while(rPtr!=NULL) {
                            printf("%s", rPtr->type);
                            if(rPtr->next!=NULL) printf(", ");
                            rPtr = rPtr->next;
                        }
                        printf("> ");
                    }else printf("%20s ", dataType);
                    printf("%20d %20s %20d %20s\n", tabPtr->size, "global", offset, "variableUsage");

                    tabPtr = tabPtr->next;
                }
                tmp = tmp->next;
            }

            break;
    }
    printf("\n");
}

void printSymbolTable(symbolTable st) {
    if(!st) return;
    printf("\n===================Symbol Table=======================\n");
    printf("\nlexeme \t\t scope \t\t typeName \t\t typeExpression \t\t width \t\t isGlobal \t\t offset \t\t variableUsage\n\n");

    record *tmp = st->functions;
    while(tmp!=NULL) {
        char *funcName = tmp->recname;
        printFuncTable(st->fTable[hashFuncST(funcName, TABLE_SIZE)]);
        tmp = tmp->next;
    }
    printf("\n======================================================\n");
}

void printGlobalVariables(symbolTable st) {
    printf("\n=================Global Variables=====================\n");
    printf("\nlexeme		type				scope		offset\n\n");
    tableHeader *th = st->fTable[hashFuncST("global", TABLE_SIZE)];
    if(!th) return;

    char *lex, *dataType, *scope;
    int offset;
    record *tmp = th->varList;

    scope = th->funName;

    while(tmp!=NULL) {
        char *str = tmp->recname;
        
        if(str[0]=='#') {
            tmp = tmp->next;
            continue;
        }

        tablePtr *tabPtr;
        tabPtr = th->localTable[hashFuncST(str, TABLE_SIZE)];

        if(!tabPtr) {
            tmp = tmp->next;
            continue;
        }

        while(tabPtr!=NULL) {
            lex = tabPtr->name;
            dataType = tabPtr->type;
            offset = tabPtr->offset;

            printf("%s		", lex);
            if(tabPtr!=NULL && dataType[0]=='#') {
                record *rPtr;
                rPtr = tabPtr->ptr;

                while(rPtr!=NULL) {
                    printf("%s", rPtr->type);
                    if(rPtr->next!=NULL) printf(" x ");

                    rPtr = rPtr->next;
                }
                printf("		");
            }else printf("%s				", dataType);

            printf("%s		%d\n", scope, offset);

            tabPtr = tabPtr->next;
        }
        tmp = tmp->next;
    }
    printf("\n======================================================\n");
}

void printFuncNameAndSizes(symbolTable st) {
    printf("\n=============Activation record sizes==================\n");
    printf("\nfunctionName		size\n\n");
    record *tmp = st->functions;
    tableHeader *th;

    while(tmp!=NULL) {
        char *str = tmp->recname;
        if(strcmp(str, "global")==0) {
            tmp = tmp->next;
            continue;
        }
        th = st->fTable[hashFuncST(str, TABLE_SIZE)];
        printf("%s		%d\n", th->funName, th->funSize);
		tmp = tmp->next;
    }
    printf("\n======================================================\n");
}

void printRecordsTypesAndSizes(symbolTable st) {
    printf("\n==============Record types and sizes==================\n");
    printf("\nrecordName		typeExpression		width\n\n");
    tableHeader *th = st->fTable[hashFuncST("global", TABLE_SIZE)];
    if(!th) return;

    char *lex;
    int width;
    record *tmp = th->varList;

    while(tmp!=NULL) {
        char *str = tmp->recname;
        if(str[0]!='#') {
            tmp = tmp->next;
            continue;
        }
        tablePtr *tabPtr = th->localTable[hashFuncST(str, TABLE_SIZE)];

        if(tabPtr==NULL) {
            tmp = tmp->next;
            continue;
        }

        while(tabPtr!=NULL) {
            lex = tabPtr->name;
            width = tabPtr->size;

            printf("%s		<", lex);
            if(tabPtr!=NULL) {
                record *rPtr;
				rPtr = tabPtr->ptr;
				while(rPtr!=NULL) {
					printf("%s", rPtr->type);
					if(rPtr->next!=NULL) printf(", ");
					rPtr=rPtr->next;
				}			
				printf(">		%d\n",width);
            }
            tabPtr = tabPtr->next;
        }
        tmp = tmp->next;
    }    

    printf("\n======================================================\n");
}