#include "script.hpp"
#include "loader.hpp"
#include <fstream>
#include <iostream>

using std::vector;
using std::string;
using std::ifstream;
using std::stoi;
using std::stof;
using std::to_string;

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


void Script::load (string file_name)
{
  ifstream binary (file_name);
  if (binary.fail ())
    throw std::runtime_error ("File not found");

  Loader loader (binary);
  // load header
  loader.loadHeader (stack, global_data_size, is_main_func_present, main_func_index);
  
  // load instruction stream
  loader.loadInstrStream (instr_stream);

  // load string table
  loader.loadStringTable (str_table);

  // load function table
  loader.loadFuncTable (func_table);

 
  binary.close ();
}

void Script::reset ()
{
  // set instruction pointer to main function's entry point
  if (func_table.size () > 0)
  {
    if (is_main_func_present)
      instr_index = func_table.at (main_func_index).entry_point;
    else
      throw std::runtime_error ("No main function found");
  }
 
  stack.reset ();

  // reserve bottom of the stack for global variables
  stack.pushFrame (global_data_size);
  // now, Push stack frame for main:
  Func main_func = func_table.at (main_func_index);
  stack.pushFrame (main_func.local_data_size, -1, main_func_index);
}

void Script::execute ()
{
  while (instr_index >= 0)
  {
    int curr_instr_index = instr_index;
    switch (instr_stream[instr_index].opcode)
    {
      case INSTR_MOV:
        resolveOpCopy (0, resolveOpValue (1));
        break;

      case INSTR_ADD:
        {
          Value dest = resolveOpValue (0);
          if (dest.type == OP_TYPE_INT)
            dest.int_literal += resolveOpAsInt (1);
          else
            dest.float_literal += resolveOpAsFloat (1);
          resolveOpCopy (0, dest);
        }
        break;

      case INSTR_PUSH:
        stack.push (resolveOpValue (0));
        break;

      case INSTR_CALL:
        {
          int func_index = resolveOpAsFuncIndex (0);
          Func func = func_table.at (func_index);
          
          // when function return, it instruction pointer should
          // points to the instruction that immediately following the call
          instr_index++;

          // now, Push stack frame for main:
          stack.pushFrame (func.local_data_size, instr_index, func_index);

          // jump to function's entry point
          instr_index = func.entry_point;
        }
        break;

      case INSTR_RET:
        {
          // obtain current function info by peeking the top of the stack frame
          Value frame_top = stack.getValue (-1); 
          int func_index = frame_top.func_index;
          Func curr_func = func_table.at (func_index);
          
          // obtain position of the return address inside stack frame
          // -1(function index) - local data - 1(return address)
          Value ret_addr = stack.getValue (-(curr_func.local_data_size + 2));
          // pop the stack frame
          stack.popFrame (curr_func.stack_frame_size);
          // jump to return address 
          instr_index = ret_addr.instr_index;
        }
        break;

      case INSTR_PRINT:
        std::cout << resolveOpAsString (0);
        break;

      default:
        break;
    }
    // move to next instruction if no jump or function call was made 
    if (curr_instr_index == instr_index)
      instr_index++;
  }
}


// ---------------------------------------------------------
// internal interface for VM

int Script::resolveOpStackIndex (int op_index)
{
  int stack_index = instr_stream[instr_index].op_list[op_index].stack_index;
  int offset_index = instr_stream[instr_index].op_list[op_index].offset_index;
  int offset = 0;
  offset = stack.getValue (offset_index).int_literal;

  return stack_index + offset;
}

Value Script::resolveOpValue (int op_index)
{
  Value op = instr_stream[instr_index].op_list[op_index]; 
  switch (op.type)
  {
    case OP_TYPE_ABS_STACK_INDEX:
      {
        int abs_index = instr_stream[instr_index].op_list[op_index].stack_index;
        return stack.getValue (abs_index);
      }
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
      {
        int abs_index = instr_stream[instr_index].op_list[op_index].stack_index;
        stack.setValue (abs_index, val);
      }
      break;
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
