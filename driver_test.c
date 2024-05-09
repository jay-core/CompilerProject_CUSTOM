#include "symbolTable1.h"
// #include "symbolTable.h"
#include "typechecker.h"
#include "semanticAnalyzer1.h"

int main(int argc, char *argv[]) 
{
    Grammar g;
    g = malloc(sizeof(lhsChar)*NO_OF_NONTERMINALS);
    getGram("grammar.txt", g);
    // printGram(g);

    /*========================================================================*/

    for(int i=0; i<NO_OF_NONTERMINALS; i++) computeFirst(g, i);
    fprintf(stderr, "First Computed!!\n");
    // printFirst(g);

    /*========================================================================*/

    g[program].follow |= (1LL<<dollar);
    for(int nt=0; nt<NO_OF_NONTERMINALS; nt++) {
        // if(g[nt].isNullable) {
            computeFollow(g, nt);
        // }
    }
    fprintf(stderr, "Follow Computed!!\n");

    // printFollow(g);

    /*========================================================================*/

    int t[NO_OF_NONTERMINALS][NO_OF_TERMINALS];
    createParseTable(g, t);

    // printParseTable(t);

    /*========================================================================*/

    char *srcFilePath = "./sample_code.txt";
    // char *srcFilePath = "./testcases_stage2_semantic_analysis/s1.txt";
    // char *srcFilePath = "./testcases_stage2_code_generation/c8.txt";
    FILE *sourceFile;

    /*========================================================================*/

    //TODO: this fucking updates lineNum which is stored in lexer.c (make it modular, which can be updated manually if needed)

    // sourceFile = fopen(srcFilePath, "r");
    // fseek(sourceFile, 0, SEEK_SET);
    // if(sourceFile==NULL) {
    //     fprintf(stderr, "error: %s: cannot open file\n", srcFilePath);
    //     return 0;
    // }
    // printTokenizedCode(sourceFile);

    /*========================================================================*/

    clock_t start_time, end_time;
    double total_CPU_time = 0, total_CPU_time_in_seconds = 0;

    int count = 0, countAST = 0;

    /*========================================================================*/

    sourceFile = fopen(srcFilePath, "r");
    fseek(sourceFile, 0, SEEK_SET);
    if(sourceFile==NULL) {
        fprintf(stderr, "error: %s: cannot open file\n", srcFilePath);
        return 0;
    }

    int error=0;
    parseTree root = NULL;

    start_time = clock();
    parseInputSourceCode(sourceFile, t, g, &root, &error);
    fclose(sourceFile);
    end_time = clock();

    printf("Error: %d\n", error);
    
    total_CPU_time += ((double)end_time - start_time);
    total_CPU_time_in_seconds += ((double)total_CPU_time/CLOCKS_PER_SEC);
    nodeCount(root, &count);
    count++;

    // printf("%lf Clocks | %lf sec | Nodes: %d", total_CPU_time, total_CPU_time_in_seconds, count);

    // printf("\n==============Parser Tree Details=====================\n");
    // printParserDetails(root);
    // printf("\n======================================================\n");

    // printf("\n===============Parse Tree=============================\n");
    // printParseTree(root, program);
    // printf("\n======================================================\n");

    /*========================================================================*/

    start_time = clock();
    parseTree ast = createASTDummy(root);
    end_time = clock();

    total_CPU_time += ((double)end_time - start_time);
    total_CPU_time_in_seconds += ((double)total_CPU_time/CLOCKS_PER_SEC);
    nodeCountAST(ast, &countAST);
    countAST++;

    // printf("%lf Clocks | %lf sec | Nodes: %d", total_CPU_time, total_CPU_time_in_seconds, countAST);

    printf("\n=================AST Tree Details=====================\n");
    printASTDetails(ast);
    printf("\n======================================================\n");

    printf("\n=================AST Tree=============================\n");
    printAST(ast, program);
    printf("\n======================================================\n");
    
    // printf("\n\nParse Tree number of nodes= %d\tAllocated Memory = %ld Bytes\n", count, count*sizeof(parsetree));
    // printf("\nAST number of nodes= %d\tAllocated Memory = %ld Bytes\n", countAST, countAST*sizeof(parsetree));	
    // double compressionPerc = (((double)count-countAST)/count)*100;
    // printf("Compression Percentage = %lf\n\n\n",compressionPerc);

    /*========================================================================*/

    start_time = clock();
    // symbolTable st = fillSymbolTable(ast);
    symbolTable st = populateSymbolTable(ast);
    printf("Symbol Table Filled Successfully!!\n");
    end_time = clock();

    total_CPU_time += ((double)end_time - start_time);
    total_CPU_time_in_seconds += ((double)total_CPU_time/CLOCKS_PER_SEC);

    /*========================================================================*/

    printSymbolTable(st);

    /*========================================================================*/

    printGlobalVariables(st);

    /*========================================================================*/

    printFuncNameAndSizes(st);

    /*========================================================================*/

    printRecordsTypesAndSizes(st);

    /*========================================================================*/

    // TypeChecker(*ast, st);

    /*========================================================================*/

    // checkSemanticsOfFunction(ast, st);

    return 0;
}