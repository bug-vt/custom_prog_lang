SHELL=/bin/bash

build: lex.yy.c asm_parser.tab.c asm_parser.tab.h
	g++ asm_parser.tab.c lex.yy.c -lfl -o asm_parser 
	g++ asm_tests.cpp ../asm_lexer.cpp ../instr_lookup.cpp ../error.cpp -o asm_tests

lex.yy.c: asm_lexer.l asm_parser.tab.h
	flex asm_lexer.l

asm_parser.tab.c asm_parser.tab.h: asm_parser.y
	bison -d asm_parser.y

clean:
	rm asm_parser lex.yy.c asm_parser.tab.* asm_tests
