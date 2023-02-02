// -----------------------------------------------------------------------------
// Grayson North
// gr967419
// COP 3402
// Spring 2021
// -----------------------------------------------------------------------------

// ------------------- LIBRARIES, HEADERS AND DEFINITIONS ----------------------
#include <stdlib.h>
#include <stdio.h>
#include "compiler.h"
#define MAX_STACK_HEIGHT 1000
#define MAX_CODE_LENGTH 1000
// -----------------------------------------------------------------------------

// ------------------------------- PROTOTYPES ----------------------------------

// -----------------------------------------------------------------------------

// ================================= TO DO =====================================
// CREATE README.TXT AND SUBMIT TO WEBCOURSES
// =============================================================================

// --------------------------- REQUIRED FUNCTIONS ------------------------------
// base function included in the instruction used to help find the base L
// levels down.
int base(int stack[], int level, int BP)
{
  int base = BP;
  int counter = level;
  while (counter > 0)
  {
    base = stack[base];
    counter--;
  }
  return base;
}

void execute(instruction *code, int print)
{
  if (print != 1)
    return;
  // the register variables and their initial values
  int sp = -1;
  int bp = 0;
  int pc = 0;
  int codeCount = 0;
  // an array of the names of the operations to be printed to the screen
  // "NON" is just a place holder due to the ops not being zero inclusive
  char *opNames[] = {"NON", "LIT", "OPR", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SYS"};
  // an array of the names of our math operations to be printed to the screen
  char *maths[] = {"RTN", "NEG", "ADD", "SUB", "MUL", "DIV", "ODD", "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"};
  // our text data segment is a MAX_CODE_LENGTH X 3 2d array made for easy fetching
  // during our fetch execute cycle
  int text[MAX_CODE_LENGTH][3];
  int stack[MAX_STACK_HEIGHT];
  int halt = 1;

  // initialization of our stack that will also be used as the activation record
  for (int i = 0; i < MAX_STACK_HEIGHT; i++)
    stack[i] = 0;

  printf("\n");
  printf("                PC   BP   SP   stack\n");
  printf("Initial values: %-2d   %-2d   %-2d   \n", pc, bp, sp);

  while (halt == 1)
  {
    // this if statement was necessary due to two the system operation having a
    // print statement that must run before any of the registers get edited or
    // values get stored in the stack-> Those instructions have their own
    // dedicated print statements in their respective cases
    if (code[pc].opcode != 9 && (code[pc].m != 1 || code[pc].m != 2))
    {
      // if else statement so that the proper operation name is printed to the
      // screen
      if (code[pc].opcode == 2 && code[pc].m != 0)
        printf("%2d OPR %2d %2d", pc, code[pc].l, code[pc].m);
      else
        printf("%2d %s %2d %2d", pc, opNames[code[pc].opcode], code[pc].l, code[pc].m);
    }

    // here is where the execution part of the fetch/execute cycle begins
    // notice that almost all the cases have an extra pc = pc + 1 line due
    // to properly increment the fetch execute cycle-> This was due to some of
    // the cases (JMP) not requiring an increase of the PC and jumping instead
    // otherwise I would have left a pc++ at the end of the while loop
    switch(code[pc].opcode)
    {
      // LIT
      case 1:
        sp = sp + 1;
        stack[sp] = code[pc].m;
        pc = pc + 1;
        break;

        // OPR
        case 2:
        switch(code[pc].m)
        {
          // RTN
          case 0:
            stack[bp - 1] = stack[sp];
            sp = bp - 1;
            bp = stack[sp + 2];
            pc = stack[sp + 3];
            break;

          // NEG
          case 1:
            stack[sp] = -1 * stack[sp];
            pc = pc + 1;
            break;

          // ADD
          case 2:
            sp = sp - 1;
            stack[sp] = stack[sp] + stack[sp + 1];
            pc = pc + 1;
            break;

          // SUB
          case 3:
            sp = sp - 1;
            stack[sp] = stack[sp] - stack[sp + 1];
            pc = pc + 1;
            break;

          // MUL
          case 4:
            sp = sp - 1;
            stack[sp] = stack[sp] * stack[sp + 1];
            pc = pc + 1;
            break;

          // DIV
          case 5:
            sp = sp - 1;
            stack[sp] = stack[sp] / stack[sp + 1];
            pc = pc + 1;
            break;

          // ODD
          case 6:
            stack[sp] = stack[sp] % 2;
            pc = pc + 1;
            break;

          // MOD
          case 7:
            sp = sp - 1;
            stack[sp] = stack[sp] % stack[sp + 1];
            pc = pc + 1;
            break;

          // EQL
          case 8:
            sp = sp - 1;
            stack[sp] = stack[sp] == stack[sp + 1];
            pc = pc + 1;
            break;

          // NEQ
          case 9:
            sp = sp - 1;
            stack[sp] = stack[sp] != stack[sp + 1];
            pc = pc + 1;
            break;

          // LSS
          case 10:
            sp = sp - 1;
            stack[sp] = stack[sp] < stack[sp + 1];
            pc = pc + 1;
            break;

          // LEQ
          case 11:
            sp = sp - 1;
            stack[sp] = stack[sp] <= stack[sp + 1];
            pc = pc + 1;
            break;

          // GTR
          case 12:
            sp = sp - 1;
            stack[sp] = stack[sp] > stack[sp + 1];
            pc = pc + 1;
            break;

          // GEQ
          case 13:
            sp = sp - 1;
            stack[sp] = stack[sp] >= stack[sp + 1];
            pc = pc + 1;
            break;
        }
        break;

        // LOD
        case 3:
          sp = sp + 1;
          stack[sp] = stack[base(stack, code[pc].l, bp) + code[pc].m];
          pc = pc + 1;
          break;

        // STO
        case 4:
          stack[base(stack, code[pc].l, bp) + code[pc].m] = stack[sp];
          sp = sp - 1;
          pc = pc + 1;
          break;

        // CAL
        case 5:
          stack[sp + 1] = base(stack, code[pc].l, bp); // static link
          stack[sp + 2] = bp;                             // dynamic link
          stack[sp + 3] = pc + 1;                         // return address
          stack[sp + 4] = stack[sp];                      // parameter
          bp = sp + 1;
          pc = code[pc].m;
          break;

        // INC
        case 6:
          sp = sp + code[pc].m;
          pc = pc + 1;
          break;

        // JMP
        case 7:
          pc = code[pc].m;
          break;

        // JPC
        case 8:
          if (stack[sp] == 0)
            pc = code[pc].m;
          sp = sp - 1;
          break;

        // SYS
        case 9:
          switch (code[pc].m)
          {
            case 1:
              printf("Top of Stack Value: %d\n", stack[sp]);
              printf("%2d SYS %2d %2d", pc, code[pc].l, code[pc].m);
              sp = sp - 1;
              pc = pc + 1;
              break;

            case 2:
              sp = sp + 1;
              printf("Please Enter an Integer:");
              scanf("%d", &stack[sp]);
              printf("\n%2d SYS %2d %2d", pc, code[pc].l, code[pc].m);
              pc = pc + 1;
              break;

            // SET HALT TO 0
            case 3:
              printf("%2d SYS %2d %2d", pc, code[pc].l, code[pc].m);
              halt = 0;
              pc = pc + 1;
              break;
          }
          break;
      }
      // The final print statement which prints the current values in the
      // PC BP & SP registers.
      printf("    %2d   %2d   %2d  ", pc, bp, sp);
      for (int i = 0; i <= sp; i++)
      {
        // here i realized when printing the stack that the only time a | was
        // printed was when the index of the stack that was being printed was
        // the same as the base pointer.
        if (i == bp && i != 0)
          printf(" |");
        // otherwise print the value of the stack at index i
        printf(" %d", stack[i]);
      }
      printf("\n");
  }
  return;
}
