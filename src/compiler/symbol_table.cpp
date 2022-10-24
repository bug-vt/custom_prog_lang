#include "symbol_table.hpp"
#include <iostream>

using std::string;
using std::unordered_map;

Symbol::Symbol (string ident, int func_index)
{
  this->ident = ident;
  this->func_index = func_index;
}

SymbolInfo::SymbolInfo (int index, int size, int type) 
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
int SymbolTable::addSymbol (Symbol symbol, int size, int type)
{
  // check if given function is already inside the table.
  if (symbol_table.count (symbol))
    return -1;

  // add the given function into the table.
  int curr_count = symbol_count;
  SymbolInfo curr_symbol (symbol_count, size, type);
  symbol_table[symbol] = curr_symbol;
  symbol_count++;

  return curr_count;
}

SymbolInfo SymbolTable::getSymbol (Symbol symbol)
{
  SymbolInfo found;
  // first, see if the symbol is defined in local scope
  if (symbol_table.find (symbol) != symbol_table.end ())
    found = symbol_table.at (symbol);
  // otherwise, see if the symbol is defined in global scope
  else 
    found = symbol_table.at (Symbol (symbol.ident, 0));

  return found;
}

string SymbolTable::at (int index)
{
  for (auto const& x : symbol_table)
  {
    if (x.second.symbol_index == index)
      return x.first.ident;
  }
  throw std::runtime_error ("No such symbol for given index");
}

int SymbolTable::getSize (Symbol symbol)
{
  int size;
  try
  {
    size = getSymbol (symbol).size;
  }
  catch (...)
  {
    size = -1;
  }
  return size;
}

void SymbolTable::print ()
{
  std::cout << "---------------------------------------" << std::endl;
  for (auto const& x : symbol_table)
  {
    std::cout << x.first.ident << " " << x.first.func_index << std::endl;
  }
  std::cout << "---------------------------------------" << std::endl;
}
