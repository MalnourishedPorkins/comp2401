#include <stdio.h>
#include <string.h>
#include "defs.h"


void printPgm(PgmType *pgmPtr)
{
  printPgmStmts(pgmPtr);
  printPgmVars(pgmPtr);
}

void printPgmStmts(PgmType *pgmPtr)
{
  int num = pgmPtr->numStmts;
  int varIndex, stmtIndex, i;

  printf("\n*** STATEMENTS:\n");

  for (i=0; i<num; i++) {
    printf("-- stmt %2d:  ", i);

    if (strcmp(pgmPtr->stmts[i].label, "") == 0) {
      printf("no label, ");
    }
    else {
      printf("label %s, ", pgmPtr->stmts[i].label);
    }
    printf("instr=");

    switch(pgmPtr->stmts[i].instr) {
      case READ:   printf("READ"); break;
      case PRINT:  printf("PRINT"); break;
      case COMP:   printf("COMP"); break;
      case JUMP:   printf("JUMP"); break;
      case JMORE:  printf("JMORE"); break;
      case END:    printf("END");
    }
    printf("\n");

    printf("----   op1:  ");

    switch(pgmPtr->stmts[i].op1.opType) {
      case VAR:
        varIndex = pgmPtr->stmts[i].op1.var;
        printf("-- type VAR, name %s", pgmPtr->vars[varIndex].name);
        break;
      case LABEL:
        stmtIndex = pgmPtr->stmts[i].op1.label;
        printf("-- type LABEL, name %s", pgmPtr->stmts[stmtIndex].label);
        break;
      case NOT_USED:
        printf("-- type NOT_USED");	
    }
    printf("\n");

    printf("----   op2:  ");

    switch(pgmPtr->stmts[i].op2.opType) {
      case VAR:
        varIndex = pgmPtr->stmts[i].op2.var;
        printf("-- type VAR, name %s", pgmPtr->vars[varIndex].name);
        break;
      case LABEL:
        stmtIndex = pgmPtr->stmts[i].op2.label;
        printf("-- type LABEL, name %s", pgmPtr->stmts[stmtIndex].label);
        break;
      case NOT_USED:
        printf("-- type NOT_USED");
    }
    printf("\n");
  }

}

void printPgmVars(PgmType *pgmPtr)
{
  int num = pgmPtr->numVars;
  int i;

  printf("\n*** VARIABLES:\n");

  for (i=0; i<num; i++) {
    printf("-- var %2d:  %s = %d\n", i, pgmPtr->vars[i].name, pgmPtr->vars[i].value);
  }

}

