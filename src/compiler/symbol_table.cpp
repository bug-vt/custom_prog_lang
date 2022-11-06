#include "symbol_table.hpp"
#include <iostream>

using std::string;
using std::unordered_map;


Symbol::Symbol (int index, int size, int type) 
{
  this->symbol_index = index;
  this->size = size;
  this->type = type;
}

SymbolTable::SymbolTable () : symbol_count (0), symbol_table ()
{
}

// Add symbol to the symbol table.
// If the symbol is already exists inside the table, return -1.
// Otherwise, return the assigned index that would use to locate 
// the symbol inside the table.
int SymbolTable::addSymbol (string name)
{
  // check if given function is already inside the table.
  if (symbol_table.count (name))
    throw std::runtime_error ("Identifier with same name already exists inside the same scope");

  // add the given function into the table.
  int index = symbol_count;
  symbol_table[name] = Symbol (index, 1, SYMBOL_TYPE_VAR);
  symbol_count++;

  return index;
}

Symbol SymbolTable::getSymbol (string name)
{
  // see if the symbol is defined in local scope
  if (symbol_table.count (name) == 0)
  {
    string msg = "Undefined variable '" + name + "'";
    throw std::runtime_error (msg);
  }
        
  return symbol_table.at (name);
}

string SymbolTable::at (int index)
{
  for (auto const& x : symbol_table)
  {
    if (x.second.symbol_index == index)
      return x.first;
  }
  throw std::runtime_error ("No such symbol for given index");
}

int SymbolTable::getSize (string name)
{
  return getSymbol (name).size;
}

void SymbolTable::print ()
{
  std::cout << "---------------------------------------" << std::endl;
  for (auto const& x : symbol_table)
  {
    std::cout << x.first << std::endl;
  }
  std::cout << "---------------------------------------" << std::endl;
}
