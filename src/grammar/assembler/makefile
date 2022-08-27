SHELL=/bin/bash

CC=gcc
# CFLAGS=-O0 -std=c99 -Wall -W

build: lex.yy.c asm_parser.tab.c asm_parser.tab.h
	$(CC) $(CFLAGS) asm_parser.tab.c lex.yy.c -lfl -o asm_parser 

lex.yy.c: asm_lexer.l asm_parser.tab.h
	flex asm_lexer.l

asm_parser.tab.c asm_parser.tab.h: asm_parser.y
	bison -d asm_parser.y

clean:
	rm asm_parser lex.yy.c asm_parser.tab.*
