#ifndef _INSTRUCTION_TABLE
#define _INSTRUCTION_TABLE

#include <unordered_map>
#include <string>
#include <vector>

typedef int OpTypes;

struct InstrLookup
{
  int opcode;
  std::vector<OpTypes> op_list;

  InstrLookup () { }
  InstrLookup (int opcode, std::vector<OpTypes> op_list);
};


class InstrLookupTable
{
  public:
    InstrLookupTable ();
    InstrLookup lookup (std::string mnemonic);

  private:
    std::unordered_map<std::string, InstrLookup> instr_lookup;

};

#endif