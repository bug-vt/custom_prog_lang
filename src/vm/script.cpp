#include "script.hpp"
#include "loader.hpp"
#include <fstream>
#include <iostream>
#include <cmath>

using std::string;
using std::ifstream;



Script::Script ()
{
  // register functions to the instruction handler table
  instr_handler[INSTR_MOV] = &Script::instrMov;
  instr_handler[INSTR_REF] = &Script::instrRef;
  instr_handler[INSTR_LW] = &Script::instrLw;
  instr_handler[INSTR_SW] = &Script::instrSw;
  instr_handler[INSTR_ADD] = &Script::instrArithmetic;
  instr_handler[INSTR_SUB] = &Script::instrArithmetic;
  instr_handler[INSTR_MUL] = &Script::instrArithmetic;
  instr_handler[INSTR_DIV] = &Script::instrArithmetic;
  instr_handler[INSTR_MOD] = &Script::instrArithmetic;
  instr_handler[INSTR_EXP] = &Script::instrArithmetic;
  instr_handler[INSTR_NEG] = &Script::instrArithmetic;
  instr_handler[INSTR_INC] = &Script::instrArithmetic;
  instr_handler[INSTR_DEC] = &Script::instrArithmetic;
  instr_handler[INSTR_AND] = &Script::instrBitwise;
  instr_handler[INSTR_OR] = &Script::instrBitwise;
  instr_handler[INSTR_XOR] = &Script::instrBitwise;
  instr_handler[INSTR_NOT] = &Script::instrBitwise;
  instr_handler[INSTR_SHL] = &Script::instrBitwise;
  instr_handler[INSTR_SHR] = &Script::instrBitwise;
  instr_handler[INSTR_CONCAT] = &Script::instrConcat;
  instr_handler[INSTR_GETCHAR] = &Script::instrGetChar;
  instr_handler[INSTR_SETCHAR] = &Script::instrSetChar;
  instr_handler[INSTR_SEQ] = &Script::instrCmp;
  instr_handler[INSTR_SNE] = &Script::instrCmp;
  instr_handler[INSTR_SGT] = &Script::instrCmp;
  instr_handler[INSTR_SLT] = &Script::instrCmp;
  instr_handler[INSTR_SGE] = &Script::instrCmp;
  instr_handler[INSTR_SLE] = &Script::instrCmp;
  instr_handler[INSTR_JMP] = &Script::instrJmp;
  instr_handler[INSTR_JE] = &Script::instrBranch;
  instr_handler[INSTR_PUSH] = &Script::instrPush;
  instr_handler[INSTR_POP] = &Script::instrPop;
  instr_handler[INSTR_CALL] = &Script::instrCall;
  instr_handler[INSTR_RET] = &Script::instrRet;
  instr_handler[INSTR_PAUSE] = &Script::instrPause;
  instr_handler[INSTR_EXIT] = &Script::instrExit;
  instr_handler[INSTR_PRINT] = &Script::instrPrint;
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

void Script::reset (int argc, char **argv)
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

  // reserve bottom of the stack for global variables.
  // allocate additional dummy spot at the bottom-most (index 0) stack,
  // since global variable indexing start at 1.
  stack.pushFrame (global_data_size + 1);
  // now, push stack frame for main:
  // first, push command line arguments if any.
  // arguments start from index 2 (0=vm, 1=exec file) 
  if (argc > 2)
  {
    // push each argument from right to left
    int str_index = str_table.size ();
    for (int i = argc - 1; i > 1; i--)
    {
      str_table[str_index + i] = string (argv[i]);
      Value arg;
      arg.type = OP_TYPE_STR;
      arg.string_index = str_index + i;
      stack.push (arg);
    }

    // push argument count
    Value arg_count;
    arg_count.type = OP_TYPE_INT;
    arg_count.int_literal = argc - 2;
    stack.push (arg_count);
  }
  // then, push remaining frame (local variables, return address, function index)
  Func main_func = func_table.at (main_func_index);
  stack.pushFrame (main_func.local_data_size, -1, main_func_index);
}

