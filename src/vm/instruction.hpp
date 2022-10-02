#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <string>
#include <vector>

enum InstrOpcode {INSTR_MOV,      // 0
                  INSTR_ADD,      // 1
                  INSTR_SUB,      // 2
                  INSTR_MUL,      // 3
                  INSTR_DIV,      // 4
                  INSTR_MOD,      // 5
                  INSTR_EXP,      // 6
                  INSTR_NEG,      // 7
                  INSTR_INC,      // 8
                  INSTR_DEC,      // 9
                  INSTR_AND,      // 10
                  INSTR_OR,       // 11
                  INSTR_XOR,      // 12
                  INSTR_NOT,      // 13
                  INSTR_SHL,      // 14
                  INSTR_SHR,      // 15
                  INSTR_CONCAT,   // 16
                  INSTR_GETCHAR,  // 17
                  INSTR_SETCHAR,  // 18
                  INSTR_JMP,      // 19
                  INSTR_JE,       // 20
                  INSTR_JNE,      // 21
                  INSTR_JG,       // 22
                  INSTR_JL,       // 23
                  INSTR_JGE,      // 24
                  INSTR_JLE,      // 25
                  INSTR_PUSH,     // 26
                  INSTR_POP,      // 27
                  INSTR_CALL,     // 28
                  INSTR_RET,      // 29
                  INSTR_PAUSE,    // 30
                  INSTR_EXIT,     // 31
                  INSTR_PRINT};   // 32


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
