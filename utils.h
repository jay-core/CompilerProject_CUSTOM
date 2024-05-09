/* 
 * Group 05
 * Kush Mehta			    2018B5A70956P
 * Purusharth Amrut		    2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		    2018B4A70884P
 */

/*
 * utils.h - definition of helper function
 * 
 */

#ifndef UTILS
#define UTILS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define NONTERMINAL_OFFSET 12345
#define NO_OF_TERMINALS 61 //excluding TK_ERROR and dollar
// #define NO_OF_TERMINALS 61 //why not include them and make it 61 (won't not including them affect follow, since we need dollar)
#define NO_OF_NONTERMINALS 53
#define MAX_RULE_LEN 10
#define NO_OF_PRODUCTIONS 90
#define MAX_ID_SIZE 30
#define RECORD_DATATYPE 50000
#define MAX_LEXEME_SIZE 32
#define MAX_TOKEN_LENGTH 30
#define MAX_BUFFER_SIZE 4096 // max buffer size --Standard Size
#define PERMITED_LEXEME_SIZE 30
#define PERMITED_ID_SIZE 20

typedef enum {
    TK_ASSIGNOP, 
    TK_COMMENT, 
    TK_FIELDID, 
    TK_ID, 
    TK_NUM, 
    TK_RNUM, 
    TK_FUNID, 
    TK_RUID, 
    TK_WITH, 
    TK_PARAMETERS, 
    TK_END, 
    TK_WHILE, 
    TK_INT, 
    TK_REAL, 
    TK_TYPE, 
    TK_MAIN, 
    TK_GLOBAL, 
    TK_PARAMETER, 
    TK_LIST, 
    TK_SQL, 
    TK_SQR, 
    TK_INPUT, 
    TK_OUTPUT,  
    TK_SEM, 
    TK_COLON, 
    TK_DOT,
    TK_COMMA,
    TK_ENDWHILE, 
    TK_OP, 
    TK_CL, 
    TK_IF, 
    TK_THEN, 
    TK_ENDIF, 
    TK_READ, 
    TK_WRITE, 
    TK_RETURN, 
    TK_PLUS, 
    TK_MINUS, 
    TK_MUL, 
    TK_DIV, 
    TK_CALL, 
    TK_RECORD, 
    TK_ENDRECORD, 
    TK_ELSE, 
    TK_AND, 
    TK_OR, 
    TK_NOT, 
    TK_LT,
    TK_LE,
    TK_EQ, 
    TK_GT, 
    TK_GE, 
    TK_NE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_EOF,
    eps, 
    dollar,
    TK_ERROR
} terminal;

typedef enum { 
    program,
    mainFunction,
    otherFunctions,
    function,
    input_par,
    output_par,
    parameter_list,
    dataType,
    primitiveDatatype,
    constructedDatatype,
    remaining_list,
    stmts,
    typeDefinitions,
    actualOrRedefined,
    typeDefinition,
    defineTypeStmt,
    A,
    fieldDefinitions,
    fieldDefinition,
    fieldType,
    moreFields,
    declarations,
    declaration,
    global_or_not,
    otherStmts,
    stmt,
    assignmentStmt,
    singleOrRecId,
    option_single_constructed,
    oneExpansion,
    moreExpansions,
    funCallStmt,
    outputParameters,
    inputParameters,
    iterativeStmt,
    conditionalStmt,
    elsePart,
    ioStmt,
    arithmeticExpression,
    expPrime,
    term,
    termPrime,
    factor,
    highPrecedenceOperators,
    lowPrecedenceOperators,
    booleanExpression,
    var,
    logicalOp,
    relationalOp,
    returnStmt,
    optionalReturn,
    idList,
    more_ids
}nonTerminal;

char* getTermString(terminal term);
char* getNonTermString(nonTerminal nonTerm);
terminal checkTerminal(char *tokenArr);
nonTerminal checkNonTerminal(char *tokenArr);

#endif
