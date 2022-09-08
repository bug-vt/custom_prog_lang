#include "symbol_table.hpp"

using std::string;
using std::unordered_map;

Symbol::Symbol (string ident, int func_index)
{
  this->ident = ident;
  this->func_index = func_index;
}

SymbolInfo::SymbolInfo (int index, int size, int stack) 
{
  this->symbol_index = index;
  this->size = size;
  this->stack_index = stack;
}

SymbolTable::SymbolTable () : symbol_index (0), symbol_table ()
{
}

// Add symbol to the symbol table.
// If the symbol is already exists inside the table, return -1.
// Otherwise, return the assigned index that would use to locate 
// the symbol inside the table.
int SymbolTable::addSymbol (Symbol symbol, int size, int stack_index)
{
  // check if given function is already inside the table.
  if (symbol_table.find (symbol) != symbol_table.end ())
    return -1;

  // add the given function into the table.
  int curr_index = symbol_index;
  SymbolInfo curr_symbol (symbol_index, size, stack_index);
  symbol_table[symbol] = curr_symbol;
  symbol_index++;

  return curr_index;
  
}

SymbolInfo SymbolTable::getSymbol (Symbol symbol)
{
  return symbol_table.at (symbol);
}
