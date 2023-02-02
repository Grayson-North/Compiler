#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"

int main(int argc, char **argv)
{
  int lflag = 0, aflag = 0, vflag = 0;
  FILE *ifp;
  char *inputfile;
  char c;
  int i;

  if (argc < 2)
  {
    printf("Error : please include the file name");
    return 0;
  }
  else if (argc > 4)
  {
    lflag = 1;
    aflag = 1;
    vflag = 1;
  }
  else if (argc == 3 || argc == 4)
  {
    if (argv[2][1] == 'l')
      lflag = 1;
    else if (argv[2][1] == 'a')
      aflag = 1;
    else
      vflag = 1;
    if (argc == 4)
    {
      if (argv[3][1] == 'l')
        lflag = 1;
      else if (argv[3][1] == 'a')
        aflag = 1;
      else
        vflag = 1;
    }
  }

  ifp = fopen(argv[1], "r");
  inputfile = malloc(1000 * sizeof(char));
  c = fgetc(ifp);
  i = 0;

  while (1)
  {
    inputfile[i++] = c;
    c = fgetc(ifp);
    if (c == EOF)
      break;
  }
  fclose(ifp);
  inputfile[i] = '\0';

  lexeme *list = lex_analyze(inputfile, lflag);
  printf("\n\n");
  instruction *code = parse(list, aflag);
  execute(code, vflag);
  free(inputfile);
  free(code);
  for (int k = 0; k < 1000; k++)
    free(list[k].name);
  free(list);
  return 0;
}
