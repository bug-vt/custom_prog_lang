SHELL=/bin/bash

CC=g++
CFLAGS=-Wall -g

build: compiler.cpp \
			 parser.o func_table.o symbol_table.o \
			 lexer.o preprocess.o token.o 
	$(CC) $(CFLAGS) -o compiler compiler.cpp \
			 parser.o func_table.o symbol_table.o \
			 lexer.o preprocess.o token.o 

parser.o: parser.cpp lexer.o func_table.cpp symbol_table.cpp \
					token.cpp
	$(CC) $(CFLAGS) -c parser.cpp func_table.cpp symbol_table.cpp \
				token.cpp 

lexer.o: lexer.cpp preprocess.cpp
	$(CC) $(CFLAGS) -c lexer.cpp preprocess.cpp 

clean:
	rm *.o

