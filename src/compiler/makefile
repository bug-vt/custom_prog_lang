SHELL=/bin/bash

CC=g++
CFLAGS=-Wall -g

build: compiler.cpp lexer.cpp preprocess.cpp token.cpp
	$(CC) $(CFLAGS) -o compiler compiler.cpp lexer.cpp preprocess.cpp token.cpp

clean:
	rm compiler

