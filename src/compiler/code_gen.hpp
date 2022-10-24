#ifndef CODE_GEN_HPP
#define CODE_GEN_HPP

#include "symbol_table.hpp"
#include "func_table.hpp"
#include "string_table.hpp"
#include <fstream>
#include <vector>


class CodeGen
{
  public:
    CodeGen () {}
    void init (std::string input_file_name, std::string output_file_name);

    void write ();
    void writeHeader ();
    void writeScopeSymbols (int scope, int type);
    void writeFunc (FuncInfo func, int func_index);

  private:
    std::string input_file_name;
    std::string output_file_name;

    std::ofstream output;

    SymbolTable sym_table;
    FuncTable func_table;

    friend class Parser;
};

#endif
