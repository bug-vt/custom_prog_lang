SHELL=/bin/bash

CC=gcc
CFLAGS=-O0 -std=c99 -Wall -W

build: lex.yy.c
	$(CC) $(CFLAGS) -o asm_lexer lex.yy.c

lex.yy.c: asm_lexer.l
	flex asm_lexer.l

clean:
	rm asm_lexer lex.yy.c
