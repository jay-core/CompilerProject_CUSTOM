/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */


#include "AST.h"
#include "parser.h"

// this is file is uselss, delete this file

// typedef struct paramInfo {
// 	bool isInputPar;
// 	bool isOutputPar;
// 	int parIndex;
// } paramInfo;

// typedef struct record {
// 	char* rname;
// 	char* type;
// 	int lineNo;
// 	struct record* next;
// } record;

// typedef struct TableLoc {
// 	char* type;
// 	char* varname;
// 	int size;
// 	int offset;
// 	// If this entry is a record type
// 	record* ptr;
// 	// If this entry is a function parameter
// 	paramInfo* param;

// 	struct TableLoc *next;
// } TableLoc;

// typedef struct hashtable {
// 	// The hashtable is implemented as a linked list of TableLoc entries
// 	TableLoc* entry;
// } hashtable;

// typedef struct tableHeader {
// 	TableLoc** localTable;
// 	char* fname;
// 	int numVar;
// 	int fSize;
// 	int numInpPar;
// 	int numOutPar;
// 	record* inParList;
// 	record* outParList;
// 	record* variables;
// } tableHeader;

// typedef struct symboltable {
// 	tableHeader** fTable;
// 	int numFunc;
// 	record* functions;
// } symboltable;

// typedef symboltable* symbolTable;