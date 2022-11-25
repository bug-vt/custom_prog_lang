#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <unordered_map>


struct Symbol
{
  int type;           // type (parameter or variable) 
  int size;           // 1 for variable, n for arrays

  Symbol () { }
  Symbol (int type, int size);
};


class SymbolTable
{
  public:
    SymbolTable ();
    SymbolTable (SymbolTable* enclosing, int scope);
    int addSymbol (std::string name, int size);
    Symbol getSymbol (std::string name);
    bool isSymbol (std::string name);
    int getScope (std::string name);
    int getSize (std::string name);
    void print ();

  private:
    std::unordered_map<std::string, Symbol> symbol_table;
    SymbolTable* enclosing;
    int scope;
};


#endif
