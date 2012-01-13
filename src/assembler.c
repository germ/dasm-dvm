#include "shared.c"
#include "assembler.h"

CURRENT_LINE = 0;

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
  char   *tok;
  
  /* Open streams, *should* use UNIX flag style parsing */
  if (argc < 3) error("No input files");
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

    tok = strtok(line, " ,");
    while (tok != NULL) {
      printf("%s\n", tok);
      strtok(NULL, " ,");
    }
    while (1) {
      char cmd;
      
      if      (tok == NULL)
      else if (strcmp(tok, "load")  == 0) {}
      else if (strcmp(tok, "store") == 0) {}
      else if (strcmp(tok, "value") == 0) {}
      else if (strcmp(tok, "jump")  == 0) {}
      else if (strcmp(tok, "jump0") == 0) {}
      else if (strcmp(tok, "add")   == 0) {}
      else if (strcmp(tok, "sub")   == 0) {
        unsigned int storage, left, right;

        strtok(NULL, " ,");
        if (tok == NULL) asm_error(CURRENT_LINE);
        storage = sscanf(tok, "r%d", &storage);

        strtok(NULL, " ,");
        if (tok == NULL) asm_error(CURRENT_LINE);
        storage = sscanf(tok, "r%d", &left);
        
        strtok(NULL, " ,");
        if (tok == NULL) asm_error(CURRENT_LINE);
        storage = sscanf(tok, "r%d", &right);

        if (storage > 1 || left > 1 || right > 1)
          asm_error(CURRENT_LINE);

        cmd = 0xC0;
        if (storage == 1) cmd = cmd & 0x04;
        if (left    == 1) cmd = cmd & 0x02;
        if (right   == 1) cmd = cmd & 0x01;
      }
      else if (strcmp(tok, "halt")  == 0) {
        cmd = 0xE0;
      }

      fputc(output, cmd);
      CURRENT_LINE++;
      break;
    }
  }
}
