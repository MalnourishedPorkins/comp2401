#include <stdio.h>
#include <string.h>
#include "defs.h"

#define MAX_STR      32
#define MAX_ARR_SIZE 32
#define NOT_FOUND -1

void initPgm(PgmType*);
void initStmt(char*, InstrCategory, OperandType, OperandType, StmtType*);
void initOperand(OpCategory, int, int, char*, OperandType*);
void initOperandVar(OpCategory, int, OperandType*);
void initOperandLabel(OpCategory, int, OperandType*);
void operandNotUsed(OperandType*);
void operandLabelNotFnd(char*, OperandType*);
void initVar(char*, int, VarType*);
int findVar(char*, PgmType*);
int findLabel(char*, PgmType*);

int main(){
  PgmType pgm;
  initPgm(&pgm);
  int strCounter = 0;
  int len;
  char colon;
  char str[MAX_STR]; // User input
  char label[MAX_STR]; // Used to splice off colon on inputted labels
  StmtType currentStmt;
  
  while(1){
    
    scanf("%s", str); // input from user delimited by space character or new line
    len = strlen(str); // Length of current str
    colon = str[len-1]; // Stores the last character of each inputted str into colon
   
    if(colon == ':') {
      strncpy(label, str, len - 1); // Copies everything but the colon
      strcpy(currentStmt.label, label); // Applies label to currentStmt structure
      strCounter++;
      continue;
    }
     
    // READ instruction condition
    //---------------------------
    if(strcmp(str, "read") == 0){
      currentStmt.instr = READ;
      strCounter++; // Increments strCounter
      scanf("%s", str); // Read next line from user (must be variable name)
      VarType var; initVar(str, 0, &var); 
      pgm.vars[pgm.numVars] = var; // Places var in pgm collection of variables
           
      OperandType opA, opB; // Initalize Operands for the statement
      initOperandVar(VAR, pgm.numVars, &opA);
      operandNotUsed(&opB);
      pgm.numVars++; // Increment vars value inside program structure

      currentStmt.op1 = opA; currentStmt.op2 = opB; // Add operands to statement
      pgm.stmts[pgm.numStmts] = currentStmt; // Add statement to program collection of stmts
      pgm.numStmts++; // Increment stmts value inside program structure
    }  
    
    
    // PRINT instruction condition
    //----------------------------
    if(strcmp(str, "print") == 0){
      currentStmt.instr = PRINT;
      strCounter++;
      int index;
      scanf("%s", str);
      
      index = findVar(str, &pgm);
      OperandType opA, opB; // Initalize Operands for the statement
      if(index != NOT_FOUND){
	initOperandVar(VAR, index, &opA);
	operandNotUsed(&opB);
      } 
      else{
	operandNotUsed(&opA);
	operandNotUsed(&opB);
      }
      currentStmt.op1 = opA; currentStmt.op2 = opB; // Add operands to statement
      pgm.stmts[pgm.numStmts] = currentStmt; // Add statement to program collection of stmts
      pgm.numStmts++; // Increment stmts value inside program structure
    }

    // COMP instruction condition
    //----------------------------
    if(strcmp(str, "comp") == 0){
      currentStmt.instr = COMP;
      strCounter++;
      
      int index1; int index2; char var1[MAX_STR]; char var2[MAX_STR];
      scanf("%s", str);
      strcpy(var1, str);
      // printf("%s\n", var1);

      scanf("%s", str);
      strcpy(var2, str);

      index1 = findVar(var1, &pgm); index2 = findVar(var2, &pgm);
      OperandType opA, opB; // Initalize Operands for the statement
      if(index1 != NOT_FOUND && index2 != NOT_FOUND){
	initOperandVar(VAR, index1, &opA);
	initOperandVar(VAR, index2, &opB);
      } 
      else{
	operandNotUsed(&opA);
	operandNotUsed(&opB);
      }
      currentStmt.op1 = opA; currentStmt.op2 = opB; // Add operands to statement
      pgm.stmts[pgm.numStmts] = currentStmt; // Add statement to program collection of stmts
      pgm.numStmts++; // Increment stmts value inside program structure
    }
    
    // JMORE instruction condition
    //-----------------------------
    if(strcmp(str, "jmore") == 0){
      currentStmt.instr = JMORE;
      strCounter++;

      int index;
      scanf("%s", str);
      index = findLabel(str, &pgm);
      printf("index: %d label: %s\n", index, str);

      OperandType opA, opB; // Initalize Operands for the statement
      if(index != NOT_FOUND){
	initOperandLabel(LABEL, index, &opA);
	operandNotUsed(&opB);
      } 
      else{
	operandLabelNotFnd(str, &opA);
	operandNotUsed(&opB);
      }
      currentStmt.op1 = opA; currentStmt.op2 = opB; // Add operands to statement
      pgm.stmts[pgm.numStmts] = currentStmt; // Add statement to program collection of stmts
      pgm.numStmts++; // Increment stmts value inside program structure
    }
    
    // TODO: jmore instructions, jump instructions, end instruction, resolve labels
    
    strCounter++; // Counter
    if(strcmp(str, "end") == 0) // Exits while loop when 'end' is entered by the user
       break;

  }

  printPgm(&pgm);
  return 0;
}
void initPgm(PgmType *pgm){
  pgm->numStmts = 0;
  pgm->numVars = 0;
  pgm->flag = NOT_SET;
}

void initStmt(char *l, InstrCategory iCat, OperandType opA, OperandType opB, StmtType *stmt){
  strcpy(stmt->label, l);
  stmt->instr = iCat;
  stmt->op1 = opA;
  stmt->op2 = opB;
}

void initOperand(OpCategory opT, int labelIndex, int varIndex, char *tmpLab, OperandType *op){
  op->opType = opT;
  op->label = labelIndex;
  op->var = varIndex;
  strcpy(op->tmpLabel, tmpLab);
}

void initOperandVar(OpCategory opT, int varIndex, OperandType *op){
  op->opType = opT;
  op->var = varIndex;
}

void initOperandLabel(OpCategory opT, int labelIndex, OperandType *op){
  op->opType = opT;
  op->label = labelIndex;
}

void operandNotUsed(OperandType *op){
  op->opType = NOT_USED;
}

void operandLabelNotFnd(char *label, OperandType *op){
  //printf("inside! %s", label);
  op->opType = LABEL_NOT_FND;
  strcpy(op->tmpLabel, label);
}

void initVar(char *n, int v, VarType *var){
  strcpy(var->name, n);
  var->value = v;
}

// Returns index of variable if found
int findVar(char *n, PgmType *pmg){
  int i;

  for(i = 0; i < pmg->numVars; i++){
    //printf("|%s == %s|\n", n, pmg->vars[i].name);
    
    if(strcmp(n, pmg->vars[i].name) == 0)
      return i;
  }
  return NOT_FOUND;
}

// Returns index of stmt using inputted label if found
int findLabel(char *l, PgmType *pmg){
  int i;

  for(i = 0; i < pmg->numStmts; i++){
    //printf("|%s == %s|\n", n, pmg->stmts[i].label);
    
    if(strcmp(l, pmg->stmts[i].label) == 0)
      return i;
  }
  return NOT_FOUND;
}
