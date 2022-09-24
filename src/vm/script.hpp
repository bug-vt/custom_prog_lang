#ifndef _SCRIPT
#define _SCRIPT

#include "instruction.hpp"
#include <unordered_map>
#include <vector>
#include <string>


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

    // interfaces
    int resolveOpStackIndex (int op_index);
    Value resolveOpValue (int op_index);
    Value* resolveOpPtr (int op_index);

    int coerceToInt (Value val);
    float coerceToFloat (Value val);
    std::string coerceToString (Value val);

    int resolveOpAsInt (int op_index);
    float resolveOpAsFloat (int op_index);
    std::string resolveOpAsString (int op_index);
    int resolveOpAsStackIndex (int op_index);
    int resolveOpAsFuncIndex (int op_index);
    int resolveOpAsReg (int op_index);

    int resolveStackIndex (int index);
    Value getStackValue (int index);
};

#endif
