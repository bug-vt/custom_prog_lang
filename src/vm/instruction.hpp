#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>
#include <vector>

enum InstrOpcode {INSTR_MOV,      // 0
                  INSTR_REF,      // 1
                  INSTR_LW,       // 2
                  INSTR_SW,       // 3
                  INSTR_ADD,      // 4 
                  INSTR_SUB,      // 5 
                  INSTR_MUL,      // 6 
                  INSTR_DIV,      // 7 
                  INSTR_MOD,      // 8 
                  INSTR_EXP,      // 9 
                  INSTR_NEG,      // 10
                  INSTR_INC,      // 11 
                  INSTR_DEC,      // 12
                  INSTR_AND,      // 13
                  INSTR_OR,       // 14
                  INSTR_XOR,      // 15
                  INSTR_NOT,      // 16
                  INSTR_SHL,      // 17
                  INSTR_SHR,      // 18
                  INSTR_GETCHAR,  // 19
                  INSTR_SETCHAR,  // 20 
                  INSTR_SEQ,      // 21
                  INSTR_SNE,      // 22
                  INSTR_SGT,      // 23
                  INSTR_SLT,      // 24
                  INSTR_SGE,      // 25
                  INSTR_SLE,      // 26
                  INSTR_JMP,      // 27
                  INSTR_JE,       // 28
                  INSTR_PUSH,     // 29
                  INSTR_POP,      // 30
                  INSTR_CALL,     // 31
                  INSTR_RET,      // 32
                  INSTR_PAUSE,    // 33
                  INSTR_EXIT,     // 34
                  INSTR_PRINT,    // 35
                  INSTR_TIME};    // 36


enum OpType {OP_TYPE_INT,
             OP_TYPE_FLOAT,
             OP_TYPE_STR,
             OP_TYPE_ABS_STACK_INDEX,
             OP_TYPE_REL_STACK_INDEX,
             OP_TYPE_INSTR_INDEX,
             OP_TYPE_FUNC,
             OP_TYPE_REG,
             OP_TYPE_INVALID};

struct Value
{
  OpType type;
  union
  {
    int int_literal;
    float float_literal;
    int string_index;
    int stack_index;
    int instr_index;
    int func_index;
    int reg;
  };

  int offset_index;

};

struct Instr
{
  int opcode;
  std::vector<Value> op_list;
};

#endif