void Script::execute ()
{
  while (instr_index >= 0)
  {
    int curr_instr_index = instr_index;
    int opcode = instr_stream.at (instr_index).opcode;
    
    // invoke appropriate function from table, index by opcode 
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

void Script::instrRef ()
{
  Value ref;
  ref.type = OP_TYPE_INT;
  // record the stack index (referencing from the bottom) of the given variable
  ref.int_literal = stack.resolveIndex (resolveOpStackIndex (1));
  resolveOpCopy (0, ref);
}

void Script::instrLw ()
{
  // load the value located at the given stack index
  int abs_index = resolveOpAsInt (1) + resolveOpAsInt (2);
  resolveOpCopy (0, stack.getValue (abs_index));
}

void Script::instrSw ()
{
  // load the value located at the given stack index
  int abs_index = resolveOpAsInt (0) + resolveOpAsInt (1);
  stack.setValue (abs_index, resolveOpValue (2));
}

void Script::instrArithmetic ()
{
  int opcode = instr_stream.at (instr_index).opcode;

  // make a local copy of destination operand
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
    throw std::runtime_error ("Unsupported arithmetic operand type");

  // write the result to destination operand
  resolveOpCopy (0, dest);
}

void Script::instrBitwise ()
{
  int opcode = instr_stream.at (instr_index).opcode;
  
  // make a local copy of destination operand
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
    throw std::runtime_error ("Unsupported bitwise operand type");
  
  // write the result to destination operand
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

void Script::instrCmp ()
{
  int opcode = instr_stream.at (instr_index).opcode;

  Value dest = resolveOpValue (0);
  Value op1 = resolveOpValue (1);
  
  // comparison between operand 1 and operand 2
  bool condition = false;
  if (op1.type == OP_TYPE_INT)
  {
    switch (opcode)
    {
      case INSTR_SEQ:
        condition = op1.int_literal == resolveOpAsInt (2);
        break;
      case INSTR_SNE:
        condition = op1.int_literal != resolveOpAsInt (2);
        break;
      case INSTR_SGT:
        condition = op1.int_literal > resolveOpAsInt (2);
        break;
      case INSTR_SLT:
        condition = op1.int_literal < resolveOpAsInt (2);
        break;
      case INSTR_SGE:
        condition = op1.int_literal >= resolveOpAsInt (2);
        break;
      case INSTR_SLE:
        condition = op1.int_literal <= resolveOpAsInt (2);
        break;
    }
  }
  else if (op1.type == OP_TYPE_FLOAT)
  {
    switch (opcode)
    {
      case INSTR_SEQ:
        condition = op1.float_literal == resolveOpAsFloat (2);
        break;
      case INSTR_SNE:
        condition = op1.float_literal != resolveOpAsFloat (2);
        break;
      case INSTR_SGT:
        condition = op1.float_literal > resolveOpAsFloat (2);
        break;
      case INSTR_SLT:
        condition = op1.float_literal < resolveOpAsFloat (2);
        break;
      case INSTR_SGE:
        condition = op1.float_literal >= resolveOpAsFloat (2);
        break;
      case INSTR_SLE:
        condition = op1.float_literal <= resolveOpAsFloat (2);
        break;
    }
  }
  else if (op1.type == OP_TYPE_STR)
  {
    string op1_str = str_table.at (op1.string_index);
    switch (opcode)
    {
      case INSTR_SEQ:
        condition = op1_str == resolveOpAsString (2);
        break;
      case INSTR_SNE:
        condition = op1_str != resolveOpAsString (2);
        break;
      case INSTR_SGT:
        condition = op1_str > resolveOpAsString (2);
        break;
      case INSTR_SLT:
        condition = op1_str < resolveOpAsString (2);
        break;
      case INSTR_SGE:
        condition = op1_str >= resolveOpAsString (2);
        break;
      case INSTR_SLE:
        condition = op1_str <= resolveOpAsString (2);
        break;
    }
  }
  else
    throw std::runtime_error ("Unsupported relational operand type");
  
  dest.type = OP_TYPE_INT; 
  dest.int_literal = condition;
  // write the result to destination operand
  resolveOpCopy (0, dest);
}

void Script::instrJmp ()
{
  int target_index = resolveOpAsInstrIndex (0);
 
  // make a jump to target index
  instr_index = target_index;
}

void Script::instrBranch ()
{
  int opcode = instr_stream.at (instr_index).opcode;

  Value op0 = resolveOpValue (0);
  int target_index = resolveOpAsInstrIndex (2);
  
  // comparison between operand 0 and operand 1
  bool branch = false;
  if (op0.type == OP_TYPE_INT)
  {
    switch (opcode)
    {
      case INSTR_JE:
        branch = op0.int_literal == resolveOpAsInt (1);
        break;
    }
  }
  else if (op0.type == OP_TYPE_FLOAT)
  {
    switch (opcode)
    {
      case INSTR_JE:
        branch = op0.float_literal == resolveOpAsFloat (1);
        break;
    }
  }
  else if (op0.type == OP_TYPE_STR)
  {
    string op0_str = str_table.at (op0.string_index);
    switch (opcode)
    {
      case INSTR_JE:
        branch = op0_str == resolveOpAsString (1);
        break;
    }
  }
  else
    throw std::runtime_error ("Unsupported relational operand type");

  // make a jump if comparison between operand 0 and operand 1 is true
  if (branch)
    instr_index = target_index;
}

void Script::instrPush ()
{
  stack.push (resolveOpValue (0));
}

void Script::instrPop ()
{
  resolveOpCopy (0, stack.pop ());
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


