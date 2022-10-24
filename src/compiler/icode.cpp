#include "icode.hpp"

ICode::ICode ()
{
  icode_count = 0;
}

int ICode::addInstr (int opcode)
{
  ICodeNode instr_node;
  instr_node.type = ICODE_NODE_INSTR;
  instr_node.instr.opcode = opcode;
  
  int curr_index = icode_count;
  icode_stream.push_back (instr_node);
  icode_count++;

  return curr_index;
}

void ICode::addOp (int instr_index, Op value)
{
  icode_stream.at (instr_index).instr.op_list.push_back (value);
}

void ICode::addIntOp (int instr_index, int int_literal)
{
  Op value;
  value.type = OP_TYPE_INT;
  value.int_literal = int_literal;

  addOp (instr_index, value);
}

void ICode::addVarOp (int instr_index, int symbol_index)
{
  Op value;
  value.type = OP_TYPE_VAR;
  value.symbol_index = symbol_index;

  addOp (instr_index, value);
}
