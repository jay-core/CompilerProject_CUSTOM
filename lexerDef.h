/*
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#ifndef LEXERDEF
#define LEXERDEF

#include "utils.h"

//lookuptable structure
struct node{
	terminal type;
	char* lexeme;
	struct node* next;
};

typedef struct node NodeElement;

struct list{
	NodeElement* head;
};

typedef struct list List;

struct lookUp{
	int num_pos;
	List** positions;
};	

typedef struct lookUp LookUpTable;

typedef struct tokenInfo{
    char lexeme[MAX_LEXEME_SIZE];
    unsigned long long lineNum;
    terminal tokenType;
} tokenInfo;

#endif