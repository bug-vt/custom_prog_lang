SHELL=/bin/bash

CC=g++
CFLAGS=-Wall -g

build: compiler.cpp \
			 parser.o func_table.o symbol_table.o string_table.o code_gen.o \
			 lexer.o preprocess.o token.o icode.o
	$(CC) $(CFLAGS) -o compiler compiler.cpp \
			 parser.o func_table.o symbol_table.o string_table.o code_gen.o \
			 lexer.o preprocess.o token.o icode.o

parser.o: parser.cpp lexer.o func_table.cpp symbol_table.cpp string_table.cpp \
					code_gen.cpp token.cpp icode.cpp
	$(CC) $(CFLAGS) -c parser.cpp func_table.cpp symbol_table.cpp string_table.cpp \
				code_gen.cpp token.cpp icode.cpp

lexer.o: lexer.cpp preprocess.cpp
	$(CC) $(CFLAGS) -c lexer.cpp preprocess.cpp 

clean:
	rm compiler *.o

