#!/usr/bin/env bash

mkdir bin

cd src/assembler
make
make clean
chmod 744 assembler
mv assembler ../../bin

cd ../vm
make
chmod 744 cs4974
chmod 744 vm
mv vm ../../bin

cd ../compiler
make
make clean
chmod 744 compiler
mv compiler ../../bin
cd ..
cd ..

export PATH=$(pwd)/bin:$PATH
