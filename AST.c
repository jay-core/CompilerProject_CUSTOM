/* 
 * Group 05
 * Kush Mehta			2018B5A70956P
 * Purusharth Amrut		2018B5A70897P
 * Patel Darsh Rajesh		2018B4A70532P
 * Harsh Jhunjhunwala		2018B5A70691P
 * Jatin Aggarwal		2018B4A70884P
 */

#include "AST.h"

void nodeCountAST(parseTree root, int* ans) {
	if(!root) return;

	// *ans = *ans + root->numChild;
	*ans = *ans + root->numChild;

	// for(int i=0; i<root->numChild; i++) nodeCountAST(&(root->children[i]), ans);
	for(int i=0; i<root->numChild; i++) nodeCountAST(&(root->children[i]), ans);
}

int useful(int tokenClass) {
    switch(tokenClass) {
        case TK_MAIN:
        case TK_END:
        case TK_SEM:
        case TK_INPUT:
        case TK_PARAMETER:
        case TK_LIST:
        case TK_SQL:
        case TK_SQR:
        case TK_OUTPUT:
        case TK_COMMA:
        case TK_COLON:
	    case TK_TYPE:
        case TK_DOT:
        case TK_CALL:
        case TK_WITH:
        case TK_PARAMETERS:
        case TK_ASSIGNOP:
        case TK_WHILE:
        case TK_ENDWHILE:
        case TK_IF:
	    case TK_ELSE:
        case TK_ENDIF:
	    case TK_READ:
	    case TK_WRITE:
        case TK_OP:
        case TK_CL:
        case TK_RETURN:
        case TK_ENDRECORD:
        case eps:
            return 0;
            break;
        default:
            return 1;
    }
}

void copy(parseTree dst, parseTree src)
{
	dst->terminal = src->terminal;
	dst->nt = src->nt;
	dst->numChild = src->numChild;
	dst->children = src->children;

    // not a correct copy function as we are moving the pointers to terminal
    // and children nodes, not exactly copying but moving the pointers
    src->terminal = NULL;
    src->children = (void*)NULL;

	// dst->ruleNo = src->ruleNo;
	// dst->numChild = src->numChild;
	// dst->tp = NULL;
}

// void createASTUtils(parseTree curr, parseTree par)
// {
// 	if(curr==NULL)
// 	return ; 
// 	if(curr -> numChild == 0)
// 	{
// 		if(!useful(curr->terminal->tokenType))
// 		par->numChild--;
// 		return;
// 	}
// 	if(curr->numChild == 1 && curr->children[0].numChild == 0)
// 	{
// 		if(useful(curr->children[0].terminal->tokenType))
// 		{
// 			copy(curr,&(curr->children[0]));
// 			return;
// 		}
// 	}
	
// 	int count = curr->numChild;
// 	for(int i = 0; i < count; i++)
// 	createASTUtils(&(curr->children[i]),curr);
	
// 	if(curr->numChild == 0)
// 		par->numChild--;
// 	if(curr->numChild == 1 && curr->children[0].numChild == 0)
// 	{
// 		for(int i = 0; i < curr->numChild ;i++)
// 		{
// 			if(curr->children[i].numChild==0 && curr->children[i].nt==-1)
// 			{
// 				if(useful(curr->children[i].terminal->tokenType))
// 					copy(curr,&(curr->children[i]));
// 			}
// 		}
// 	}
// }

// void buildAST(parseTree ast,parseTree root)
// {
// 	if(!root)
// 	return;
// 	ast->children = malloc((root->numChild)*sizeof(parsetree));
// 	int m=0;
// 	for(int i = 0;i < root->numChild;i++) 
// 	{
// 		if(root->children[i].numChild!=0)
// 		{
// 			copy(&(ast->children[m]),&(root->children[i]));
// 			buildAST(&(ast->children[m]),&(root->children[i]));
// 			m++;
// 		}
// 		else
// 		{
// 			if(root->children[i].nt==-1 && useful(root->children[i].terminal->tokenType))
// 			{
// 				copy(&(ast->children[m]),&(root->children[i]));
// 				m++;
// 			}
// 		}
// 	}
// }

