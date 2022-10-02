SHELL=/bin/bash

CC=g++
CFLAGS=-Wall -g

build: interface_tests.cpp ../script.cpp ../loader.cpp ../stack.cpp
	$(CC) $(CFLAGS) -o interface_tests interface_tests.cpp ../script.cpp ../loader.cpp ../stack.cpp

clean:
	rm interface_tests 

