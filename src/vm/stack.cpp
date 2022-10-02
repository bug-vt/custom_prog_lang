#include "stack.hpp"
#include <iostream>

using std::vector;

Stack::Stack (int size)
{
  stack = vector<Value> (size);
  top_index = 0;
  frame_index = 0;
}

void Stack::reset ()
{
  top_index = 0;
  frame_index = 0;

  for (int i = 0; i < stack.size (); i++)
    stack[i].type = OP_TYPE_INVALID;
}

// calculate stack index
// case 1: index >= 0 -> offset from bottom of the stack
// case 2: index < 0  -> offset from top of the stack
int Stack::resolveIndex (int index)
{
  int resolved_index = index < 0 ? frame_index + index : index;
  if (resolved_index < 0 || resolved_index >= top_index)
  {
    std::cout << "Given index: " << index << std::endl;
    std::cout << "Resolved index: " << resolved_index << std::endl;
    std::cout << "Current top: " << top_index << std::endl;
    throw std::runtime_error ("Stack index out of bound");
  }

  return resolved_index;
}

Value Stack::getValue (int index)
{
  return stack.at (resolveIndex (index));
}

void Stack::setValue (int index, Value val)
{
  stack.at (resolveIndex (index)) = val;
}

void Stack::push (Value val)
{
  stack[top_index] = val;
  top_index++;
}

Value Stack::pop ()
{
  top_index--;
  return stack.at (top_index);
}

void Stack::pushFrame (int size)
{
  top_index += size;
  // change to new stack frame
  frame_index = top_index;
}

void Stack::pushFrame (int size, int ret_index, int func_index)
{
  // first, push return address 
  Value return_addr;
  return_addr.instr_index = ret_index;
  push (return_addr);

  // next, reserve space for local variables
  // and additional space for function index on the top.
  top_index += size + 1;
  // record the current stack frame
  int prev_frame_index = frame_index;
  // change to new stack frame
  frame_index = top_index;

  // finally, set function index on the top of the frame
  Value frame_top;
  frame_top.func_index = func_index;
  frame_top.offset_index = prev_frame_index;
  setValue (-1, frame_top);
}

void Stack::popFrame (int size)
{
  // silently pop any values pushed above current stack frame
  top_index = frame_index;
  // restore to previous stack frame
  frame_index = getValue (-1).offset_index;
  // pop the current stack frame
  top_index -= size;
}
