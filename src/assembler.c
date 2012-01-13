#include "shared.c"
#include "assembler.h"

int CURRENT_LINE;

char     parse_line(char** string) {
  char*  line; /* Raw text to process */
  char*  tok;  /* Current tokenized section */
  char   cmd;  /* 8-bit command to be returned */
  char   sv;   /* Switch Variable */

  /* Init */
  cmd    = 0;
  line   = *string;
  tok    = strtok(line, " ,");

  /* Find current instruction */
  if      (strcmp(tok, "load")  == 0) sv = LOAD;
  else if (strcmp(tok, "store") == 0) sv = STOR;
  else if (strcmp(tok, "value") == 0) sv = VAL;
  else if (strcmp(tok, "jump")  == 0) sv = JMP;
  else if (strcmp(tok, "jump0") == 0) sv = JMP0;
  else if (strcmp(tok, "add")   == 0) sv = ADD;
  else if (strcmp(tok, "sub")   == 0) sv = SUB;
  else if (strcmp(tok, "halt")  == 0) sv = HALT;
  else if (strcmp(tok, "nop")   == 0) sv = NOP;
  else if (strcmp(tok, "data")  == 0) sv = DATA;
  else                                sv = -1;

  /* Dennis Ritchie just turned in his grave */
  if      (tok == NULL) {
    asm_error(CURRENT_LINE);
  }
  else if (sv == LOAD || sv == STOR || sv == VAL || sv == JMP0) {
    /*  Generic data parsing */
    unsigned int reg, val;

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "r%d", &reg);

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "0x%X", &val);

    /* Specialized handling */
    if      (sv == LOAD) {
      cmd = 0x00 | val;
      if (reg) 
        cmd |= 0x10;
    } 
    else if (sv == STOR) {
      cmd = 0x20 | val;
      if (reg) 
        cmd |= 0x10;
    } 
    else if (sv == VAL) {
      cmd = 0x40 | val;
      if (reg) 
        cmd |= 0x10;
    } 
    else if (sv == JMP0) {
      cmd = 0x80 | val;
      if (reg) 
        cmd |= 0x10;
    }
  }
  else if (sv == ADD || sv == SUB) {
    unsigned int storage, left, right;

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "r%d", &storage);

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "r%d", &left);
    
    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "r%d", &right);

    if (storage > 1 || left > 1 || right > 1)
      asm_error(CURRENT_LINE);

    if (sv == ADD) cmd = 0xA0;
    if (sv == SUB) cmd = 0xC0;
    if (storage == 1) cmd |= 0x04;
    if (left    == 1) cmd |= 0x02;
    if (right   == 1) cmd |= 0x01;
  } 
  else if (sv == JMP || sv == DATA) {
    int val;

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "0x%X", &val);

    if (sv == JMP)  cmd = 0x60 | val;
    if (sv == DATA) cmd = val;
  } 
  else if (sv == HALT) {
    cmd = 0xE0;
  } 
  else if (sv == NOP) {
    cmd = 0x00;
  }
  else if (sv == -1) {
    asm_error(CURRENT_LINE);
  }
  
  tok == NULL;
  line == NULL;

  return cmd;
}
char*    fgetline(FILE* input, char term, int n) {
  int    i;
  char   c;
  char*  ret;

  ret = calloc(n, sizeof(char));
  memset(ret, 0, n);
  
  if (input == NULL) error("File Error");
  for (i = 0; (i < n) && ((c = fgetc(input)) != EOF) && (c != term); i++)
    ret[i] = c;

  if (i == 0 && c == EOF) {
    free(ret);
    return NULL;
  }

  return ret;
}

int main(int argc, char* argv[]) {
  FILE   *output,
         *input;
  char   *line;
  char  cmd;
  
  CURRENT_LINE = 0;

  /* Open streams, *should* use UNIX flag style parsing */
  if (argc < 3) error("Too Few Arguments\nUsage: ./dvm source.asm outfile.o");
  input  = fopen(argv[1], "r");
  output = fopen(argv[2], "w");
  
  if (input == NULL || output == NULL)
    error("File Error");

  /* Main logic */
  while ((line = fgetline(input, '\n', MAX_LEN)) != NULL) {
    /* Check for EOF/Errors */
    if (line == NULL) break;

    /* Check for blanks */
    if (strlen(line) == 0) {
      CURRENT_LINE++;
      continue;
    }

    cmd = parse_line(&line);
    CURRENT_LINE++;
    fputc(cmd, output);
    free(line);
  }
  
  /* cleanup */
  fclose(output);
  fclose(input);

  output = NULL;
  input  = NULL;
  line   = NULL;
  return 0;
}
