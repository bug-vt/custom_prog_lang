#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <string>
#include <unordered_map>

enum SymbolType {SYMBOL_TYPE_VAR, SYMBOL_TYPE_PARAM};


struct Symbol
{
  int symbol_index;   // index within the symbol table
  int size;           // 1 for variable, n for arrays
  int type;           // type (parameter or variable) 

  Symbol () { }
  Symbol (int index, int size, int type);
};


class SymbolTable
{
  public:
    SymbolTable ();
    //int addSymbol (std::string name, int size, int type);
    int addSymbol (std::string name);
    Symbol getSymbol (std::string name);
    std::string at (int index);
    int getSize (std::string name);
    void print ();

  private:
    int symbol_count;
    std::unordered_map<std::string, Symbol> symbol_table;

  friend class CodeGen;
};


#endif
