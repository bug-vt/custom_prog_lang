#ifndef _DECODER
#define _DECODER

#include <fstream>
#include <iostream>
#include <sstream>
#include "../instruction.hpp"


struct TestHeader
{
  char ver_major;
  char ver_minor;
  int stack_size;
  int global_data_size;
  char is_main_func_present;
  int main_func_index;
};

struct TestOp
{
    OpType type;
    union
    {
        int int_literal;
        float float_literal;
        int str_table_index;    // string table index
        int instr_index;        // instruction stream index
        int stack_index;        // stack index
        int func_index;         // function table index
        int reg;                // register code
    };
    int offset_index;           // stack index of the offset variable
};

struct Decoder
{
  Decoder () { }
  static std::string readHeader (std::ifstream &binary);
  static std::string readInstrStream (std::ifstream &binary);
  static std::string readStringTable (std::ifstream &binary);
  static std::string readFuncTable (std::ifstream &binary);
};


#endif
