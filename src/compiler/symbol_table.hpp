#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <unordered_map>


struct Symbol
{
  int size;           // 1 for variable, n for arrays
  bool is_ref;

  Symbol () { }
  Symbol (int size, bool is_ref);
};


class SymbolTable
{
  public:
    SymbolTable ();
    SymbolTable (SymbolTable* enclosing, int scope);
    int addSymbol (std::string name, int size, bool is_ref);
    Symbol getSymbol (std::string name);
    bool isSymbol (std::string name);
    int getScope (std::string name);
    int getSize (std::string name);
    bool isRef (std::string name);
    void print ();

  private:
    std::unordered_map<std::string, Symbol> symbol_table;
    SymbolTable* enclosing;
    int scope;
};


#endif
