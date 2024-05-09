/*
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#ifndef LEXER
#define LEXER

#include "lexerDef.h"

//Function Declarations of lookuptable
LookUpTable* createLookUpTable(int num_pos);
void insertNodeElement(LookUpTable *lookUpTable, char *key, terminal tokentype);
int hashFuncLUT(char *key, int num_pos);
void fillLookUpTable(LookUpTable *lookUpTable);
terminal getTokenType(char *key, LookUpTable *lookUpTable);

//Function Declarations of lexer
void removeComments(char *testcaseFile, char *cleanFile);
FILE *getStream(FILE *fp, char *c);
void printToken(tokenInfo *t);
void printTokenizedCode(FILE *fp);
void getNextToken(FILE *fp, tokenInfo *t);

#endif