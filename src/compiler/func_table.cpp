#include "func_table.hpp"
#include <iostream>

using std::string;
using std::unordered_map;
using std::cout;
using std::endl;


FuncTable::FuncTable () : func_table ()
{
}

// Add function to the function table.
// If function is already exists inside the table, throw exception.
void FuncTable::addFunc (string name, int param_count)
{
  // check if given function is already inside the table.
  if (func_table.count (name))
  {
    string msg = "Function with '" + name + "' already exists";
    throw std::runtime_error (msg);
  }

  // add the given function into the table.
  func_table[name] = param_count;
}

int FuncTable::getFunc (string name)
{
  // see if the function was declared
  if (func_table.count (name))
    return func_table.at (name);

  string msg = "Undefined function '" + name + "'";
  throw std::runtime_error (msg);
}

bool FuncTable::isFunc (string name)
{
  // see if the function was declared
  if (func_table.count (name))
    return true;

  return false;
}

void FuncTable::print ()
{
  cout << "---------------------------------------" << endl;
  for (auto const& x : func_table)
  {
    std::cout << x.first << std::endl;
  }
  cout << "---------------------------------------" << endl;
}
