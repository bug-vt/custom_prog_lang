#include "script.hpp"
#include "loader.hpp"
#include <fstream>
#include <iostream>
#include <cmath>

using std::string;
using std::ifstream;



Script::Script ()
{
  instr_handler[0] = &Script::instrMov;
  instr_handler[1] = &Script::instrArithmetic;
  instr_handler[2] = &Script::instrArithmetic;
  instr_handler[3] = &Script::instrArithmetic;
  instr_handler[4] = &Script::instrArithmetic;
  instr_handler[5] = &Script::instrArithmetic;
  instr_handler[6] = &Script::instrArithmetic;
  instr_handler[7] = &Script::instrArithmetic;
  instr_handler[8] = &Script::instrArithmetic;
  instr_handler[9] = &Script::instrArithmetic;
  instr_handler[10] = &Script::instrBitwise;
  instr_handler[11] = &Script::instrBitwise;
  instr_handler[12] = &Script::instrBitwise;
  instr_handler[13] = &Script::instrBitwise;
  instr_handler[14] = &Script::instrBitwise;
  instr_handler[15] = &Script::instrBitwise;
  instr_handler[16] = &Script::instrConcat;
  instr_handler[17] = &Script::instrGetChar;
  instr_handler[18] = &Script::instrSetChar;
  instr_handler[19] = &Script::instrJmp;
  instr_handler[20] = &Script::instrJe;
  instr_handler[21] = &Script::instrJne;
  instr_handler[22] = &Script::instrJg;
  instr_handler[23] = &Script::instrJl;
  instr_handler[24] = &Script::instrJge;
  instr_handler[25] = &Script::instrJle;
  instr_handler[26] = &Script::instrPush;
  instr_handler[27] = &Script::instrPop;
  instr_handler[28] = &Script::instrCall;
  instr_handler[29] = &Script::instrRet;
  instr_handler[30] = &Script::instrPause;
  instr_handler[31] = &Script::instrExit;
  instr_handler[32] = &Script::instrPrint;
}

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
    int opcode = instr_stream.at (instr_index).opcode;

    (*this.*instr_handler.at (opcode)) ();

    // move to next instruction if no jump or function call was made 
    if (curr_instr_index == instr_index)
      instr_index++;
  }
}

// ---------------------------------------------------------------
// instruction execution

void Script::instrMov ()
{
  resolveOpCopy (0, resolveOpValue (1));
}

void Script::instrArithmetic ()
{
  int opcode = instr_stream.at (instr_index).opcode;

  Value dest = resolveOpValue (0);
  if (dest.type == OP_TYPE_INT)
  {
    switch (opcode)
    {
      case INSTR_ADD:
        dest.int_literal += resolveOpAsInt (1);
        break;
      case INSTR_SUB:
        dest.int_literal -= resolveOpAsInt (1);
        break;
      case INSTR_MUL:
        dest.int_literal *= resolveOpAsInt (1);
        break;
      case INSTR_DIV:
        dest.int_literal /= resolveOpAsInt (1);
        break;
      case INSTR_MOD:
        dest.int_literal %= resolveOpAsInt (1);
        break;
      case INSTR_EXP:
        dest.int_literal = pow (dest.int_literal, resolveOpAsInt (1));
        break;
      case INSTR_NEG:
        dest.int_literal = -dest.int_literal;
        break;
      case INSTR_INC:
        dest.int_literal = dest.int_literal + 1;
        break;
      case INSTR_DEC:
        dest.int_literal = dest.int_literal - 1;
        break;
    }
  }
  else if (dest.type == OP_TYPE_FLOAT)
  {
    switch (opcode)
    {
      case INSTR_ADD:
        dest.float_literal += resolveOpAsFloat (1);
        break;
      case INSTR_SUB:
        dest.float_literal -= resolveOpAsFloat (1);
        break;
      case INSTR_MUL:
        dest.float_literal *= resolveOpAsFloat (1);
        break;
      case INSTR_DIV:
        dest.float_literal /= resolveOpAsFloat (1);
        break;
      case INSTR_MOD:
        dest.float_literal = fmod (dest.float_literal, resolveOpAsFloat (1));
        break;
      case INSTR_EXP:
        dest.float_literal = pow (dest.float_literal, resolveOpAsFloat (1));
        break;
      case INSTR_NEG:
        dest.float_literal = -dest.float_literal;
        break;
      case INSTR_INC:
        dest.float_literal = dest.float_literal + 1.0;
        break;
      case INSTR_DEC:
        dest.float_literal = dest.float_literal - 1.0;
        break;
    }
  }
  else
    throw std::runtime_error ("Unsupported destination operand type");

  resolveOpCopy (0, dest);
}

void Script::instrBitwise ()
{
  int opcode = instr_stream.at (instr_index).opcode;

  Value dest = resolveOpValue (0);
  if (dest.type == OP_TYPE_INT)
  {
    switch (opcode)
    {
      case INSTR_AND:
        dest.int_literal &= resolveOpAsInt (1);
        break;
      case INSTR_OR:
        dest.int_literal |= resolveOpAsInt (1);
        break;
      case INSTR_XOR:
        dest.int_literal ^= resolveOpAsInt (1);
        break;
      case INSTR_NOT:
        dest.int_literal = ~dest.int_literal;
        break;
      case INSTR_SHL:
        dest.int_literal <<= resolveOpAsInt (1);
        break;
      case INSTR_SHR:
        dest.int_literal >>= resolveOpAsInt (1);
        break;
    }
  }
  else
    throw std::runtime_error ("Unsupported destination operand type");

  resolveOpCopy (0, dest);
}

void Script::instrConcat ()
{
}

void Script::instrGetChar ()
{
}

void Script::instrSetChar ()
{
}

void Script::instrJmp ()
{
}

void Script::instrJe ()
{
}

void Script::instrJne ()
{
}

void Script::instrJg ()
{
}

void Script::instrJl ()
{
}

void Script::instrJge ()
{
}

void Script::instrJle ()
{
}

void Script::instrPush ()
{
  stack.push (resolveOpValue (0));
}

void Script::instrPop ()
{
}

void Script::instrCall ()
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

void Script::instrRet ()
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

void Script::instrPause ()
{
}

void Script::instrExit ()
{
}

void Script::instrPrint ()
{
  std::cout << resolveOpAsString (0);
}


