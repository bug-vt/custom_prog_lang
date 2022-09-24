#ifndef _SCRIPT
#define _SCRIPT

#include "instruction.hpp"
#include <fstream>
#include <unordered_map>


struct Func
{
  int entry_point;
  int param_count;
  int local_data_size;
  int stack_frame_size;
};


class Script
{
  public:
    Script () { }
    void loadScript (std::string file_name);

  private:
    // header
    int global_data_size;
    char is_main_func_present;
    int main_func_index;

    // registers
    int instr_index;
    int frame_index;
    int stack_index;
    Value ret_val;

    // instruction stream
    std::vector<Instr> instr_stream;

    // runtime stack
    std::vector<Value> stack;

    // string table
    std::unordered_map<int, std::string> str_table;

    // function table
    std::unordered_map<int, Func> func_table;

    void loadHeader (std::ifstream &binary);
    void loadInstrStream (std::ifstream &binary);
    void loadStringTable (std::ifstream &binary);
    void loadFuncTable (std::ifstream &binary);
};

#endif
