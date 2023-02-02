// -----------------------------------------------------------------------------
// Grayson North
// gr967419
// COP 3402
// Spring 2021
// -----------------------------------------------------------------------------

// ================================= TO DO =====================================
// error7 is being called which means symbolTableCheck is returning -1 when it
// shouldn't, FIX IT.
// =============================================================================

// ------------------- DEFINITIONS, HEADERS AND LIBRARIES ----------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

#define OPR 2
#define UNMARKED 0
// -----------------------------------------------------------------------------

// --------------------- GLOBAL VARIABLES AMD STRUCTS --------------------------
instruction *code;
symbol *table;
int procedureCount = 0;
int codeIdx = 0;
int numVars = 0;
int symIdx = 0;
int endOfTable = 0;
int tokenIdx = 0;
int loopIdx = 0;
int jpcIdx = 0;
int jmpIdx = 0;
char *errormsg[] = {
	"",
	"Error : program must end with period\n",
	"Error : const, var, call, and read keywords must be followed by identifier\n",
	"Error : competing symbol declarations at the same level\n",
	"Error : constants must be assigned with =\n",
	"Error : constants must be assigned an integer value\n",
	"Error : symbol declarations must be followed by a semicolon\n",
	"Error : undeclared variable or constant in the equation\n",
	"Error : only variable values may be altered\n",
	"Error : assignment statements must use :=\n",
	"Error : begin must be followed by end\n",
	"Error : if must be followed by then\n",
	"Error : while must be followed by do\n",
	"Error : condition must contain comparison operator\n",
	"Error : right parenthesis must follow left parenthesis\n",
	"Error : arithmetic equations must contain operands, parentheses, numbers, or symbols\n",
	"Error : undeclared procedure for call\n",
	"Error : parameters may only be specified by an identifier\n",
	"Error : parameters must be declared\n",
	"Error : cannot return from main\n"
};
// -----------------------------------------------------------------------------

// ------------------------------- PROTOTYPES ----------------------------------
void emit(int opcode, char op[], int l, int m);
void printCode(instruction *code);
void add(int kind, char name[], int value, int level, int addr, int mark, int param);
void getNextToken();
int symbolTableCheck(lexeme *tokenList, int lexLevel);
int symbolTableSearch(lexeme *tokenList, int lexLevel, int kind);
int findProcedure(int num);
void mark();
void error(int n);
void program(lexeme *tokenList);
void block(lexeme *tokenList, int lexLevel, int param, int procIdx);
int constDeclaration(lexeme *tokenList, int lexLevelm);
int varDeclaration(lexeme *tokenList, int lexLevel, int param);
int procDeclaration(lexeme *tokenList, int lexLevel);
void statement(lexeme *tokenList, int lexLevel);
void expression(lexeme *tokenList, int lexLevel);
void term(lexeme *tokenList, int lexLevel);
void factor(lexeme *tokenList, int lexLevel);
void condition(lexeme *tokenList, int lexLevel);
// -----------------------------------------------------------------------------

// --------------------------- REQUIRED FUNCTIONS ------------------------------

// this function was taken from the skeleton available in webcourses
instruction *parse(lexeme *tokenList, int print)
{
	code = malloc(1000 * sizeof(instruction));
	table = malloc (1000 * sizeof(symbol));
	program(tokenList);
	if (print != 1)
	{
		free(table);
		return code;
	}
	printCode(code);
	free(table);
	return code;
}

// this function was taken from the skeleton available in webcourses
void emit(int opcode, char op[], int l, int m)
{
	code[codeIdx].opcode = opcode;
	strcpy(code[codeIdx].op, op);
	code[codeIdx].l = l;
	code[codeIdx].m = m;
	codeIdx++;
	return;
}

