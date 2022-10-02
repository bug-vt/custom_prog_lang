#include "script.hpp"
#include "loader.hpp"
#include <fstream>
#include <iostream>

using std::string;
using std::ifstream;



Script::Script ()
{
  instr_handler[0] = &Script::instrMov;
  instr_handler[1] = &Script::instrAdd;
  //instr_handler[2] = sub;
  //instr_handler[3] = instrMul;
  //instr_handler[4] = instrDiv;
  //instr_handler[5] = instrMod;
  //instr_handler[6] = instrExp;
  //instr_handler[7] = instrNeg;
  //instr_handler[8] = instrInc;
  //instr_handler[9] = instrDec;
  //instr_handler[10] = instrAnd;
  //instr_handler[11] = instrOr;
  //instr_handler[12] = instrXor;
  //instr_handler[13] = instrNot;
  //instr_handler[14] = instrShl;
  //instr_handler[15] = instrShr;
  //instr_handler[16] = instrConcat;
  //instr_handler[17] = instrGetChar;
  //instr_handler[18] = instrSetChar;
  //instr_handler[19] = instrJmp;
  //instr_handler[20] = instrJe;
  //instr_handler[21] = instrJne;
  //instr_handler[22] = instrJg;
  //instr_handler[23] = instrJl;
  //instr_handler[24] = instrJge;
  //instr_handler[25] = instrJle;
  instr_handler[26] = &Script::instrPush;
  //instr_handler[27] = instrPop;
  instr_handler[28] = &Script::instrCall;
  instr_handler[29] = &Script::instrRet;
  //instr_handler[30] = instrPause;
  //instr_handler[31] = instrExit;
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

void Script::instrAdd ()
{
  Value dest = resolveOpValue (0);
  if (dest.type == OP_TYPE_INT)
    dest.int_literal += resolveOpAsInt (1);
  else
    dest.float_literal += resolveOpAsFloat (1);
  resolveOpCopy (0, dest);
}

void Script::instrPush ()
{
  stack.push (resolveOpValue (0));
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

void Script::instrPrint ()
{
  std::cout << resolveOpAsString (0);
}


