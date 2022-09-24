#ifndef _LOADER
#define _LOADER

#include <fstream>
#include "script.hpp"

class Loader
{
  public:
    Loader (std::ifstream &binary);

    void loadHeader (std::vector<Value> &stack,
                          int &global_data_size,
                          char &is_main_func_present,
                          int &main_func_index);
    void loadInstrStream (std::vector<Instr> &instr_stream);
    void loadStringTable (std::unordered_map<int, std::string> &str_table);
    void loadFuncTable (std::unordered_map<int, Func> &func_table);

  private:
    std::ifstream &binary;
};

#endif
