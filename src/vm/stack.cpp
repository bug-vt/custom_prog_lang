#include "stack.hpp"

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

// stack interface
int Stack::resolveIndex (int index)
{
  return index < 0 ? frame_index + index : index;
}

Value Stack::getValue (int index)
{
  return stack[ resolveIndex (index) ];
}

void Stack::setValue (int index, Value val)
{
  stack[ resolveIndex (index) ] = val;
}

void Stack::push (Value val)
{
  stack[top_index] = val;
  top_index++;
}

Value Stack::pop ()
{
  top_index--;
  return stack[top_index];
}

void Stack::pushFrame (int size)
{
  top_index += size;
  // record the current stack frame
  prev_frame_index = frame_index;
  // change to new stack frame
  frame_index = top_index;
}

void Stack::popFrame (int size)
{
  // silently pop any values pushed above current stack frame
  top_index = frame_index;
  // pop the current stack frame
  top_index -= size;
  // restore to previous stack frame
  frame_index = prev_frame_index;
}
