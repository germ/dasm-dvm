#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTE       unsigned char
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

/* Parsing functions, should be macros */
BYTE     get_inst(BYTE c) {
  return c >> 5;
}
BYTE     get_reg (BYTE c) {
  return c = (c >> 4) & 0x01;
}
BYTE     get_loc (BYTE c) {
  return c & 0x0F;
}
BYTE     get_bit (BYTE c, BYTE b) {
  return c & (1 << b);
}

/* Debug functions */
void     error(char* msg) {
  printf("ERROR: %s\n", msg);
  exit(1);
}
void     print_state(BYTE* ip, BYTE* ir, BYTE** reg, BYTE** mem) {
  int i;

  printf("Registers\n\tr0:%.2X\n\tr1:%.2X\n", (*reg)[0], (*reg)[1]);
  printf("Instruction\n\tCurrent:%X\n\tPointer:%X\n",*ir, *ip);
  printf("Memory\n\t");
  for (i = 0; i < 16; i++) {
    if (i == 8) printf("\n\t");
    printf("%.2X ", (*mem)[i]);
  }
  printf("\n");

  return;
}

int      main(int argc, char* argv[]) {
  BYTE   ip;
  BYTE   ir;
  BYTE*  mem;
  BYTE*  reg;

  FILE*  input;
  int    i;
  BYTE   c;
  BYTE   state;
  
  /* Reset state */
  ip     = 0;
  ir     = 0;
  input  = NULL;
  state  = RUNNING;
  mem    = calloc(16, sizeof(BYTE));
  reg    = calloc(2,  sizeof(BYTE));

  /* Check for input */
  if (argc < 2) {
    printf("No input.\n");
    return 1;
  }
  
  input = fopen(argv[1], "r");
  if (input == NULL) error("Could not open file");

  /* Load initial memory */
  for (i = 0; (i < 16) && ((c = fgetc(input)) != EOF); i++) 
    mem[i] = c;

  /* Run the program */
  while (state == RUNNING) {
    ir = mem[ip];
    switch(get_inst(ir)) {
      case LOAD:
        reg[get_reg(ir)] = mem[get_loc(ir)];
        break;
      case STOR:
        mem[get_loc(ir)] = reg[get_reg(ir)];
        break;
      case VAL:
        reg[get_reg(ir)] = get_loc(ir);
        break;
      case JMP:
        ip = get_loc(ir)-1;
        break;
      case JMP0:
        if (reg[get_reg(ir)] == 0) ip = get_loc(ir)-1;
        break;
      case ADD:
        reg[get_reg(ir)] = reg[get_bit(ir, 1)] + reg[get_bit(ir, 0)];
        break;
      case SUB:
        reg[get_reg(ir)] = reg[get_bit(ir, 1)] - reg[get_bit(ir, 0)];
        break;
      case HALT:
        print_state(&ip, &ir, &reg, &mem);
        state = STOPPED;
        break;
    }

    if (state == RUNNING) ip++;
  }
  
  free(mem);
  free(reg);

  return 0;
}
