#include "func_table.hpp"
#include <iostream>

using std::vector;
using std::string;

FuncInfo::FuncInfo ()
{
}

FuncInfo::FuncInfo (string name, int entry_pos)
{
  func_name = name;
  entry_point = entry_pos;
  param_count = 0;
  local_data_size = 0;
}

// Start the function count from 1, since we reserve 0 for global scope
FuncTable::FuncTable () : func_count (1), func_table ()
{
  // put place holder for global scope at index 0
  FuncInfo func;
  func.entry_point = -1;
  func.param_count = -1;
  func.local_data_size = -1;
  func_table.push_back (func);
}

// Add function to the function table.
// If the function is already exists inside the table, return -1.
// Otherwise, return the assigned index that would use to locate 
// the function inside the table.
int FuncTable::addFunc (string func_name, int entry_point)
{
  // check if given function is already inside the table.
  if (getFuncIndex (func_name) != -1)
  {
    return -1;
  }

  // add the given function into the table.
  int curr_count = func_count;
  FuncInfo curr_func (func_name, entry_point);
  func_table.push_back (curr_func);
  func_count++;

  return curr_count;
}

// Return function information for the given function name.
// Return by value, so that caller get copy of FuncInfo from the function table,
// which ensure that caller cannot modify what is inside the table.
// Shallow copy is acceptable in this case since FuncInfo does not contains
// pointers/references/objects.
FuncInfo FuncTable::getFunc (string func_name)
{
  int func_index = getFuncIndex (func_name);
  return func_table.at (func_index);
}

int FuncTable::getFuncIndex (string func_name)
{
  int func_index = -1;
  for (int i = 0; i < func_table.size (); i++)
  {
    if (func_table[i].func_name == func_name)
    {
      func_index = i;
      break;
    } 
  }

  return func_index;
}

void FuncTable::setFunc (int func_index, int param_count, int local_data_size)
{
  func_table.at (func_index).param_count = param_count;
  func_table.at (func_index).local_data_size = local_data_size;
}

FuncInfo FuncTable::at (int index)
{
  return func_table.at (index);
}

int FuncTable::size ()
{
  return func_table.size ();
}

void FuncTable::print ()
{
  std::cout << "------------------------------------------------" << std::endl;
  for (int i = 0; i < func_table.size (); i++)
  {
    std::cout << func_table[i].func_name << std::endl;
  }
  std::cout << "------------------------------------------------" << std::endl;
}
