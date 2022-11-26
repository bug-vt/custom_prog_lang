#ifndef SCRIPT_HPP
#define SCRIPT_HPP

#include "instruction.hpp"
#include <unordered_map>
#include <vector>
#include <string>
#include "stack.hpp"


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
    Script ();
    void load (std::string file_name);
    void reset (int argc, char **argv);
    void execute ();

  private:
    // header
    int global_data_size;
    char is_main_func_present;
    int main_func_index;

    // registers
    int instr_index;  // instruction pointer
    Value ret_val;    // return value register

    // instruction stream
    std::vector<Instr> instr_stream;

    // runtime stack
    Stack stack;

    // string table
    std::unordered_map<int, std::string> str_table;

    // function table
    std::unordered_map<int, Func> func_table;

    // ------------------------------------------
    // instruction handler
    std::unordered_map<int, void (Script::*)()> instr_handler;

    // instruction execution
    void instrMov ();
    void instrRef ();
    void instrLw ();
    void instrSw ();
    void instrArithmetic ();
    void instrBitwise ();
    void instrGetChar ();
    void instrSetChar ();
    void instrCmp ();
    void instrJmp ();
    void instrBranch ();
    void instrPush ();
    void instrPop ();
    void instrCall ();
    void instrRet ();
    void instrPause ();
    void instrExit ();
    void instrPrint ();
    void instrTime ();
    void instrRandInt ();

    // ------------------------------------------
    // internal interface for resolution/coercion
    int resolveOpStackIndex (int op_index);
    Value resolveOpValue (int op_index);
    void resolveOpCopy (int op_index, Value val);

    int coerceToInt (Value val);
    float coerceToFloat (Value val);
    std::string coerceToString (Value val);

    int resolveOpAsInt (int op_index);
    float resolveOpAsFloat (int op_index);
    std::string resolveOpAsString (int op_index);
    int resolveOpAsStackIndex (int op_index);
    int resolveOpAsInstrIndex (int op_index);
    int resolveOpAsFuncIndex (int op_index);
    int resolveOpAsReg (int op_index);
};

#endif
