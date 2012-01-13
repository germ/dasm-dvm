#ifndef ASSEMBLER_H
#define ASSEMBLER_H

/* String defines */
#define  MAX_LEN    200
#define  MAX_SEP    10

char*    fgetline(FILE* input, char term, int n);
char     parse_line(char** line);

#endif /* ASSEMBLER_H */
