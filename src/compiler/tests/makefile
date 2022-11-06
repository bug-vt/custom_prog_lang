SHELL=/bin/bash

CC=g++
CFLAGS=-Wall -g

build: compiler_tests.cpp lexer_tests.o parser_tests.o \
			 parser.o func_table.o symbol_table.o string_table.o \
			 lexer.o preprocess.o token.o
	$(CC) $(CFLAGS) -o compiler_tests compiler_tests.cpp lexer_tests.o parser_tests.o \
			 parser.o func_table.o symbol_table.o string_table.o \
			 lexer.o preprocess.o token.o 

parser_tests.o: parser_tests.cpp parser.o
	$(CC) $(CFLAGS) -c parser_tests.cpp 

parser.o: ../parser.cpp lexer.o ../func_table.cpp ../symbol_table.cpp ../string_table.cpp \
					../token.cpp
	$(CC) $(CFLAGS) -c ../parser.cpp ../func_table.cpp ../symbol_table.cpp ../string_table.cpp \
				../token.cpp
	
lexer_tests.o: lexer_tests.cpp lexer.o 
	$(CC) $(CFLAGS) -c lexer_tests.cpp  

lexer.o: ../lexer.cpp ../preprocess.cpp
	$(CC) $(CFLAGS) -c ../lexer.cpp ../preprocess.cpp 

clean:
	rm compiler_tests *.o

