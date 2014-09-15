stage1exe : driver.o grammarOps.o parser.o lexer.o symbol.o codeGen.o
	gcc -g -o toycompiler driver.o grammarOps.o parser.o lexer.o symbol.o codeGen.o
	
driver.o : driver.c
	gcc -g -Wall -c driver.c
	
lexer.o : lexer.c
	gcc -g -Wall -c lexer.c
	
grammarOps.o : grammarOps.c
	gcc -g -Wall -c grammarOps.c
	
parser.o : parser.c
	gcc -g -Wall -c parser.c
	
symbol.o : symbol.c
	gcc -g -Wall -c symbol.c
	
codeGen.o : codeGen.c
	gcc -g -Wall -c codeGen.c
	
clean : rm *.o
		rm toycompiler
