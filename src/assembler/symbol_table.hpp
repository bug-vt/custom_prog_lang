#ifndef _SYMBOL_TABLE
#define _SYMBOL_TABLE

#include <string>
#include <unordered_map>

struct Symbol
{
  std::string ident;
  int func_index;     // function in which the symbol resides

  Symbol () { }
  Symbol (std::string ident, int func_index);
  bool operator== (const Symbol &other) const
  {
    return ident == other.ident && func_index == other.func_index;
  }
};

// To use user-defined key type for unordered_map,
// hash function must overrides operator() and calculates the hash value
// for a given key type.
namespace std 
{
  template <>
  struct hash<Symbol>
  {
    std::size_t operator() (const Symbol &symbol) const
    {
      using std::size_t;
      using std::hash;
      using std::string;

      return ((hash<string>()(symbol.ident)
              ^ (hash<int>()(symbol.func_index) << 1)) >> 1);
    }
  };
}

struct SymbolInfo
{
  int symbol_index;   // index within the symbol table
  int size;           // 1 for variable, n for arrays
  int stack_index;    // the stack index to which the symbol points

  SymbolInfo () { }
  SymbolInfo (int index, int size, int stack);
};


class SymbolTable
{
  public:
    SymbolTable ();
    int addSymbol (Symbol symbol, int size, int stack_index);
    SymbolInfo getSymbol (Symbol symbol);
    int getStackIndex (Symbol symbol);
    void print ();

  private:
    int symbol_count;
    std::unordered_map<Symbol, SymbolInfo> symbol_table;
};


#endif
