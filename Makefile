CC		 = gcc
FLAGS  = -g -ansi -O0
SRC		= src

all: vm assembler

vm: $(SRC)/vm.c 
	$(CC) $(FLAGS) $(SRC)/vm.c -o dvm

assembler: $(SRC)/assembler.c $(SRC)/assembler.h
	$(CC) $(FLAGS) $(SRC)/assembler.c -o dasm

clean:
	rm -f dvm dasm