parseTree createASTDummy(parseTree root){
	if(root==NULL) return NULL;
	
	if(root->numChild > 1){
        // for(int i=0; i<root->numChild; i++) {
        //     if(root->children[i].nt!=-1) root->children[i] = *createASTDummy(&root->children[i]);
        // }

        int newChildArrSize = 0;

        for(int i=0; i<root->numChild; i++) {
            if(root->children[i].nt!=-1) {
                newChildArrSize++;
                root->children[i] = *createASTDummy(&(root->children[i]));
            }else if(useful(root->children[i].terminal->tokenType)) newChildArrSize++;
        }

        // fprintf(stderr, "%s-> before: %d | after: %d\n", getNonTermString(root->nt), root->numChild, newChildArrSize);

        parseTree newChildArr = (parseTree)malloc(newChildArrSize*sizeof(parsetree));

        int idx = 0;
        for(int i=0; i<root->numChild; i++) {
            if(root->children[i].nt!=-1 || useful(root->children[i].terminal->tokenType)) {
                copyparsetree(&newChildArr[idx++], &root->children[i]);
            }
        }

        free(root->children);

        root->numChild = newChildArrSize;
        
        root->children = newChildArr;
        newChildArr = NULL;

        // root->children = (parseTree)malloc(root->numChild*sizeof(parsetree));
        
        // for(int i=0; i<root->numChild; i++) copyparsetree(&root->children[i], &newChildArr[i]);

        // free(newChildArr);

        return root;
	} else if(root->numChild == 1 && root->children[0].nt!=-1){
        // root has only one child, which is a non terminal
		//change root to child of root and delete root
        parseTree oldChild = &root->children[0];
        // parseTree *oldChild = &root->children;
		copy(root, &(root->children[0]));
        free(oldChild);
		// return createASTDummy(root->children);
		return createASTDummy(root);
	} else if(root->numChild == 1 && root->children[0].nt==-1){
        // root has only one child, which is a terminal
		if(useful(root->children[0].terminal->tokenType)){
			//change pointer to terminal
            parseTree oldChild = &root->children[0];
            // parseTree *oldChild = &root->children;
			copy(root, &(root->children[0]));
			// free(root->children);
            free(oldChild);
			return root;
		} else{
			root->numChild--;
			free(root->children);
			return root;
		}
	} else {
        // either a useful terminal or a non terminal whose propagated child was a useless terminal
        return root;
    }
	
	return root;
}
	
parseTree createAST(parseTree root)
{
    // isn't this just a redundant function
	// createASTUtils(root,NULL);
	// parseTree ast = malloc(sizeof(parsetree));
	// copy(ast, root);
	// buildAST(ast,root);
	// return ast;
	return createASTDummy(root);
}

void printAST(parseTree ast, nonTerminal parent)
{
    // if the format is same as printParseTree, then its a redundant function
    if(ast==NULL) return;

    if(ast->nt==-1) {
        // terminal
        printf("Lexeme: %s, LineNo: %lld, TokenName: %s, ValueOfNumber: %s, parentNodeSymbol: %s, isLeafNode: YES, NodeSymbol: %s\n",
        ast->terminal->lexeme, ast->terminal->lineNum, ast->terminal->lexeme, 
        ( (ast->terminal->tokenType==TK_INT || ast->terminal->tokenType==TK_REAL) ? ast->terminal->lexeme : "----" ), 
        getNonTermString(parent), getTermString(ast->terminal->tokenType));
        return;
    }

    if(ast->numChild==0) {
        printf("Lexeme: ----, LineNo: ----, TokenName: %s, ValueOfNumber: ----, parentNodeSymbol: %s, isLeafNode: NO, NodeSymbol: %s\n",
        getNonTermString(ast->nt), ( (ast->nt==program) ? "ROOT" : getNonTermString(parent) ), getNonTermString(ast->nt));
        return;
    }
    
    for(int i=0; i<ast->numChild; i++) {
        printAST(&ast->children[i], ast->nt);
        if(!i) {
            printf("Lexeme: ----, LineNo: ----, TokenName: %s, ValueOfNumber: ----, parentNodeSymbol: %s, isLeafNode: NO, NodeSymbol: %s\n",
            getNonTermString(ast->nt), ( (ast->nt==program) ? "ROOT" : getNonTermString(parent) ), getNonTermString(ast->nt));
        }
    }

}

void printASTDetails(parseTree ast) {
    if(ast==NULL) return;

    if(ast->nt==-1) printf("%s ", getTermString(ast->terminal->tokenType));
    else {
        printf("%s -> ", getNonTermString(ast->nt));
        for(int i=0; i<ast->numChild; i++) printASTDetails(&(ast->children[i]));
        printf("\n");
    }
}
