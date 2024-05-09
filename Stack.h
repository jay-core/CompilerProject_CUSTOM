/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#include "parserDef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct key{
	symbol id;
	char *lexeme;
	unsigned int tag : 1;
	parseTree subtree;
}key;
typedef struct key* Key;

struct element{
	Key k;
	struct element* next;
};
typedef struct element* Element;

struct stack{
	int count;
	Element head;
	Element tail;
};
typedef struct stack* Stack;


Stack newStack();

void insert_at_front(Stack s, Element e);
void insert_at_end(Stack s, Element e);

void delete_at_front(Stack s);

void push(Stack s, Key k);
void pop(Stack s);
Key top1(Stack s);
Element newElement(Key k);
Key newKey(symbol id, char* lexeme, unsigned int tag, parseTree pt);
void printKey(Key k);