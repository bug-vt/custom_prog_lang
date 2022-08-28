#include "func_table.hpp"
#include <cassert>

using std::unordered_map;
using std::string;

FuncNode::FuncNode ()
{
}

FuncNode::FuncNode (int index, int entry_pos)
{
  func_index = index;
  entry_point = entry_pos;
  param_count = 0;
  local_data_size = 0;
}


FuncTable::FuncTable () : func_index (0), func_table ()
{
}


// Add function to the function table.
// If the function is already inside the table, return -1.
// Otherwise, return the assigned index that would use to locate 
// the function inside the table.
int FuncTable::addFunc (string func_name, int entry_point)
{
  // check if given function is already inside the table.
  if (func_table.find (func_name) != func_table.end ())
    return -1;

  // add the given function into the table.
  int curr_index = func_index;
  FuncNode curr_func (curr_index, entry_point);
  func_table[func_name] = curr_func;
  func_index++;

  return curr_index;
}

// return function information for the given function name.
void FuncTable::getFunc (string func_name)
{
  return func_table[func_name];
}

void FuncTable::setFunc (string func_name, int param_count, int local_data_size)
{
  // make sure the given function is already inside the table.
  assert (func_table.find (func_name) != func_table.end ());

  func_table[func_name].param_count = param_count;
  func_table[func_name].local_data_size = local_data_size;
}

