#include <stdio.h>
#include <string.h>
#include "defs.h"

#define MAX_STR      32
#define MAX_ARR_SIZE 32
#define NOT_FOUND -1

// Forward references and function prototypes
void initPgm(PgmType*);
void initStmt(char*, InstrCategory, OperandType, OperandType, StmtType*);
void initOperand(OpCategory, int, int, char*, OperandType*);
void initOperandVar(int, OperandType*);
void initOperandLabel(int, OperandType*);
void operandNotUsed(OperandType*);
void operandLabelNotFnd(char*, OperandType*);
void initVar(char*, int, VarType*);
int findVar(char*, PgmType*);
int findLabel(char*, PgmType*);
void resolveLabels(PgmType*);

int main(){
  PgmType pgm; // PgmType instance that will store stmts collection and vars collection
  initPgm(&pgm); // Initalizes pgm values
  int strCounter = 0;
  int len;
  char colon;
  char str[MAX_STR]; // User input
  StmtType currentStmt; strcpy(currentStmt.label, "");
  
  while(1){
    
    scanf("%s", str); // input from user delimited by space character or new line
    len = strlen(str); // Length of current str
    colon = str[len-1]; // Stores the last character of each inputted str into colon
   
    if(colon == ':') {
      char label[MAX_STR] = ""; // Used to splice off colon on inputted labels
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
      initOperandVar(pgm.numVars, &opA); // Initalizes opA as a VAR operand 
      operandNotUsed(&opB); // Initalizes opB as a NOT_USED operand
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
      int index; // Value to store returning index from findVar function
      scanf("%s", str);
      
      index = findVar(str, &pgm);
      OperandType opA, opB; // Initalize Operands for the statement
      if(index != NOT_FOUND){
	initOperandVar(index, &opA);
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

      scanf("%s", str);
      strcpy(var2, str);

      index1 = findVar(var1, &pgm); index2 = findVar(var2, &pgm);
      OperandType opA, opB; // Initalize Operands for the statement
      if(index1 != NOT_FOUND && index2 != NOT_FOUND){ // If variables are found in vars collection
	initOperandVar(index1, &opA);
	initOperandVar(index2, &opB);
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

      OperandType opA, opB; // Initalize Operands for the statement
      if(index != NOT_FOUND){
	initOperandLabel(index, &opA);
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

    // JUMP instruction condition
    //-----------------------------
    if(strcmp(str, "jump") == 0){
      currentStmt.instr = JUMP;
      strCounter++;

      int index;
      scanf("%s", str);
      index = findLabel(str, &pgm);

      OperandType opA, opB; // Initalize Operands for the statement
      if(index != NOT_FOUND){
	initOperandLabel(index, &opA);
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
      
    // TODO: resolve labels
    
    strCounter++; // Counter
    if(strcmp(str, "end") == 0){  // Exits while loop when 'end' is entered by the user
      currentStmt.instr = END;
      
      OperandType opA, opB; // Initalize Operands for the statement
      operandNotUsed(&opA); operandNotUsed(&opB);

      currentStmt.op1 = opA; currentStmt.op2 = opB; // Add operands to statement
      pgm.stmts[pgm.numStmts] = currentStmt; // Add statement to program collection of stmts
      pgm.numStmts++; // Increment stmts value inside program structure
      break;
    }   
  }
  
  resolveLabels(&pgm); // Resolves LABEL_NOT_FND operands
  printPgm(&pgm);      // Prints stored information of TASSLA instructions entered 
  return 0;
}

/*
-------------------------------------------------------------------------------------------------------
HELPER FUNCTIONS
-------------------------------------------------------------------------------------------------------
*/

// Initalizes PgmType
void initPgm(PgmType *pgm){
  pgm->numStmts = 0;
  pgm->numVars = 0;
  pgm->flag = NOT_SET;
}

// Initalizes StmtType
void initStmt(char *l, InstrCategory iCat, OperandType opA, OperandType opB, StmtType *stmt){
  strcpy(stmt->label, l);
  stmt->instr = iCat;
  stmt->op1 = opA;
  stmt->op2 = opB;
}

// Initalizes OperandType
void initOperand(OpCategory opT, int labelIndex, int varIndex, char *tmpLab, OperandType *op){
  op->opType = opT;
  op->label = labelIndex;
  op->var = varIndex;
  strcpy(op->tmpLabel, tmpLab);
}

// Initalizes OperandType as a variable
void initOperandVar(int varIndex, OperandType *op){
  op->opType = VAR;
  op->var = varIndex;
}

// Initalizes OperandType as a label
void initOperandLabel(int labelIndex, OperandType *op){
  op->opType = LABEL;
  op->label = labelIndex;
}

// Initalizes OperandType as not used
void operandNotUsed(OperandType *op){
  op->opType = NOT_USED;
}

// Initalizes OperandType as a label not found
void operandLabelNotFnd(char *label, OperandType *op){
  op->opType = LABEL_NOT_FND;
  strcpy(op->tmpLabel, label);
}

// Initalizes VarType
void initVar(char *n, int v, VarType *var){
  strcpy(var->name, n);
  var->value = v;
}

// Returns index of variable if found, other returns NOT_FOUND value (-1)
int findVar(char *n, PgmType *pmg){
  int i;

  for(i = 0; i < pmg->numVars; i++){
    
    if(strcmp(n, pmg->vars[i].name) == 0)
      return i;
  }
  return NOT_FOUND;
}

// Returns index of stmt inside the collection of stmts using inputted label if found, otherwise returns NOT_FOUND value (-1)
int findLabel(char *l, PgmType *pmg){
  int i;

  for(i = 0; i < pmg->numStmts; i++){
    
    if(strcmp(l, pmg->stmts[i].label) == 0)
      return i;
  }
  return NOT_FOUND;
}

// Finds operands with LABEL_NOT_FND after all user input and modifies the operand if a matching label can be found in the collection of stmts
void resolveLabels(PgmType *pgm){
  int i;
  
  // Operand 1 check
  for(i = 0; i < pgm->numStmts; i++){
    int index;

    if(pgm->stmts[i].op1.opType == LABEL_NOT_FND){

      index = findLabel(pgm->stmts[i].op1.tmpLabel, pgm);

      if(index != NOT_FOUND)
	initOperandLabel(index, &pgm->stmts[i].op1);
      else
	operandNotUsed(&pgm->stmts[i].op1);
    }
  }
  
  // Operand 2 check
  for(i = 0; i < pgm->numStmts; i++){
    int index;

    if(pgm->stmts[i].op2.opType == LABEL_NOT_FND){

      index = findLabel(pgm->stmts[i].op2.tmpLabel, pgm);

      if(index != NOT_FOUND)
	initOperandLabel(index, &pgm->stmts[i].op2);
      else
	operandNotUsed(&pgm->stmts[i].op2);
    }
  }

}
