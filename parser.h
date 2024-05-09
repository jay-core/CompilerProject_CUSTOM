/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#ifndef PARSER
#define PARSER

#include "parserDef.h"
#include "lexer.h"
#include "Stack.h"

void nodeCount(parseTree root,int* ans);
void printGram(Grammar g);
void getGram(char *fname, Grammar g);
void first(Grammar g, rhsChar rcn, long long int* firstBitString);
void computeFirst(Grammar g, nonTerminal nt);
void printTerminalList(long long bitString);
void printFirst(Grammar g);
void computeFollow(Grammar g, nonTerminal nt);
void printFollow(Grammar g);
void createParseTable(Grammar g, int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS]);
void printParseTable(int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS]);
void parseInputSourceCode(FILE* sourceFile, int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS], Grammar g, parseTree *root, int* error);
void copyparsetree(parseTree dst, parseTree src);
void printParseTree(parseTree root, nonTerminal parent);
void printParserDetails(parseTree root);


#endif
