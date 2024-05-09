/* 
* Group 05
* Kush Mehta			2018B5A70956P
* Purusharth Amrut		2018B5A70897P
* Patel Darsh Rajesh	2018B4A70532P
* Harsh Jhunjhunwala	2018B5A70691P
* Jatin Aggarwal		2018B4A70884P
*/

#include <limits.h>
#include "parser.h"
#include "symbolTable1.h"

struct linkedList {
    parseTree this;
    struct linkedList *next;
};

typedef struct linkedList* LinkedList;

LinkedList addOneElem(LinkedList l, parseTree t);
LinkedList findBooleanTerminals(parseTree root, LinkedList l);

parseTree* getChildren(parseTree root, int *size);
int whileStmtSemantics(parseTree t);
int checkForFunctionCallStmts(parseTree stmt, parseTree *functionList, 
symbolTable s, int *assignedVal, parseTree tmpOutputPar, int i, int size);
int checkSemanticsOfFunction(parseTree root, symbolTable s);