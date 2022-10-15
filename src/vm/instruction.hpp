#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>
#include <vector>

enum InstrOpcode {INSTR_MOV,      // 0
                  INSTR_MEM,      // 1
                  INSTR_REF,      // 2
                  INSTR_ADD,      // 3
                  INSTR_SUB,      // 4
                  INSTR_MUL,      // 5
                  INSTR_DIV,      // 6
                  INSTR_MOD,      // 7
                  INSTR_EXP,      // 8
                  INSTR_NEG,      // 9
                  INSTR_INC,      // 10
                  INSTR_DEC,      // 11
                  INSTR_AND,      // 12
                  INSTR_OR,       // 13
                  INSTR_XOR,      // 14
                  INSTR_NOT,      // 15
                  INSTR_SHL,      // 16
                  INSTR_SHR,      // 17
                  INSTR_CONCAT,   // 18
                  INSTR_GETCHAR,  // 19
                  INSTR_SETCHAR,  // 20 
                  INSTR_JMP,      // 21 
                  INSTR_JE,       // 22
                  INSTR_JNE,      // 23
                  INSTR_JG,       // 24
                  INSTR_JL,       // 25
                  INSTR_JGE,      // 26
                  INSTR_JLE,      // 27
                  INSTR_PUSH,     // 28
                  INSTR_POP,      // 29
                  INSTR_CALL,     // 30
                  INSTR_RET,      // 31
                  INSTR_PAUSE,    // 32
                  INSTR_EXIT,     // 33
                  INSTR_PRINT};   // 34


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
