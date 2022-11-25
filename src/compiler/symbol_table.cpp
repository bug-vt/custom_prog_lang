#include "symbol_table.hpp"
#include <iostream>

using std::string;
using std::unordered_map;
using std::cout;
using std::endl;


Symbol::Symbol (int type, int size) 
{
  this->type = type;
  this->size = size;
}

SymbolTable::SymbolTable () : symbol_table (), 
                              enclosing (nullptr), scope (0)
{
}

SymbolTable::SymbolTable (SymbolTable* enclosing, int scope) : symbol_table ()
{
  this->enclosing = enclosing;
  this->scope = scope;
}

// Add symbol to the symbol table.
// If the symbol is already exists inside the table, throw exception.
// Otherwise, return the assigned scope of the symbol
int SymbolTable::addSymbol (string name)
{
  // check if given function is already inside the table.
  if (symbol_table.count (name))
  {
    string msg = "Identifier with '" + name + "' already exists inside the same scope";
    throw std::runtime_error (msg);
  }

  // add the given symbol into the table.
  symbol_table[name] = Symbol (SYMBOL_TYPE_VAR, 1);

  return scope;
}

Symbol SymbolTable::getSymbol (string name)
{
  // see if the symbol is defined in local scope
  if (symbol_table.count (name))
    return symbol_table.at (name);

  if (enclosing)
    return enclosing->getSymbol (name);

  string msg = "Undefined variable '" + name + "'";
  throw std::runtime_error (msg);
}

bool SymbolTable::isSymbol (string name)
{
  // see if the symbol was declared
  if (symbol_table.count (name))
    return true;

  if (enclosing)
    return enclosing->isSymbol (name);

  return false;
}

int SymbolTable::getScope (string name)
{
  // see if the symbol is defined in local scope
  if (symbol_table.count (name))
    return scope;

  if (enclosing)
    return enclosing->getScope (name);

  string msg = "Undefined variable '" + name + "'";
  throw std::runtime_error (msg);
}

int SymbolTable::getSize (string name)
{
  return getSymbol (name).size;
}

void SymbolTable::print ()
{
  cout << "---------------------------------------" << endl;
  for (auto const& x : symbol_table)
  {
    std::cout << x.first << std::endl;
  }
  cout << "---------------------------------------" << endl;
}
