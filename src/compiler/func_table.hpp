#ifndef FUNC_TABLE_HPP
#define FUNC_TABLE_HPP

#include <vector>
#include <string>
#include "icode.hpp"


// A function table node.
// Hold information to describe a function's scope and stack frame.
struct FuncInfo
{
  std::string func_name; 
  int param_count;
  ICode icode;

  FuncInfo ();
  FuncInfo (std::string name);
};


class FuncTable
{
  public:
    FuncTable ();
    int addFunc (std::string func_name);
    FuncInfo getFunc (std::string func_name);
    int getFuncIndex (std::string func_name);
    void setFunc (int func_index, 
                  int param_count); 
    FuncInfo& at (int index);
    int size ();
    void print ();

  private:
    int func_count;
    std::vector<FuncInfo> func_table;

  friend class CodeGen;
};

#endif
