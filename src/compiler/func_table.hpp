#ifndef FUNC_TABLE_HPP
#define FUNC_TABLE_HPP

#include <string>
#include <unordered_map>


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
