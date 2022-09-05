#include "func_table.hpp"
#include <cassert>

using std::unordered_map;
using std::string;

FuncInfo::FuncInfo ()
{
}

FuncInfo::FuncInfo (int index, int entry_pos)
{
  func_index = index;
  entry_point = entry_pos;
  param_count = 0;
  local_data_size = 0;
}


FuncTable::FuncTable () : func_index (1), func_table ()
{
}

// Add function to the function table.
// If the function is already exists inside the table, return -1.
// Otherwise, return the assigned index that would use to locate 
// the function inside the table.
int FuncTable::addFunc (string func_name, int entry_point)
{
  // check if given function is already inside the table.
  try
  {
    getFunc (func_name);
    return -1;
  }
  catch (...) { }

  // add the given function into the table.
  int curr_index = func_index;
  FuncInfo curr_func (curr_index, entry_point);
  func_table[func_name] = curr_func;
  func_index++;

  return curr_index;
}

// Return function information for the given function name.
// Return by value, so that caller get copy of FuncInfo from the function table,
// which ensure that caller cannot modify what is inside the table.
// Shallow copy is acceptable in this case since FuncInfo does not contains
// pointers/references/objects.
FuncInfo FuncTable::getFunc (string func_name)
{
  return func_table.at (func_name);
}

void FuncTable::setFunc (int func_index, int param_count, int local_data_size)
{
  // make sure the given function is already inside the table.
  string func_name;
  for (const auto &iter : func_table)
  {
    if (iter.second.func_index == func_index)
    {
      func_name = iter.first;
      func_table[func_name].param_count = param_count;
      func_table[func_name].local_data_size = local_data_size;
      break;
    }
  }
}

