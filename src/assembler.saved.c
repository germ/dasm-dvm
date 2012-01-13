#include "shared.c"
#include "assembler.h"

int CURRENT_LINE = 0;

char     parse_line(char** string) {
  char*  line;
  char*  tok;
  char   cmd;

  cmd    = 0;
  line   = *string;
  tok    = strtok(line, " ,");

  /* Dennis Ritchie just turned in his grave */
  if      (tok == NULL) asm_error(CURRENT_LINE);
  else if (strcmp(tok, "load")  == 0) {
    unsigned int reg, val;

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "r%d", &reg);

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "0x%X", &val);

    cmd = 0x00 | val;
    if (reg) cmd |= 0x10;
  }
  else if (strcmp(tok, "store") == 0) {
    unsigned int reg, val;

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "r%d", &reg);

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "0x%X", &val);

    cmd = 0x20 | val;
    if (reg) cmd |= 0x10;
  }
  else if (strcmp(tok, "value") == 0) {
    unsigned int reg, val;

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "r%d", &reg);

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "0x%X", &val);

    cmd = 0x40 & val;
    if (reg) cmd |= 0x10;
  }
  else if (strcmp(tok, "jump")  == 0) {
    int val;

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "0x%X", &val);

    cmd = 0x60 | val;
  }
  else if (strcmp(tok, "jump0") == 0) {
    unsigned int reg, val;

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "r%d", &reg);

    tok = strtok(NULL, " ,");
    if (tok == NULL) asm_error(CURRENT_LINE);
    sscanf(tok, "0x%X", &val);

    cmd = 0x80 | val;
    if (reg) cmd |= 0x10;
  }
  else if (strcmp(tok, "add")   == 0) {
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

    cmd = 0xA0;
    if (storage == 1) cmd |= 0x04;
    if (left    == 1) cmd |= 0x02;
    if (right   == 1) cmd |= 0x01;
  }
  else if (strcmp(tok, "sub")   == 0) {
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

    cmd = 0xC0;
    if (storage == 1) cmd |= 0x04;
    if (left    == 1) cmd |= 0x02;
    if (right   == 1) cmd |= 0x01;
  }
  else if (strcmp(tok, "halt")  == 0) {
    cmd = 0xE0;
  } 
  else asm_error(CURRENT_LINE);
  printf("%.2X\n", cmd);
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
  
  /* Open streams, *should* use UNIX flag style parsing */
  if (argc < 3) error("Too Few Arguments\nUsage: ./dvm source.asm outfile.o");
  input  = fopen(argv[1], "r");
  output = fopen(argv[2], "w");
  
  if (input == NULL || output == NULL)
    error("File Error");

  /* Main logic */
  char  cmd;
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
  }
  
  fclose(output);
  fclose(input);
  return 0;
}
