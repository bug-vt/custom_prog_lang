#ifndef _SCRIPT
#define _SCRIPT

#include "instruction.hpp"
#include <fstream>

struct RuntimeStack
{
  Value *elem;
  int size;
  int top_index;
  int frame_index;
};

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

    // register
    Value ret_val;

    // instruction stream
    std::vector<Instr> instr_stream;
    int instr_ptr;

    // runtime stack
    RuntimeStack stack;

    // string table
    std::vector<std::string> str_table;

    // function table
    std::vector<Func> func_table;

    void loadHeader (std::ifstream &binary);
    void loadInstrStream (std::ifstream &binary);
    void loadStringTable (std::ifstream &binary);
    void loadFuncTable (std::ifstream &binary);
};

#endif
