#ifndef _FUNC_TABLE
#define _FUNC_TABLE

#include <unordered_map>
#include <string>


// A function table node.
// Hold information to describe a function's scope and stack frame.
struct FuncNode
{
  int func_index;
  int entry_point;
  int param_count;
  int local_data_size;

  FuncNode ();
  FuncNode (int index, int entry_pos);
};


class FuncTable
{
  public:
    FuncTable ();
    int addFunc (std::string func_name, int entry_point);
    FuncNode getFunc (std::string func_name);
    void setFunc (std::string func_name, 
                  int param_count, 
                  int local_data_size);

  private:
    int func_index;
    std::unordered_map<std::string, FuncNode> func_table;

};

#endif
