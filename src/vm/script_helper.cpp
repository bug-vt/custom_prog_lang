#include "script.hpp"
#include <iostream>

using std::string;
using std::stoi;
using std::stof;
using std::to_string;

// ---------------------------------------------------------
// internal interface for VM

int Script::resolveOpStackIndex (int op_index)
{
  Value op = instr_stream[instr_index].op_list[op_index]; 
  int stack_index = op.stack_index;
  int offset_index = op.offset_index;
  int offset = 0;
  if (offset_index != 0)
    offset = stack.getValue (offset_index).int_literal;

  return stack_index + offset;
}

Value Script::resolveOpValue (int op_index)
{
  Value op = instr_stream[instr_index].op_list[op_index]; 
  switch (op.type)
  {
    case OP_TYPE_ABS_STACK_INDEX:
    case OP_TYPE_REL_STACK_INDEX:
      {
        int abs_index = resolveOpStackIndex (op_index);
        return stack.getValue (abs_index);
      }
    case OP_TYPE_REG:
      return ret_val;

    default:
      return op;
  }
}

int Script::coerceToInt (Value val)
{
  switch (val.type)
  {
    case OP_TYPE_INT:
      return val.int_literal;
    case OP_TYPE_FLOAT:
      return (int) val.float_literal;
    case OP_TYPE_STR:
      return stoi (str_table.at (val.string_index));
    default:
      return 0;
  }
}

float Script::coerceToFloat (Value val)
{
  switch (val.type)
  {
    case OP_TYPE_INT:
      return (float) val.int_literal;
    case OP_TYPE_FLOAT:
      return val.float_literal;
    case OP_TYPE_STR:
      return stof (str_table.at (val.string_index));
    default:
      return 0;
  }
}

string Script::coerceToString (Value val)
{
  switch (val.type)
  {
    case OP_TYPE_INT:
      return to_string (val.int_literal);
    case OP_TYPE_FLOAT:
      return to_string (val.float_literal);
    case OP_TYPE_STR:
      return str_table.at (val.string_index);
    default:
      return "";
  }
}

int Script::resolveOpAsInt (int op_index)
{
  Value op = resolveOpValue (op_index);
  return coerceToInt (op);
}

float Script::resolveOpAsFloat (int op_index)
{
  Value op = resolveOpValue (op_index);
  return coerceToFloat (op);
}

string Script::resolveOpAsString (int op_index)
{
  Value op = resolveOpValue (op_index);
  return coerceToString (op);
}

int Script::resolveOpAsStackIndex (int op_index)
{
  return resolveOpValue (op_index).stack_index;
}

int Script::resolveOpAsInstrIndex (int op_index)
{
  return resolveOpValue (op_index).instr_index;
}

int Script::resolveOpAsFuncIndex (int op_index)
{
  return resolveOpValue (op_index).func_index;
}

int Script::resolveOpAsReg (int op_index)
{
  // register code 0 represent return value register
  return resolveOpValue (op_index).reg;
}

void Script::resolveOpCopy (int op_index, Value val)
{
  Value op = instr_stream[instr_index].op_list[op_index]; 
  switch (op.type)
  {
    case OP_TYPE_ABS_STACK_INDEX:
    case OP_TYPE_REL_STACK_INDEX:
      {
        int abs_index = resolveOpStackIndex (op_index);
        stack.setValue (abs_index, val);
      }
      break;
    case OP_TYPE_REG:
      ret_val = val;
      break;

    default:
      throw std::runtime_error ("Cannot write to non-destination operand"); 
  }
}
