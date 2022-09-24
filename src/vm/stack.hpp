#ifndef _STACK
#define _STACK

#include "instruction.hpp"
#include <vector>

class Stack
{
  public:
    Stack () { }
    Stack (int size);
    void reset ();
    int resolveIndex (int index);
    Value getValue (int index);
    Value *getValuePtr (int index);
    void setValue (int index, Value val);
    void push (Value val);
    Value pop ();
    void pushFrame (int size);
    void popFrame (int size);

  private:
    std::vector<Value> stack;
    int top_index;
    int frame_index;
};

#endif
