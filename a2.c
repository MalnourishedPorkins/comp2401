#include <stdio.h>
#include <string.h>
#include "defs.h"

#define MAX_STR      32
#define MAX_ARR_SIZE 32
#define VAR_NOT_FOUND -1

void initPgm(PgmType*);
void initStmt(char*, InstrCategory, OperandType, OperandType, StmtType*);
void initOperand(OpCategory, int, int, char*, OperandType*);
void initOperandVar(OpCategory, int, OperandType*);
void operandNotUsed(OperandType*);
void initVar(char*, int, VarType*);
int findVar(char*, PgmType*);

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
      if(index >= 0){
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
      printf("%s\n", var1);

      scanf("%s", str);
      strcpy(var2, str);

      index1 = findVar(var1, &pgm); index2 = findVar(var2, &pgm);
      OperandType opA, opB; // Initalize Operands for the statement
      if(index1 >= 0 && index2 >=0){
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
  
    
    strCounter++; // Counter
    if(strcmp(str, "end") == 0) // Exits while loop when 'end' is entered by the user
       break;

  }
  /*
  VarType var;
  int v = 20;
  initVar("a", v, &var);
  pgm.vars[pgm.numVars] = var; pgm.numVars++;

  OperandType opA; OperandType opB;
  OpCategory opCat = VAR; OpCategory opCatB = NOT_USED;
  initOperand(opCat, -1, 0, "", &opA);
  initOperand(opCatB, -1, -1, "", &opB);
  
  StmtType stmt;
  initStmt("", READ, opA, opB, &stmt);
  pgm.stmts[pgm.numStmts] = stmt; pgm.numStmts++;
  
  printPgm(&pgm);
  */
  //int index;
  //index = findVar("c", &pgm);
  //printf("Index: %d\n", index);

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

void operandNotUsed(OperandType *op){
  op->opType = NOT_USED;
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
  return VAR_NOT_FOUND;
}
