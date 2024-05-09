#include "parser.h"

void nodeCountAST(parseTree root,int* ans);
int useful(int node);
void copy(parseTree dst,parseTree src);
// void createASTUtils(parseTree curr, parseTree par);
// void buildAST(parseTree ast,parseTree root);
parseTree createASTDummy(parseTree root);
parseTree createAST(parseTree root);
void printAST(parseTree root, nonTerminal parent);
void printASTDetails(parseTree ast);