#include "stack.hpp"

using std::vector;

Stack::Stack (int size)
{
  stack = vector<Value> (size);
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

Value* Stack::getValuePtr (int index)
{
  return &stack[ resolveIndex (index) ];
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
  frame_index = top_index;
}

void Stack::popFrame (int size)
{
  top_index -= size;
}
