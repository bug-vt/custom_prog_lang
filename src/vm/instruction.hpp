#ifndef _INSTRUCTION
#define _INSTRUCTION

#include <string>
#include <vector>

struct Value
{
  int type;
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
