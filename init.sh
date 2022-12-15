#!/usr/bin/env bash

cd src/assembler
make
make clean
chmod 744 assembler
cd ../vm
make
chmod 744 cs4974
chmod 744 vm
cd ../compiler
make
make clean
chmod 744 compiler
cd ..

export PATH=$PATH:$(pwd)/assembler:$(pwd)/vm:$(pwd)/compiler
cd ..
