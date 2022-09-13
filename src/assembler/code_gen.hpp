#ifndef _CODE_GEN
#define _CODE_GEN

#include "symbol_table.hpp"
#include "label_table.hpp"
#include "func_table.hpp"
#include "string_table.hpp"
#include "instruction.hpp"
#include <fstream>
#include <vector>

struct Header
{
  char ver_major;
  char ver_minor;
  int stack_size;
  int global_data_size;
  char is_main_func_present;
  int main_func_index;
};

class CodeGen
{
  public:
    CodeGen (std::ostream &out_file);
    void setVersion (char ver_major, char ver_minor);

    void writeHeader ();
    void writeSymbolTable ();
    void writeLabelTable ();
    void writeFuncTable ();
    void writeStringTable ();
    void writeInstrStream ();

  private:
    std::ostream &output;

    Header header;
    SymbolTable sym_table;
    LabelTable label_table;
    FuncTable func_table;
    StringTable str_table;

    std::vector<Instr> instr_stream;

    friend class AsmParser;
};

#endif