// this function prints the generated assembly going from 0 to value of codeIdx
// (a global variable)
void printCode(instruction *code)
{
	int i = 0;
	printf("Generated Assembly:\n");
	printf("Line    OP    L    M\n");
	while (i < codeIdx)
	{
		printf("%3d    %s%5d%5d\n", i, code[i].op, code[i].l, code[i].m);
		i++;
	}
	printf("\n");
	return;
}

// simply sets the values of the symbol table to their associated parameters
void add(int kind, char name[], int value, int level, int addr, int mark, int param)
{
	table[symIdx].kind = kind;
	strcpy(table[symIdx].name, name);
	table[symIdx].val = value;
	table[symIdx].level = level;
	table[symIdx].addr = addr;
	table[symIdx].mark = mark;
	table[symIdx].param = param;
	symIdx++;
	endOfTable++;
}

// increments token index
void getNextToken()
{
	tokenIdx++;
	return;
}

// a linear search through the symbol table, runs in O(n)
int symbolTableCheck(lexeme *tokenList, int lexLevel)
{
	int i = 0;
	while (i < 1000)
	{
		if ((strcmp(table[i].name, tokenList[tokenIdx].name) == 0) && table[i].level == lexLevel)
			return i;
		i++;
	}
	return -1;
}

int symbolTableSearch(lexeme *tokenList, int lexLevel, int kind)
{
	for (int i = endOfTable; i >= 0; i--)
		if ((strcmp(table[i].name, tokenList[tokenIdx].name) == 0) && table[i].level <= lexLevel && table[i].kind == kind && table[i].mark == UNMARKED)
			return i;
	return -1;
}

int findProcedure(int num)
{
	for (int i = 1000; i >= 0; i--)
		if (table[i].val == num && table[i].kind == 3)
			return i;
}

void mark(int count)
{
	for (int i = endOfTable; i > 0; i--)
	{
		if (count == 0)
			break;
		if (table[i].mark == UNMARKED)
		{
			table[i].mark = 1;
			count--;
		}
		else
			continue;
	}
}

// prints the error message based on the integer passed into the function
void error(int n)
{
	printf("%s\n", errormsg[n]);
	exit(0);
}
// =============================================================================
// THE FOLLOWING CODE IS BASED OFF THE LOGIC PRESENTED IN APPENDIX D: PSUEDOCODE
// =============================================================================
void program(lexeme *tokenList)
{
	int numProc = 1;
	int count = 0;
	emit(7, "JMP", 0, 0);
	while (strcmp(tokenList[count].name, "\0") != 0 || tokenList[count].type != 0)
	{
		if (tokenList[count].type == procsym)
		{
			numProc++;
			emit(7, "JMP", 0, 0);
		}
		count++;
	}
	add(3, "main", 0, 0, 0, UNMARKED, 0);
	procedureCount++;
	block(tokenList, 0, 0, 0);
	if (tokenList[tokenIdx].type != periodsym)
		error(1);
	for (int i = 0; i < numProc; i++)
		code[i].m = table[findProcedure(i)].addr; // maybe this is fixed???
	for (int i = 0; i < 500; i++)
		if (code[i].opcode == 5)
			code[i].m = table[findProcedure(code[i].m)].addr; // maybe this is fixed???
	emit(9, "SYS", 0, 3);
}

void block(lexeme *tokenList, int lexLevel, int param, int procIdx)
{
	int c = constDeclaration(tokenList, lexLevel);
	int v	= varDeclaration(tokenList, lexLevel, param);
	int p = procDeclaration(tokenList, lexLevel);
	table[procIdx].addr = codeIdx;
	emit(6, "INC", 0, 4 + v);
	statement(tokenList, lexLevel);
	mark(c + v + p);
}

