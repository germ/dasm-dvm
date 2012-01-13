#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE       unsigned char

/* Program states */
#define RUNNING    1
#define STOPPED    0

/* opcodes */
#define LOAD       0x00
#define STOR       0x01
#define VAL        0x02
#define JMP        0x03
#define JMP0       0x04
#define ADD        0x05
#define SUB        0x06
#define HALT       0x07

/* Assembler special defs */
#define NOP        0x08
#define DATA       0x09

/* Parsing functions, should be macros */
BYTE     get_inst(BYTE c);
BYTE     get_reg (BYTE c);
BYTE     get_loc (BYTE c);
BYTE     get_bit (BYTE c, BYTE b);

/* Debug functions */
void     error(char* msg);
void     asm_error(int line);

#endif /* SHARED_H */
