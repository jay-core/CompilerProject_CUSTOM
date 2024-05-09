/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#include <limits.h>
#include "parser.h"

typedef enum{
    integer,
    real,
    rec,
    uniontype,
    boolean,
    error
} types;

int TypeChecker(parsetree root, symbolTable s);
struct tableHeader* tp;