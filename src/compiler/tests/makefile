SHELL=/bin/bash

CC=g++
CFLAGS=-Wall -g

build: compiler_tests.cpp lexer_tests.o parser_tests.o \
			 lexer.o preprocess.o token.o
	$(CC) $(CFLAGS) -o compiler_tests compiler_tests.cpp lexer_tests.o parser_tests.o \
			 lexer.o preprocess.o token.o 

parser_tests.o: parser_tests.cpp
	$(CC) $(CFLAGS) -c parser_tests.cpp 
	
lexer_tests.o: lexer_tests.cpp lexer.o ../token.cpp
	$(CC) $(CFLAGS) -c lexer_tests.cpp ../token.cpp 

lexer.o: ../lexer.cpp ../preprocess.cpp
	$(CC) $(CFLAGS) -c ../lexer.cpp ../preprocess.cpp 

clean:
	rm compiler_tests *.o

