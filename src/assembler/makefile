SHELL=/bin/bash

CC=g++
CFLAGS=-Wall -g

build: assembler.cpp \
			 asm_parser.o func_table.o symbol_table.o label_table.o string_table.o token.o \
			 asm_lexer.o instr_lookup.o error.o \
			 code_gen.o
	$(CC) $(CFLAGS) -o assembler assembler.cpp \
			 asm_parser.o func_table.o symbol_table.o label_table.o string_table.o token.o \
			 asm_lexer.o instr_lookup.o error.o \
			 code_gen.o

asm_parser.o: asm_parser.cpp asm_lexer.cpp func_table.cpp symbol_table.cpp label_table.cpp string_table.cpp token.cpp code_gen.cpp
	$(CC) $(CFLAGS) -c asm_parser.cpp asm_lexer.cpp func_table.cpp symbol_table.cpp label_table.cpp string_table.cpp token.cpp code_gen.cpp

asm_lexer.o: asm_lexer.cpp instr_lookup.cpp error.cpp
	$(CC) $(CFLAGS) -c lexer_tests.cpp asm_lexer.cpp instr_lookup.cpp error.cpp

clean:
	rm *.o *.executable

clean2:
	rm assembler

