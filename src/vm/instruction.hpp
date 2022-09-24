#ifndef _INSTRUCTION
#define _INSTRUCTION

#include <string>
#include <vector>

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
