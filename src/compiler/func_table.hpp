#ifndef FUNC_TABLE_HPP
#define FUNC_TABLE_HPP

#include <string>
#include <unordered_map>
#include "token.hpp"

struct Param
{
  Token name;
  bool is_ref;

  Param () { }
  Param (Token name, bool is_ref)
  {
    this->name = name;
    this->is_ref = is_ref;
  }
};

class FuncTable
{
  public:
    FuncTable ();
    void addFunc (std::string name, int param_count);
    int getFunc (std::string name);
    bool isFunc (std::string name);
    void print ();

  private:
    std::unordered_map<std::string, int> func_table;
};

#endif
