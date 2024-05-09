/* 
* Group 05
* Kush Mehta			2018B5A70956P
* Purusharth Amrut		2018B5A70897P
* Patel Darsh Rajesh	2018B4A70532P
* Harsh Jhunjhunwala	2018B5A70691P
* Jatin Aggarwal		2018B4A70884P
*/

#include "AST.h"

int hashFuncST(char *key, int size);

record* insertBack(record *head, record *ptr);
int lookUpFunc(tablePtr *tabPtr, char *key);
void addEntryBack(tablePtr **table, tablePtr *tabPtr);
symbolTable createSymbolTable(int size);
tablePtr *createNewEntry(char *dataType, char *idName, int idSize, int offset, record *ptr, paramDetail *param);
tableHeader* createTableHeader(char *funName, int size);

void populateGlobalRecordsHelperDefineType(tableHeader *th, parseTree ast);
void populateGlobalRecordsHelperUtils2(tablePtr *tabPtr, parseTree ast);
void populateGlobalRecordsHelperUtils(tableHeader *th, parseTree ast);

void populateGlobalRecordsHelper(tableHeader *th, parseTree ast);
void populateGlobalIDsHelper(tableHeader *th, parseTree ast);

void populateGlobalRecords(parseTree ast, symbolTable st, tableHeader *th);
void populateGlobalIDs(parseTree ast, symbolTable st, tableHeader *th);

char* getDataType(parseTree ast);
void populateFuncParameters(symbolTable st, parseTree ast, int idx, paramDetail *param);
void populateStmtVarUtils2(symbolTable st, tableHeader *th, parseTree ast, int idx);
void populateStmtVarUtils(symbolTable st, tableHeader *th, parseTree ast, int idx);
void populateStmtVar(symbolTable st, tableHeader *th, parseTree ast, int idx);
void populateFuncUtils(symbolTable st, parseTree ast, char *funcName);
void populateFuncOther(symbolTable st, parseTree ast);
void populateFunc(symbolTable st, parseTree ast);

symbolTable populateSymbolTable(parseTree ast);

void printFuncTableVariables(record *tmp, tableHeader *th);
void printFuncTable(tableHeader *th);
void printSymbolTable(symbolTable st);

void printGlobalVariables(symbolTable st);
void printFuncNameAndSizes(symbolTable st);
void printRecordsTypesAndSizes(symbolTable st);