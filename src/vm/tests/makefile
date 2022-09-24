SHELL=/bin/bash

CC=g++
CFLAGS=-Wall -g

build: vm_tests.cpp ../script.cpp ../loader.cpp
	$(CC) $(CFLAGS) -o vm_tests vm_tests.cpp ../script.cpp ../loader.cpp

clean:
	rm vm_tests

