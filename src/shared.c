#include "shared.h"

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
void     asm_error(int line) {
  printf("Could not parse at line %d, Exiting\n", line);
  exit(1);
}
