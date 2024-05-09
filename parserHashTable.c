/* 
 * Group 08
 * Shubham Lather		2016A7PS0006P
 * Devyash Parihar		2016A7PS0066P
 * Rahul Khandelwal		2016A7PS0128P
 * Aniruddha Karve		2016A7PS0042P
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parserHashTable.h"

parserTable* createParserHashTable(int num_pos)
{
	parserTable* lookUpTable = (parserTable*)malloc(sizeof(parserTable));
	lookUpTable->num_pos = num_pos;
	lookUpTable->positions = (parserList**)malloc(num_pos*sizeof(parserList*));
	for(int i=0;i<num_pos;i++){
		lookUpTable->positions[i] = (parserList*)malloc(sizeof(parserList));
		lookUpTable->positions[i]->head = NULL;
	}
	fillPHT(lookUpTable);
	return lookUpTable;
}

void insertElementPHT(parserTable* lookUpTable, char* key, terminal tokentype)
{
	int hashvalue =  hashFuncPHT(key,lookUpTable->num_pos);
	parserElement* entry = (parserElement*)malloc(sizeof(parserElement));
	entry->type = tokentype;
	entry->lexeme = (char*)malloc(sizeof(char)*(strlen(key)+1));
	strcpy(entry->lexeme,key);
	entry->next = lookUpTable->positions[hashvalue]->head;
	lookUpTable->positions[hashvalue]->head = entry;
}


int hashFuncPHT(char* key, int num_pos)
{
	int sum = 0;
	for(int i=0;i<strlen(key);i++){
		sum = sum*10 + (key[i] - '0');
		if(sum<0)
			sum = 0;
	}
	int hashvalue = sum % num_pos;
	return hashvalue;
}

//filling table with entries of element
void fillPHT(parserTable* lookUpTable)
{
	insertElementPHT( lookUpTable, "program", 12345);
	insertElementPHT( lookUpTable, "mainFunction", 12346);
	insertElementPHT( lookUpTable, "otherFunctions", 12347);
	insertElementPHT( lookUpTable, "function", 12348);
	insertElementPHT( lookUpTable, "input_par", 12349);
	insertElementPHT( lookUpTable, "output_par", 12350);
	insertElementPHT( lookUpTable, "parameter_list", 12351);
	insertElementPHT( lookUpTable, "dataType", 12352);
	insertElementPHT( lookUpTable, "primitiveDatatype", 12353);
	insertElementPHT( lookUpTable, "constructedDatatype", 12354);
	insertElementPHT( lookUpTable, "remaining_list", 12355);
	insertElementPHT( lookUpTable, "stmts", 12356);
	insertElementPHT( lookUpTable, "typeDefinitions", 12357);
	insertElementPHT( lookUpTable, "typeDefinition", 12358);
	insertElementPHT( lookUpTable, "fieldDefinitions", 12359);
	insertElementPHT( lookUpTable, "fieldDefinition", 12360);
	insertElementPHT( lookUpTable, "moreFields", 12361);
	insertElementPHT( lookUpTable, "declarations",12362);
	insertElementPHT( lookUpTable, "declaration", 12363);
	insertElementPHT( lookUpTable, "global_or_not", 12364);
	insertElementPHT( lookUpTable, "otherStmts", 12365);
	insertElementPHT( lookUpTable, "stmt", 12366);
	insertElementPHT( lookUpTable, "assignmentStmt", 12367);
	insertElementPHT( lookUpTable, "singleOrRecId", 12368);
	insertElementPHT( lookUpTable, "singleOrRecIdPrime", 12369);	
	insertElementPHT( lookUpTable, "funCallStmt", 12370);
	insertElementPHT( lookUpTable, "outputParameters", 12371);
	insertElementPHT( lookUpTable, "inputParameters", 12372);
	insertElementPHT( lookUpTable, "iterativeStmt",12373 );
	insertElementPHT( lookUpTable, "conditionalStmt", 12374);	
	insertElementPHT( lookUpTable, "elsePart", 12375);
	insertElementPHT( lookUpTable, "ioStmt", 12376);
	insertElementPHT( lookUpTable, "allVar", 12377);
	insertElementPHT( lookUpTable, "arithmeticExpression", 12378);
	insertElementPHT( lookUpTable, "expPrime", 12379);
	insertElementPHT( lookUpTable, "term", 12380);
	insertElementPHT( lookUpTable, "termPrime", 12381);
	insertElementPHT( lookUpTable, "factor", 12382);
	insertElementPHT( lookUpTable, "highPrecedenceOperators",12383);
	insertElementPHT( lookUpTable, "lowPrecedenceOperators", 12384);
	insertElementPHT( lookUpTable, "all", 12385);
	insertElementPHT( lookUpTable, "temp",12386);
	insertElementPHT( lookUpTable, "booleanExpression",12387);
	insertElementPHT( lookUpTable, "var",12388);
	insertElementPHT( lookUpTable, "logicalOp", 12389);
	insertElementPHT( lookUpTable, "relationalOp", 12390);
	insertElementPHT( lookUpTable, "returnStmt", 12391);
	insertElementPHT( lookUpTable, "optionalReturn", 12392);
	insertElementPHT( lookUpTable, "idList", 12393);
	insertElementPHT( lookUpTable, "more_ids", 12394);
	insertElementPHT( lookUpTable, "TK_ASSIGNOP", 0);
	insertElementPHT( lookUpTable, "TK_COMMENT", 1);
	insertElementPHT( lookUpTable, "TK_FIELDID", 2);
	insertElementPHT( lookUpTable, "TK_ID", 3);
	insertElementPHT( lookUpTable, "TK_NUM", 4);
	insertElementPHT( lookUpTable, "TK_RNUM", 5);
	insertElementPHT( lookUpTable, "TK_FUNID", 6);
	insertElementPHT( lookUpTable, "TK_RECORDID", 7);
	insertElementPHT( lookUpTable, "TK_WITH", 8);
	insertElementPHT( lookUpTable, "TK_PARAMETERS", 9);
	insertElementPHT( lookUpTable, "TK_END", 10);
	insertElementPHT( lookUpTable, "TK_WHILE", 11);
	insertElementPHT( lookUpTable, "TK_INT", 12);
	insertElementPHT( lookUpTable, "TK_REAL", 13);
	insertElementPHT( lookUpTable, "TK_TYPE", 14);
	insertElementPHT( lookUpTable, "TK_MAIN", 15);
	insertElementPHT( lookUpTable, "TK_GLOBAL", 16);
	insertElementPHT( lookUpTable, "TK_PARAMETER", 17);
	insertElementPHT( lookUpTable, "TK_LIST", 18);
	insertElementPHT( lookUpTable, "TK_SQL", 19);
	insertElementPHT( lookUpTable, "TK_SQR", 20);
	insertElementPHT( lookUpTable, "TK_INPUT", 21);
	insertElementPHT( lookUpTable, "TK_OUTPUT", 22);
	insertElementPHT( lookUpTable, "TK_SEM", 23);
	insertElementPHT( lookUpTable, "TK_COLON", 24);
	insertElementPHT( lookUpTable, "TK_DOT", 25);
	insertElementPHT( lookUpTable, "TK_COMMA", 26);
	insertElementPHT( lookUpTable, "TK_ENDWHILE", 27);
	insertElementPHT( lookUpTable, "TK_OP", 28);
	insertElementPHT( lookUpTable, "TK_CL", 29);
	insertElementPHT( lookUpTable, "TK_IF", 30);
	insertElementPHT( lookUpTable, "TK_THEN", 31);
	insertElementPHT( lookUpTable, "TK_ENDIF", 32);
	insertElementPHT( lookUpTable, "TK_READ", 33);
	insertElementPHT( lookUpTable, "TK_WRITE", 34);
	insertElementPHT( lookUpTable, "TK_RETURN", 35);
	insertElementPHT( lookUpTable, "TK_PLUS", 36);
	insertElementPHT( lookUpTable, "TK_MINUS", 37);
	insertElementPHT( lookUpTable, "TK_MUL", 38);
	insertElementPHT( lookUpTable, "TK_DIV", 39);
	insertElementPHT( lookUpTable, "TK_CALL", 40);
	insertElementPHT( lookUpTable, "TK_RECORD", 41);
	insertElementPHT( lookUpTable, "TK_ENDRECORD", 42);
	insertElementPHT( lookUpTable, "TK_ELSE", 43);
	insertElementPHT( lookUpTable, "TK_AND", 44);
	insertElementPHT( lookUpTable, "TK_OR", 45);
	insertElementPHT( lookUpTable, "TK_NOT", 46);
	insertElementPHT( lookUpTable, "TK_LT", 47);
	insertElementPHT( lookUpTable, "TK_LE", 48);
	insertElementPHT( lookUpTable, "TK_EQ", 49);
	insertElementPHT( lookUpTable, "TK_GT", 50);
	insertElementPHT( lookUpTable, "TK_GE", 51);
	insertElementPHT( lookUpTable, "TK_NE", 52);
	insertElementPHT( lookUpTable, "TK_EOF", 53);
	insertElementPHT( lookUpTable, "eps", 54);
	insertElementPHT( lookUpTable, "TK_ERROR",55);
}



terminal getTokenTypePHT(char* key, parserTable* lookUpTable)
{
	int hashvalue = hashFuncPHT(key, lookUpTable->num_pos);
	parserElement* temp = lookUpTable->positions[hashvalue]->head;
	while(temp!=NULL)
	{
		if(strcmp(temp->lexeme,key)==0)
			return temp->type;

		temp = temp->next;
	}
	return -1;
}