int constDeclaration(lexeme *tokenList, int lexLevel)
{
	int numConst = 0;
	char name[12];
	if (tokenList[tokenIdx].type == constsym)
	{
		do
		{
			numConst++;
			getNextToken();
			if (tokenList[tokenIdx].type != identsym)
				error(2);
			if (symbolTableCheck(tokenList, lexLevel) != -1)
				error(7);
			strcpy(name, tokenList[tokenIdx].name);
			getNextToken();
			if (tokenList[tokenIdx].type != eqlsym)
				error(4);
			getNextToken();
			if (tokenList[tokenIdx].type != numbersym)
				error(5);
			add(1, name, tokenList[tokenIdx].value, lexLevel, 0, UNMARKED, 0); // fix later
			getNextToken();
		} while (tokenList[tokenIdx].type == commasym);
		if (tokenList[tokenIdx].type != semicolonsym)
			error(6);
		getNextToken();
	}
	return numConst;
}

int varDeclaration(lexeme *tokenList, int lexLevel, int param)
{
	int numVars;
	if (param == 1)
		numVars = 1;
	else
		numVars = 0;
	if (tokenList[tokenIdx].type == varsym)
	{
		do
		{
			numVars++;
			getNextToken();
			if (tokenList[tokenIdx].type != identsym)
				error(2);
			if (symbolTableCheck(tokenList, lexLevel) != -1)
				error(7);
			add(2, tokenList[tokenIdx].name, 0, lexLevel, numVars + 3, UNMARKED, 0);
			getNextToken();
		} while (tokenList[tokenIdx].type == commasym);
		if (tokenList[tokenIdx].type != semicolonsym)
			error(6);
		getNextToken();
	}
	return numVars;
}

int procDeclaration(lexeme *tokenList, int lexLevel)
{
	int numProc = 0;
	int procIdx = 0;
	if (tokenList[tokenIdx].type == procsym)
		do
		{
			numProc++;
			getNextToken();
			if (tokenList[tokenIdx].type != identsym)
				error(2);
			if (symbolTableCheck(tokenList, lexLevel) != -1)
				error(3);
			procIdx = endOfTable;
			add(3, tokenList[tokenIdx].name, procedureCount, lexLevel, 0, UNMARKED, 0);
			procedureCount++;
			getNextToken();
			if (tokenList[tokenIdx].type == lparentsym)
			{
				getNextToken();
				if (tokenList[tokenIdx].type != identsym)
					error(2);
				add(2, tokenList[tokenIdx].name, 0, lexLevel + 1, 3, UNMARKED, 0);
				table[procIdx].param = 1;
				getNextToken();
				if (tokenList[tokenIdx].type != rparentsym)
					error(14);
				getNextToken();
				if (tokenList[tokenIdx].type != semicolonsym)
					error(6);
				getNextToken();
				block(tokenList, lexLevel + 1, 1, procIdx);
			}
			else
			{
				if (tokenList[tokenIdx].type != semicolonsym)
					error(6);
				getNextToken();
				block(tokenList, lexLevel + 1, 0, procIdx);
				if (code[codeIdx - 1].opcode != 2 && code[codeIdx - 1].m != 0)
				{
					emit(1, "LIT", 0, 0);
					emit(2, "OPR", 0, 0);
				}
				if (tokenList[tokenIdx].type != semicolonsym)
					error(6);
				getNextToken();
			}
		} while (tokenList[tokenIdx].type == procsym);
	return numProc;
}

