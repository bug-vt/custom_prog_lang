SHELL=/bin/bash

CC=g++
CFLAGS=-Wall -g

build: asm_tests.cpp lexer_tests.o parser_tests.o code_gen_tests.o \
			 asm_parser.o func_table.o symbol_table.o label_table.o string_table.o token.o \
			 asm_lexer.o instr_lookup.o error.o \
			 code_gen.o
	$(CC) $(CFLAGS) -o asm_tests asm_tests.cpp lexer_tests.o parser_tests.o code_gen_tests.o \
			 asm_parser.o func_table.o symbol_table.o label_table.o string_table.o token.o \
			 asm_lexer.o instr_lookup.o error.o \
			 code_gen.o

code_gen_tests.o: code_gen_tests.cpp code_gen.o
	$(CC) $(CFLAGS) -c code_gen_tests.cpp 

parser_tests.o: parser_tests.cpp asm_parser.o
	$(CC) $(CFLAGS) -c parser_tests.cpp 

asm_parser.o: ../asm_parser.cpp ../asm_lexer.cpp ../func_table.cpp ../symbol_table.cpp ../label_table.cpp ../string_table.cpp ../token.cpp ../code_gen.cpp
	$(CC) $(CFLAGS) -c ../asm_parser.cpp ../asm_lexer.cpp ../func_table.cpp ../symbol_table.cpp ../label_table.cpp ../string_table.cpp ../token.cpp ../code_gen.cpp
	
lexer_tests.o: lexer_tests.cpp asm_lexer.o 
	$(CC) $(CFLAGS) -c lexer_tests.cpp 

asm_lexer.o: ../asm_lexer.cpp ../instr_lookup.cpp ../error.cpp
	$(CC) $(CFLAGS) -c lexer_tests.cpp ../asm_lexer.cpp ../instr_lookup.cpp ../error.cpp

clean:
	rm asm_tests *.o

# --------------------------------------------------------------------------
# flex and bison

bison: lex.yy.c asm_parser.tab.c asm_parser.tab.h
	g++ asm_parser.tab.c lex.yy.c -lfl -o asm_parser 

lex.yy.c: asm_lexer.l asm_parser.tab.h
	flex asm_lexer.l

asm_parser.tab.c asm_parser.tab.h: asm_parser.y
	bison -d asm_parser.y

clean_bison:
	rm asm_parser lex.yy.c asm_parser.tab.* 

