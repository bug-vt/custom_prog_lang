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
    // using singleton pattern to ensure the class has one instance
    // and provide global access
    static InstrLookupTable& instance ();
    InstrLookup lookup (std::string mnemonic);
    bool isInstr (std::string mnemonic);

  private:
    std::unordered_map<std::string, InstrLookup> instr_lookup;

    InstrLookupTable ();
};

#endif
