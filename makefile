edit: driver.o utils.o lexer.o Stack.o parser.o AST.o symbolTable.o typechecker.o semanticAnalyzer.o
	gcc -o driver driver.o utils.o lexer.o Stack.o parser.o AST.o symbolTable.o typechecker.o semanticAnalyzer.o

utils.o: utils.c utils.h
	gcc -c utils.c

lexer.o: lexer.c lexer.h lexerDef.h
	gcc -c lexer.c

Stack.o: Stack.c Stack.h
	gcc -c Stack.c

parser.o: parser.c parser.h parserDef.h
	gcc -c parser.c

AST.o: AST.c AST.h
	gcc -c AST.c

symbolTable.o: symbolTable.c symbolTable.h
	gcc -c symbolTable.c

typechecker.o: typechecker.c typechecker.h
	gcc -c typechecker.c

semanticAnalyzer.o: semanticAnalyzer.c semanticAnalyzer.h
	gcc -c semanticAnalyzer.c

driver.o: driver.c 
	gcc -c driver.c

clean:
	rm driver driver.o utils.o lexer.o Stack.o parser.o AST.o symbolTable.o typechecker.o semanticAnalyzer.o