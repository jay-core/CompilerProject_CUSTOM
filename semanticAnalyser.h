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

struct linklist{
	parseTree this;
	struct linklist* next;
};
typedef struct linklist* LinkedList;

LinkedList add_1(LinkedList l, parseTree t);
parseTree* findChildren(parseTree root, int* size);
LinkedList findTerminalsBoolean(parseTree root, LinkedList l);
int WhileSemantics(parseTree t);
int checkFuncCallStmts(parseTree stmt, parseTree* funList, symbolTable s, int* assigned, parseTree output_par, int i, int size);
int checkFunctionSemantics(parseTree root, symbolTable s);