void statement(lexeme *tokenList, int lexLevel)
{
	if (tokenList[tokenIdx].type == identsym)
	{
		symIdx = symbolTableSearch(tokenList, lexLevel, 2);
		if (symIdx == -1)
			error(7);
		getNextToken();
		if (tokenList[tokenIdx].type != becomessym)
			error(9);
		getNextToken();
		expression(tokenList, lexLevel);
		emit(4, "STO", lexLevel - table[symIdx].level, table[symIdx].addr);
		return;
	}
	if (tokenList[tokenIdx].type == callsym)
	{
		getNextToken();
		if (tokenList[tokenIdx].type != identsym)
			error(2);
		symIdx = symbolTableSearch(tokenList, lexLevel, 3);
		if (symIdx == -1)
			error(7);
		getNextToken();
		if (tokenList[tokenIdx].type == lparentsym)
		{
			getNextToken();
			if (table[symIdx].param != 1)
				error(18); //fix later
			expression(tokenList, lexLevel);
			if (tokenList[tokenIdx].type != rparentsym)
				error(14);
			getNextToken();
		}
		else
			emit(1, "LIT", 0, 0);
		emit(5, "CAL", lexLevel - table[symIdx].level, table[symIdx].val);
		return;
	}
	if (tokenList[tokenIdx].type == returnsym)
	{
		if (lexLevel == 0)
			error(19); // fix later
		getNextToken();
		if (tokenList[tokenIdx].type == lparentsym)
		{
			getNextToken();
			expression(tokenList, lexLevel);
			emit(2, "OPR", 0, 0); // fix later
			if (tokenList[tokenIdx].type != rparentsym)
				error(14);
			getNextToken();
		}
		else
		{
			emit(1, "LIT", 0, 0);
			emit(2, "OPR", 0, 0);
		}
		return;
	}
	if (tokenList[tokenIdx].type == beginsym)
	{
		do
		{
			getNextToken();
			statement(tokenList, lexLevel);
		} while (tokenList[tokenIdx].type == semicolonsym);
		if (tokenList[tokenIdx].type != endsym)
			error(9);
		getNextToken();
		return;
	}
	if (tokenList[tokenIdx].type == ifsym)
	{
		getNextToken();
		condition(tokenList, lexLevel);
		jpcIdx = codeIdx;
		emit(8, "JPC", 0, 0);
		if (tokenList[tokenIdx].type != thensym)
			error(10);
		getNextToken();
		statement(tokenList, lexLevel);
		if (tokenList[tokenIdx].type == elsesym)
		{
			getNextToken();
			jmpIdx = codeIdx;
			emit(7, "JMP", 0, 0); // fix later
			code[jpcIdx].m = codeIdx;
			statement(tokenList, lexLevel);
			code[jmpIdx].m = codeIdx;
		}
		else
			code[jpcIdx].m = codeIdx;
		return;
	}
	if (tokenList[tokenIdx].type == whilesym)
	{
		getNextToken();
		loopIdx = codeIdx;
		condition(tokenList, lexLevel);
		if (tokenList[tokenIdx].type != dosym)
			error(11);
		getNextToken();
		jpcIdx = codeIdx;
		emit(8, "JPC", 0, 0);
		statement(tokenList, lexLevel);
		emit(7, "JMP", 0, loopIdx);
		code[jpcIdx].m = codeIdx;
		return;
	}
	if (tokenList[tokenIdx].type == readsym)
	{
		getNextToken();
		if (tokenList[tokenIdx].type != identsym)
			error(2);
		symIdx = symbolTableSearch(tokenList, lexLevel, 2);
		if (symIdx == -1)
			error(7);
		getNextToken();
		emit(9, "SYS", 0, 2);
		emit(4, "STO", lexLevel - table[symIdx].level, table[symIdx].addr);
		return;
	}
	if (tokenList[tokenIdx].type == writesym)
	{
		getNextToken();
		expression(tokenList, lexLevel);
		emit(9, "SYS", 0, 1);
		return;
	}
}

void condition(lexeme *tokenList, int lexLevel)
{
	if (tokenList[tokenIdx].type == oddsym)
	{
		getNextToken();
		expression(tokenList, lexLevel);
		emit(OPR, "OPR", 0, 6);
	}
	else
	{
		expression(tokenList, lexLevel);
		if (tokenList[tokenIdx].type == eqlsym)
		{
			getNextToken();
			expression(tokenList, lexLevel);
			emit(OPR, "OPR", 0, 8);
		}
		else if (tokenList[tokenIdx].type == neqsym)
		{
			getNextToken();
			expression(tokenList, lexLevel);
			emit(OPR, "OPR", 0, 9);
		}
		else if (tokenList[tokenIdx].type == lessym)
		{
			getNextToken();
			expression(tokenList, lexLevel);
			emit(OPR, "OPR", 0, 10);
		}
		else if (tokenList[tokenIdx].type == leqsym)
		{
			getNextToken();
			expression(tokenList, lexLevel);
			emit(OPR, "OPR", 0, 11);
		}
		else if (tokenList[tokenIdx].type == gtrsym)
		{
			getNextToken();
			expression(tokenList, lexLevel);
			emit(OPR, "OPR", 0, 12);
		}
		else if (tokenList[tokenIdx].type == geqsym)
		{
			getNextToken();
			expression(tokenList, lexLevel);
			emit(OPR, "OPR", 0, 13);
		}
		else
			error(15);
	}
}

