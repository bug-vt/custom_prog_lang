SHELL=/bin/bash

CC=g++
CFLAGS=-Wall -g

build: vm.cpp script.cpp loader.cpp stack.cpp
	$(CC) $(CFLAGS) -o vm vm.cpp script.cpp loader.cpp stack.cpp

clean:
	rm vms

