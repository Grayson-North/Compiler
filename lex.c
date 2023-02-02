// -----------------------------------------------------------------------------
// Grayson North
// gr967419
// COP 3402
// Spring 2021
// -----------------------------------------------------------------------------

// ================================= TO DO =====================================
// CREATE README AND SUBMIT TO WEBCOURSES
// =============================================================================

// ------------------- LIBRARIES, HEADERS AND DEFINITIONS ----------------------
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "compiler.h"

#define ID_ERR -1
#define NUM_ERR -2
#define SYM_ERR -3
#define DIG_ERR -4
// -----------------------------------------------------------------------------

// ------------------------------ FUNCTIONS ------------------------------------
lexeme* lex_analyze(char *inputfile, int print)
{
  // This program uses several arrays and different indexes. The inputfile array
  // is the inputfile that is read in the input file. tokens is the integer array
  // that stores the integer values of the tokens. List is an array of inputfiles
  // that is the final token list printed at the end of this program. Lex is
  // the array of inputfiles that contains all the lexemes. Buf is an array of
  // characters that will hold each lexeme as it is read from inputfile.
  int tokens[1000];
  char list[1000][12];
  char lex[1000][50];
  char buf[50];
  int c = 0;
  int t = 0;
  int accum = 0;
  int sp = 0;
  int l = 0;
  int m = 0;
  lexeme *lexeme_list = malloc(1000 * sizeof(lexeme));

  for (int i = 0; i < 1000; i++)
  {
    lexeme_list[i].name = malloc(12 * sizeof(char));
    for (int j = 0; j < 12; j++)
      lexeme_list[i].name[j] = '\0';
    lexeme_list[i].value = 0;
    lexeme_list[i].type = 0;
  }

  // filling inputfile with null sentinel characters and tokens with zeros
  for (c = 0; c < 1000; c++)
    tokens[c] = 0;
  c = 0;

  // filling buf with null sentinel characters
  for (int i = 0; i < 50; i++)
    buf[i] = '\0';

  // filling lex with inputfiles length of null sentinel characters
  for (int i = 0; i < 1000; i++)
    for (int j = 0; j < 50; j++)
      lex[i][j] = '\0';

  // filling list with inputfiles length of null sentinel characters
  for (int i = 0; i < 1000; i++)
    for (int j = 0; j < 12; j++)
      list[i][j] = '\0';

  // while no null sentinel is detected read "word" into buff and separate into
  // lexemes that will be stored into lex at the appropriate indexes.
  while (inputfile[c] != '\0')
  {
    // for whatever reason iscntrl() would not work with whitespace so i had to
    // add inputfile[c] != ' ', even though that it is redundant.
    while (!iscntrl(inputfile[c]) && !ispunct(inputfile[c]) && inputfile[c] != ' ')
    {
      buf[accum] = inputfile[c];
      accum++;
      c++;
    }
    if (buf[0] != '\0')
    {
      strcpy(lex[sp], buf);
      sp++;
    }
    if (ispunct(inputfile[c]))
    {
      lex[sp][0] = inputfile[c];
      if (inputfile[c + 1] == '=' || inputfile[c + 1] == '>')
      {
        lex[sp][1] = inputfile[c + 1];
        c++;
      }
      sp++;
    }
    c++;
    for (int i = 0; i < accum; i++)
      buf[i] = '\0';
    accum = 0;
  }

  // looking at each member in lex, we identify what that member is and store
  // its token value into tokens, or find if it is an error and store that
  // error value in tokens as well
  for (int i = 0; i < sp; i++)
  {
    if (isalpha(lex[i][0]))
    {
      while (lex[i][l] != '\0')
      {
        if (l > 11)
          break;
        l++;
      }
      if (l > 11)
      {
        tokens[i] = ID_ERR; // this is the Identifier too long error
        l = 0;
      }
      else if (strcmp(lex[i], "odd") == 0)
      {
        tokens[i] = oddsym;
        strcpy(lexeme_list[i].name, "odd");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = oddsym;
      }

      else if(strcmp(lex[i], "begin") == 0)
      {
        tokens[i] = beginsym;
        strcpy(lexeme_list[i].name, "begin");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = beginsym;
      }

      else if(strcmp(lex[i], "end") == 0)
      {
        tokens[i] = endsym;
        strcpy(lexeme_list[i].name, "end");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = endsym;
      }

      else if(strcmp(lex[i], "if") == 0)
      {
        tokens[i] = ifsym;
        strcpy(lexeme_list[i].name, "if");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = ifsym;
      }

      else if(strcmp(lex[i], "then") == 0)
      {
        tokens[i] = thensym;
        strcpy(lexeme_list[i].name, "then");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = thensym;
      }

      else if(strcmp(lex[i], "while") == 0)
      {
        tokens[i] = whilesym;
        strcpy(lexeme_list[i].name, "while");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = whilesym;
      }

      else if(strcmp(lex[i], "do") == 0)
      {
        tokens[i] = dosym;
        strcpy(lexeme_list[i].name, "do");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = dosym;
      }

      else if(strcmp(lex[i], "call") == 0)
      {
        tokens[i] = callsym;
        strcpy(lexeme_list[i].name, "call");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = callsym;
      }

      else if(strcmp(lex[i], "const") == 0)
      {
        tokens[i] = constsym;
        strcpy(lexeme_list[i].name, "const");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = constsym;
      }

      else if(strcmp(lex[i], "var") == 0)
      {
        tokens[i] = varsym;
        strcpy(lexeme_list[i].name, lex[i]);
        lexeme_list[i].value = 0;
        lexeme_list[i].type = varsym;
      }

      else if(strcmp(lex[i], "procedure") == 0)
      {
        tokens[i] = procsym;
        strcpy(lexeme_list[i].name, "procedure");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = procsym;
      }

      else if(strcmp(lex[i], "write") == 0)
      {
        tokens[i] = writesym;
        strcpy(lexeme_list[i].name, "write");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = writesym;
      }

      else if(strcmp(lex[i], "read") == 0)
      {
        tokens[i] = readsym;
        strcpy(lexeme_list[i].name, "read");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = readsym;
      }

      else if(strcmp(lex[i], "else") == 0)
      {
        tokens[i] = elsesym;
        strcpy(lexeme_list[i].name, "else");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = elsesym;
      }

      else if(strcmp(lex[i], "return") == 0)
      {
        tokens[i] = returnsym;
        strcpy(lexeme_list[i].name, "return");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = returnsym;
      }

      else
      {
        tokens[i] = identsym;
        strcpy(lexeme_list[i].name, lex[i]);
        lexeme_list[i].value = 0;
        lexeme_list[i].type = identsym;
      }
    }
    else if (isdigit(lex[i][0]))
    {
      if (isalpha(lex[i][1]))
        tokens[i] = DIG_ERR; // Identifier names cannot start with digits
      else
      {
        while (lex[i][m] != '\0')
        {
          if (m > 5)
            break;
          m++;
        }
        if (m > 5)
          tokens[i] = NUM_ERR; // digit too long
        else
        {
          tokens[i] = numbersym;
          strcpy(lexeme_list[i].name, lex[i]);
          lexeme_list[i].value = atoi(lex[i]);
          lexeme_list[i].type = numbersym;
        }
        m = 0;
      }
    }
    else
    {
      if (strcmp(lex[i], "%") == 0)
      {
        tokens[i] = modsym;
        strcpy(lexeme_list[i].name, "*");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = modsym;
      }

      else if (strcmp(lex[i], "+") == 0)
      {
        tokens[i] = plussym;
        strcpy(lexeme_list[i].name, "+");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = plussym;
      }

      else if (strcmp(lex[i], "-") == 0)
      {
        tokens[i] = minussym;
        strcpy(lexeme_list[i].name, "-");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = minussym;
      }

      else if (strcmp(lex[i], "*") == 0)
      {
        tokens[i] = multsym;
        strcpy(lexeme_list[i].name, "*");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = multsym;
      }

      else if (strcmp(lex[i], "/") == 0)
      {
        tokens[i] = slashsym;
        strcpy(lexeme_list[i].name, "/");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = slashsym;
      }

      else if (strcmp(lex[i], "=") == 0)
      {
        tokens[i] = eqlsym;
        strcpy(lexeme_list[i].name, "=");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = eqlsym;
      }

      else if (strcmp(lex[i], "<>") == 0)
      {
        tokens[i] = neqsym;
        strcpy(lexeme_list[i].name, "<>");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = neqsym;
      }

      else if (strcmp(lex[i], "<") == 0)
      {
        tokens[i] = lessym;
        strcpy(lexeme_list[i].name, "<");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = lessym;
      }

      else if (strcmp(lex[i], "<=") == 0)
      {
        tokens[i] = leqsym;
        strcpy(lexeme_list[i].name, "<=");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = leqsym;
      }

      else if (strcmp(lex[i], ">") == 0)
      {
        tokens[i] = gtrsym;
        strcpy(lexeme_list[i].name, ">");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = gtrsym;
      }

      else if (strcmp(lex[i], ">=") == 0)
      {
        tokens[i] = geqsym;
        strcpy(lexeme_list[i].name, ">=");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = geqsym;
      }

      else if (strcmp(lex[i], "(") == 0)
      {
        tokens[i] = lparentsym;
        strcpy(lexeme_list[i].name, "(");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = lparentsym;
      }

      else if (strcmp(lex[i], ")") == 0)
      {
        tokens[i] = rparentsym;
        strcpy(lexeme_list[i].name, ")");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = rparentsym;
      }

      else if (strcmp(lex[i], ",") == 0)
      {
        tokens[i] = commasym;
        strcpy(lexeme_list[i].name, ",");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = commasym;
      }

      else if (strcmp(lex[i], ";") == 0)
      {
        tokens[i] = semicolonsym;
        strcpy(lexeme_list[i].name, ";");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = semicolonsym;
      }

      else if (strcmp(lex[i], ".") == 0)
      {
        tokens[i] = periodsym;
        strcpy(lexeme_list[i].name, ".");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = periodsym;
      }

      else if (strcmp(lex[i], ":=") == 0)
      {
        tokens[i] = becomessym;
        strcpy(lexeme_list[i].name, ":=");
        lexeme_list[i].value = 0;
        lexeme_list[i].type = becomessym;
      }

      else
        tokens[i] = SYM_ERR; // unknown symbol detected
    }
  }

  if (print != 1)
    return lexeme_list;


  printf("Lexeme Table:\n");
  printf("lexeme          token type\n");

  // prints either the error message or the lexeme and the token value
  for (int i = 0; i < sp; i++)
  {
    if (tokens[i] == ID_ERR)
    {
      printf("Error : Identifier names cannot exceed 11 characters\n");
      exit(0);
    }
    else if (tokens[i] == NUM_ERR)
    {
      printf("Error : Numbers cannot exceed 5 digits\n");
      exit(0);
    }
    else if (tokens[i] == SYM_ERR)
    {
      printf("Error : Invalid Symbol\n");
      exit(0);
    }
    else if (tokens[i] == DIG_ERR)
    {
      printf("Error : Identifiers cannot begin with a digit\n");
      exit(0);
    }
    else
      printf("%11s     %d\n", lex[i], tokens[i]);
  }

  // prints the token list
  printf("\nToken List:\n");
  for (int i = 0; i < sp; i++)
  {
    if (tokens[i] < 0)
      continue;
    printf("%d ", tokens[i]);
    if (tokens[i] == 2)
      printf("%s ", lex[i]);
    if (tokens[i] == 3)
      printf("%s ", lex[i]);
  }

  printf("\n");
  return lexeme_list;
}