void expression(lexeme *tokenList, int lexLevel)
{
	if (tokenList[tokenIdx].type == minussym)
	{
		getNextToken();
		term(tokenList, lexLevel);
		emit(2, "OPR", 0, 1);
		while (tokenList[tokenIdx].type == plussym || tokenList[tokenIdx].type == minussym)
		{
			if (tokenList[tokenIdx].type == plussym)
			{
				getNextToken();
				term(tokenList, lexLevel);
				emit(2, "OPR", 0, 2);
			}
			else
			{
				getNextToken();
				term(tokenList, lexLevel);
				emit(2, "OPR", 0, 3);
			}
		}
	}
	else
	{
		if (tokenList[tokenIdx].type == plussym)
			getNextToken();
		term(tokenList, lexLevel);
		while (tokenList[tokenIdx].type == plussym || tokenList[tokenIdx].type == minussym)
		{
			if (tokenList[tokenIdx].type == plussym)
			{
				getNextToken();
				term(tokenList, lexLevel);
				emit(2, "OPR", 0, 2);
			}
			else
			{
				getNextToken();
				term(tokenList, lexLevel);
				emit(2, "OPR", 0, 3);
			}
		}
	}
}

void term(lexeme *tokenList, int lexLevel)
{
	factor(tokenList, lexLevel);
	while (tokenList[tokenIdx].type == multsym || tokenList[tokenIdx].type == slashsym || tokenList[tokenIdx].type == modsym)
	{
		if (tokenList[tokenIdx].type == multsym)
		{
			getNextToken();
			factor(tokenList, lexLevel);
			emit(OPR, "OPR", 0, 4);
		}
		else if (tokenList[tokenIdx].type == slashsym)
		{
			getNextToken();
			factor(tokenList, lexLevel);
			emit(OPR, "OPR", 0, 5);
		}
		else
		{
			getNextToken();
			factor(tokenList, lexLevel);
			emit(OPR, "OPR", 0, 7);
		}
	}
}

void factor(lexeme *tokenList, int lexLevel)
{
	int symIdxV, symIdxC, i;
	if (tokenList[tokenIdx].type == identsym)
	{
		symIdxV = symbolTableSearch(tokenList, lexLevel, 2);
		symIdxC = symbolTableSearch(tokenList, lexLevel, 1);
		if (symIdxV == -1 && symIdxC == -1)
			error(3);// fix later
		else if (symIdxC == -1 || (symIdxV != -1 && table[symIdxV].level < table[symIdxC].level))
			emit(3, "LOD", lexLevel - table[symIdxV].level, table[symIdxV].addr);
		else
			emit(1, "LIT", 0, table[symIdxC].val);
		getNextToken();
	}
	else if (tokenList[tokenIdx].type == numbersym)
	{
		emit(1, "LIT", 0, tokenList[tokenIdx].value);
		getNextToken();
	}
	else if (tokenList[tokenIdx].type == lparentsym)
	{
		getNextToken();
		expression(tokenList, lexLevel);
		if (tokenList[tokenIdx].type != rparentsym);
			error(14);
		getNextToken();
	}
	else if (tokenList[tokenIdx].type == callsym)
		statement(tokenList, lexLevel);
	else
		error(0);
}
// -----------------------------------------------------------------------------
