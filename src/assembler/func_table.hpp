#ifndef _FUNC_TABLE
#define _FUNC_TABLE

#include <vector>
#include <string>


// A function table node.
// Hold information to describe a function's scope and stack frame.
struct FuncInfo
{
  std::string func_name; 
  int entry_point;
  int param_count;
  int local_data_size;

  FuncInfo ();
  FuncInfo (std::string name, int entry_pos);
};


class FuncTable
{
  public:
    FuncTable ();
    int addFunc (std::string func_name, int entry_point);
    FuncInfo getFunc (std::string func_name);
    int getFuncIndex (std::string func_name);
    void setFunc (int func_index, 
                  int param_count, 
                  int local_data_size);
    FuncInfo at (int index);
    int size ();
    void print ();

  private:
    int func_count;
    std::vector<FuncInfo> func_table;
};

#endif
