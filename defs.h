#define MAX_STR       32
#define MAX_ARR_SIZE  32

// Instruction category: 
typedef enum { READ, PRINT, COMP, JMORE, JUMP, END } InstrCategory;

// Operand category:  variable, label, unresolved label, not used
typedef enum { VAR, LABEL, LABEL_NOT_FND, NOT_USED } OpCategory;

// Flag type:  used during program execution to hold comparison result
typedef enum { GT, LT, EQ, NOT_SET } FlagType;

// Variable type:  used to store the program variables 
typedef struct {
  char name[MAX_STR];
  int  value;
} VarType;

// Forward reference
struct Stmt;

// OperandType:  used to store an operand for a program statement
typedef struct {
  OpCategory opType;
  int label;
  int var;
  char tmpLabel[MAX_STR];
} OperandType;

// StmtType:  used to store a statement for a program
typedef struct Stmt {
  char label[MAX_STR];
  InstrCategory instr;
  OperandType op1; 
  OperandType op2; 
} StmtType;

// PgmType:  used to store information about a program
typedef struct {
  int numStmts;
  StmtType stmts[MAX_ARR_SIZE];
  int numVars;
  VarType vars[MAX_ARR_SIZE];
  FlagType flag;
} PgmType;


void printPgm(PgmType*);
void printPgmStmts(PgmType*);
void printPgmVars(PgmType*);

