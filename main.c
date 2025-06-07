#include <stdio.h>

const size_t VAR_STORAGE_SIZE = 1000;
const size_t COMMAND_MAX_ARG_AMOUNT = 5;

typedef enum VarType_ {
    DELETED,
    NULL_,
    INTEGER,
    FLOAT,
    OBJECT
} VarType;

typedef struct _Var {
    VarType type; // 8 bits
    unsigned long int data; // 32 bits
} Var;

struct CompileTime {
    
} comptime;

struct Runtime {
    size_t nVars;
    Var vars[VAR_STORAGE_SIZE];
} runtime;

typedef enum CommandType_ {
    CREATE_VAR
} CommandType;

typedef struct _Command {
    size_t nArgs;
    int args[COMMAND_MAX_ARG_AMOUNT];
    CommandType type;
} Command;

size_t Runtime_createVar() {

}

int main(void) {

}